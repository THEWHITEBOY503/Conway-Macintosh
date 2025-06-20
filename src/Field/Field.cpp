#include "Field.h"

#include <random>

#include <OSUtils.h>

template <typename T>
class MyRandomNumberGenerator {
private:
    const uint32_t seed = TickCount();
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

/****** getters and setters for `next` generation ******/
template <uint16_t rowCount, uint16_t columnCount>
bool Field<rowCount, columnCount>::getCellState_Next(const uint16_t column, const uint16_t row) const {
    return this->buffers.getNext().getBitState(column, row);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::setColumnState_Next(const uint16_t column, const uint32_t value) {
    this->buffers.getNext().setColumnState(column, value);
}

/****** getters and setters for `current` generation ******/
template <uint16_t rowCount, uint16_t columnCount>
bool Field<rowCount, columnCount>::getCellState_Current(const uint16_t column, const uint16_t row) const {
    return this->buffers.getCurrent().getBitState(column, row);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::setCell_Current(const uint16_t column, const uint16_t row, const bool value) {
    return this->buffers.getCurrent().setBit(column, row, value);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::toggleCell_Current(const uint16_t column, const uint16_t row) {
    return this->buffers.getCurrent().toggleBit(column, row);
}

template <uint16_t rowCount, uint16_t columnCount>
uint32_t Field<rowCount, columnCount>::getColumnState_Current(const uint16_t column) const {
    return this->buffers.getCurrent().getColumnState(column);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::setColumnState_Current(const uint16_t column, const uint32_t value) {
    this->buffers.getCurrent().setColumnState(column, value);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::setColumn_Current(const uint16_t column, const bool value) {
    this->buffers.getCurrent().setColumn(column, value);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::setRow_Current(const uint16_t column, const bool value) {
    this->buffers.getCurrent().setRow(column, value);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::setField_Current(const bool value) {
    this->buffers.getCurrent().setMatrix(value);
}

template <uint16_t rowCount, uint16_t columnCount>
bool Field<rowCount, columnCount>::cellHasChangedSinceLastGeneration(const uint16_t column, const uint16_t row) const {
    const bool currentCellState = getCellState_Current(column, row);
    const bool prevCellState = getCellState_Next(column, row);

    return currentCellState != prevCellState;
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::setRandomStart() {
    static MyRandomNumberGenerator<uint32_t> rng{};

    for (int c = 0; c < columnCount; c++) {
        const auto randomNum = rng.get();
        setColumnState_Next(c, randomNum);
    }

    buffers.swap();
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::clearField() {
    setField_Current(DEAD);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::fillField() {
    setField_Current(ALIVE);
}

template <uint16_t rowCount, uint16_t columnCount>
void Field<rowCount, columnCount>::createNextGeneration() {
    for (int c = 0; c < columnCount; c++) {
        uint32_t columnNext = 0;
        for (int r = 0; r < rowCount; r++) {
            // We need to be checking in a 3x3 grid for a cells neighbors
            const auto iterateOverNeighbors = [&c, &r](auto functor) {
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
            iterateOverNeighbors([&livingNeighbors, this](auto x, auto y) {
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

    buffers.swap();
}

template class Field<MAX_ROWS, MAX_COLUMNS>;
