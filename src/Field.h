#pragma once

#include <cstdint>

/* Linear Congruential Generator (LCG) for generating random numbers */
static unsigned long rand_state = 123456789; /* RNG Seed */
inline uint16_t rand_gen() {
    // Random bullshit happens here
    rand_state *= 1103515245;
    rand_state += 12345;

    return (rand_state / UINT16_MAX) % INT16_MAX;
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
            setColumnState(array, column, UINT32_MAX);
        } else {
            setColumnState(array, column, 0);
        }
    }

    static void setRow(uint32_t array[columnCount], const uint16_t column, const bool value) {
        for (int r = 0; r < rowCount; r++) {
            setCell(array, column, r, value);
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

    /****** end of generic getters and setters ******/

    void setRandomStart() {
        for (int c = 0; c < columnCount; c++) {
            this->current[c] = 0; /* Empties out the entire column */
            /* Same as the above loop, but we're using r, and checking it against field->rows */
            for (int r = 0; r < rowCount; r++) {
                // 50% chance of being true
                if (!randBool()) {
                    /* field->next is an unsigned long, so it's a 32-bit binary value.
                    We start the entire column as 0, so that means it's now 00000000000000000000000000000000
                    then, rand_gen() is run to return either a 1 or a 0 The % operator returns the remainder, so if we're
                    dividing by 2, the remainder can only be either 1 (odd number) or 0 (even number). If the condition to
                    make a cell alive is met (in our case it's 0, but it could just as easily be 1), the bit for that cell
                    is set to 1. Let's say that we somehow manage to only have row 16 come alive. field->current[c] then
                    becomes: 00000000000000001000000000000000 Because `|= ((unsigned long)1 << r` basically moves the
                    'cursor' back r places, then sets the bit at its position to 1. */
                    setCell_Next(c, r, true);
                }
            }
        }
    }

    void clearField() {
        for (int c = 0; c < columnCount; c++) {
            // Clear all cells - sets all iterations of field->current to 0
            setColumn_Next(c, DEAD);
        }
    }

    void fillField() {
        for (int c = 0; c < columnCount; c++) {
            setColumn_Next(c, ALIVE);
        }
    }

    // This is the function that decides which cells in the next iteration are alive/dead.
    void createNextGeneration();
};

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::createNextGeneration() {
    for (int c = 0; c < columnCount; c++) {
        unsigned long rowNext = 0; /* Another 32-bit-long value */
        for (int r = 0; r < rowCount; r++) {
            /* Basically checking "Is r lower than (the number of rows)"? */
            const int currentState =
                (this->current[c] & ((unsigned long)1 << r)) !=
                0; /* Remember that the << operator shifts the bit it's operating on from right to left r times. */
            int neighbors = 0;
            /* Creates and defines our variable that will tell us how many alive neighbors we have */
            /* We need to be checking in a 3x3 grid for a cells neighbors */
            for (int x = c - 1; x <= c + 1; x++) {
                /* x is created with the result of c-1, then each time the loop iterates
                                                                     it checks if it's greater than or equal to the result of c+1. So, if
                                                                     we're in column 5, it's going to check columns 3, 4 and 5. */
                for (int y = r - 1; y <= r + 1; y++) {
                    /* Same deal as above but with rows */
                    if (x == c && y == r)
                        continue;
                    /* We don't count the cell that we're checking so we skip it. You can't be your own
                                                            neighbor. */
                    /* These two lines make sure x and y are within bounds */
                    if (x >= 0 && x < columnCount && y >= 0 && y < rowCount &&
                        /* Check if the cell from column x at position y is alive */
                        (this->current[x] & ((unsigned long)1 << y))) {
                        /* If it is, increase neighbors by 1 */
                        neighbors++;
                    }
                }
            }

            if (currentState != 0) {
                // If the cell is alive (currentState == 1), we check if it has 2 or 3 alive neighbors.
                // If it does, it stays alive (inserts a 1 into its position in the row).
                if (neighbors == 2 || neighbors == 3)
                    rowNext |= (unsigned long)1 << r;
            } else {
                // If the cell is dead (currentState == 0), we check if it has 3 alive neighbors, and if it does, it
                // comes alive (a 1 is inserted in its position in the row)
                if (neighbors == 3)
                    rowNext |= (unsigned long)1 << r;
            }
        }
        /* Commit the results of the above function into the next iteration */
        this->next[c] = rowNext;
    }
}

#include "conway.h"

template class Field<MAX_ROWS, MAX_COLUMNS>;
