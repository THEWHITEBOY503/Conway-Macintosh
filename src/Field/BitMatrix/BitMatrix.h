#pragma once

#include <cstdint>
#include <limits>
#include <algorithm>

template <typename T, uint16_t rowCount, uint16_t columnCount>
class BitMatrix {
private:
    static_assert((sizeof(T) * 8) == rowCount,
                  "The rowCount is purely informational of the number of bits the type T takes up. "
                  "If this assertion fails, then it means that that the informational parameter rowCount (or T)"
                  "is incorrect, which is a bug."
    );

    using Row = T;
    using Column = T;
    using BitState = bool;
    using Index = uint16_t;

    T array[columnCount] = {};

public:
    [[nodiscard]] bool getBitState(const uint16_t column, const uint16_t row) const {
        const auto state = (array[column] & (static_cast<T>(1) << row));

        return state;
    }

    void setBit(const uint16_t column, const uint16_t row, const bool value) {
        array[column] |= (static_cast<T>(value) << row);
    }

    void toggleBit(const uint16_t column, const uint16_t row) {
        array[column] ^= (static_cast<T>(1) << row);
    }

    [[nodiscard]] T getColumnState(const uint16_t column) const {
        return array[column];
    }

    /**
     * Directly set the column to the provided value
     */
    void setColumnState(const uint16_t column, const T value) {
        array[column] = value;
    }

    /**
     * Set every bit in column to the same provided boolean value
     */
    void setColumn(const uint16_t column, const bool value) {
        if (value) {
            setColumnState(column, std::numeric_limits<T>::max());
        } else {
            setColumnState(column, 0);
        }
    }

    void setRow(const uint16_t column, const bool value) {
        for (int r = 0; r < rowCount; r++) {
            setBit(column, r, value);
        }
    }

    void setMatrix(const bool value) {
        if (value) {
            std::fill_n(array, columnCount, std::numeric_limits<T>::max());
        } else {
            std::fill_n(array, columnCount, 0);
        }
    }
};
