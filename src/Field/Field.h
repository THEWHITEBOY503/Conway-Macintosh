#pragma once

#include <cstdint>
#include <random>

#include "conway.h"

template <typename T>
class MyRandomNumberGenerator {
private:
    constexpr static T seed = 123456789;
    std::default_random_engine randEngine{seed};

    std::uniform_int_distribution<T> uniformDist;

public:
    constexpr static T MIN = std::numeric_limits<T>::min();
    constexpr static T MAX = std::numeric_limits<T>::max();

    MyRandomNumberGenerator(): uniformDist{MIN, MAX} {
    }

    T get() {
        return uniformDist(randEngine);
    }
};

/* Linear Congruential Generator (LCG) for generating random numbers */
inline uint32_t rand_gen() {
    static MyRandomNumberGenerator<uint32_t> rng{};

    return rng.get();
}

inline bool randBool() {
    return rand_gen() % 2 == 1;
}

/*
A word to the uninitiated:

Field acts as a container for a 2D grid representation.

Field stores the size of the grid (rows and columns), the current state of the grid (current[MAX_COLUMNS]), and
the next state of the grid (next[MAX_COLUMNS]). We can access a field's properties with a pointer.

So, say I had MAX_COLUMNS set to 32 (as it is by default), which means I have [0]-[31].
If I wanted to refer to next at 7, I would use `field->next[7]`.
*/

/* This works as a sort of table -- As you see at the end, we call this 'table' "Field" */

// Unsigned 16-bit data type can hold values from 0 to 65,535
template <uint16_t rowCount, uint16_t columnCount>
class Field {
    using CellRow = uint32_t;
    using CellColumn = uint32_t;
    using CellState = bool;
    using CellIndex = uint16_t;

private:
    // [columnCount] makes current an array of the size of columnCount.
    // So, if columnCount is 32, then you would have current[0] through current[31].
    uint32_t current[columnCount] = {}; // 4 bytes (32 bits) * MAX_COLUMNS
    uint32_t next[columnCount] = {};

    static bool getCellState(const uint32_t array[columnCount], const uint16_t column, const uint16_t row) {
        const auto state = (array[column] & (static_cast<uint32_t>(1) << row));

        return state;
    }

    static void setCell(uint32_t array[columnCount], const uint16_t column, const uint16_t row, const bool value) {
        array[column] |= (static_cast<uint32_t>(value) << row);
    }

    static void toggleCell(uint32_t array[columnCount], const uint16_t column, const uint16_t row) {
        array[column] ^= (static_cast<uint32_t>(1) << row);
    }

    static uint32_t getColumnState(uint32_t array[columnCount], const uint16_t column) {
        return array[column];
    }

    /**
     * Directly set the column to the provided value
     */
    static void setColumnState(uint32_t array[columnCount], const uint16_t column, const uint32_t value) {
        array[column] = value;
    }

    /**
     * Set every cell (bit) in column to the same provided boolean value
     */
    static void setColumn(uint32_t array[columnCount], const uint16_t column, const bool value) {
        if (value) {
            setColumnState(array, column, std::numeric_limits<uint32_t>::max());
        } else {
            setColumnState(array, column, 0);
        }
    }

    static void setRow(uint32_t array[columnCount], const uint16_t column, const bool value) {
        for (int r = 0; r < rowCount; r++) {
            setCell(array, column, r, value);
        }
    }

    static void setField(uint32_t array[columnCount], const bool value) {
        if (value) {
            std::fill_n(array, columnCount, std::numeric_limits<uint32_t>::max());
        } else {
            std::fill_n(array, columnCount, 0);
        }
    }

public:
    /****** getters and setters for `current` generation ******/
    [[nodiscard]] bool getCellState_Current(const uint16_t column, const uint16_t row) const {
        return getCellState(this->current, column, row);
    }

    void setCell_Current(const uint16_t column, const uint16_t row, const bool value) {
        return setCell(this->current, column, row, value);
    }

    void toggleCell_Current(const uint16_t column, const uint16_t row) {
        return toggleCell(this->current, column, row);
    }

    uint32_t getColumnState_Current(const uint16_t column) {
        return getColumnState(this->current, column);
    }

    void setColumnState_Current(const uint16_t column, const uint32_t value) {
        setColumnState(this->current, column, value);
    }

    void setColumn_Current(const uint16_t column, const bool value) {
        setColumn(this->current, column, value);
    }

    void setRow_Current(const uint16_t column, const bool value) {
        setRow(this->current, column, value);
    }

    void setField_Current(const bool value) {
        setField(this->current, value);
    }

    /****** getters and setters for `next` generation ******/
    [[nodiscard]] bool getCellState_Next(const uint16_t column, const uint16_t row) const {
        return getCellState(this->next, column, row);
    }

    void setCell_Next(const uint16_t column, const uint16_t row, const bool value) {
        return setCell(this->next, column, row, value);
    }

    void toggleCell_Next(const uint16_t column, const uint16_t row) {
        return toggleCell(this->next, column, row);
    }

    uint32_t getColumnState_Next(const uint16_t column) {
        return getColumnState(this->next, column);
    }

    void setColumnState_Next(const uint16_t column, const uint32_t value) {
        setColumnState(this->next, column, value);
    }

    void setColumn_Next(const uint16_t column, const bool value) {
        setColumn(this->next, column, value);
    }

    void setRow_Next(const uint16_t column, const bool value) {
        setRow(this->next, column, value);
    }

    void setField_Next(const bool value) {
        setField(this->next, value);
    }

    /****** end of generic getters and setters ******/

    void setRandomStart() {
        setField_Current(DEAD);

        for (int c = 0; c < columnCount; c++) {
            const auto randomNum = rand_gen();
            setColumnState_Next(c, randomNum);
        }
    }

    void clearField() {
        setField_Next(DEAD);
    }

    void fillField() {
        setField_Next(ALIVE);
    }

    // This is the function that decides which cells in the next iteration are alive/dead.
    void createNextGeneration();
};

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::createNextGeneration() {
    for (int c = 0; c < columnCount; c++) {
        uint32_t columnNext = 0;
        for (int r = 0; r < rowCount; r++) {
            // We need to be checking in a 3x3 grid for a cells neighbors
            const auto iterateOverNeighbors = [&](auto functor) {
                for (int x = c - 1; x <= c + 1; x++) {
                    for (int y = r - 1; y <= r + 1; y++) {
                        // We don't count the cell that we're checking so we skip it.
                        // You can't be your own neighbor.
                        if (x == c && y == r) {
                            continue;
                        }

                        // These lines make sure x and y are within bounds
                        if (x >= 0 && x < columnCount &&
                            y >= 0 && y < rowCount
                        ) {
                            functor(x, y);
                        }
                    }
                }
            };

            /* our variable that will tell us how many alive neighbors we have */
            int livingNeighbors = 0;
            iterateOverNeighbors([&](auto x, auto y) {
                if (getCellState_Current(x, y) == ALIVE) {
                    livingNeighbors++;
                }
            });

            if (getCellState_Current(c, r) == ALIVE) {
                if (livingNeighbors == 2 || livingNeighbors == 3) {
                    columnNext |= static_cast<unsigned long>(1) << r;
                } else {
                    // Dies due to over/underpopulation
                }
            } else {
                if (livingNeighbors == 3) {
                    columnNext |= static_cast<unsigned long>(1) << r;
                } else {
                    // Does not come back to life
                }
            }
        }
        /* Commit the results of the above function into the next iteration */
        setColumnState_Next(c, columnNext);
    }
}

template class Field<MAX_ROWS, MAX_COLUMNS>;

using CellField = Field<MAX_ROWS, MAX_COLUMNS>;
