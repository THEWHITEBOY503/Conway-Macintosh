#pragma once
#include <Quickdraw.h>

#include "conway.h"

template <short width, short height>
class MyBitMap {
private:
    static constexpr short rowBytes = ((width + 15) >> 4) << 1; // round to even
    static constexpr auto bounds = Rect{
        .top = 0,
        .left = 0,
        .bottom = height,
        .right = width,
    };

    BitMap bitmap;

    // Setter allows me to add bounds checking during debugging later if I want.
    // Also isolates the IDE warning from setting a char* to a u_char value to just this method.
    inline void setByte(const long offset, const u_char value) const {
        // Suppressing lint warning because this seems to be inaccurate for the quirky mac platform we're targeting
        // i.e.: Setting the data to signed char values seems to leave the high bit unset, so we need to use unsigned
        bitmap.baseAddr[offset] = value; // NOLINT(*-narrowing-conversions)
    }

public:
    static constexpr long numBytes = (static_cast<long>(height) * static_cast<long>(rowBytes));

    explicit MyBitMap(): bitmap() {
        bitmap.bounds = bounds;

        bitmap.rowBytes = rowBytes;

        bitmap.baseAddr = NewPtr(numBytes);
        bitmap.baseAddr = StripAddress(bitmap.baseAddr);
    }

    [[nodiscard]] const BitMap& getBits() const {
        return bitmap;
    }

    [[nodiscard]] const Rect& getBounds() const {
        return bitmap.bounds;
    }

    void invert() const {
        for (short i = 0; i < numBytes; ++i) {
            bitmap.baseAddr[i] = ~bitmap.baseAddr[i];
        }
    }

    void blank() const {
        std::fill_n(bitmap.baseAddr, numBytes, 0);
    }

    void fill() const {
        std::fill_n(bitmap.baseAddr, numBytes, std::numeric_limits<u_char>::max());
    }

    void drawSquare(const uint16_t column, const uint16_t row, const uint8_t size) const {
        const uint16_t x = (column * size);
        const uint16_t y = (row * size);
        for (uint_fast8_t i = 1; i < size; ++i) {
            drawHorizontalLine(x, y + i, size);
        }
    }

    void drawDot(const uint16_t column, const uint16_t row) const {
        const uint16_t x = (column * CELL_SIZE) + (CELL_SIZE / 2);
        const uint16_t y = (row * CELL_SIZE) + (CELL_SIZE / 2);
        drawVerticalLine(x, y, 1);
    }

    void drawWeirdLineThing(const uint16_t column, const uint16_t row, const uint8_t size) const {
        drawVerticalLine((column * CELL_SIZE) + (CELL_SIZE / 2), row * CELL_SIZE, size);
    }

    // TODO: test this with values of x not divisible by 8
    void drawVerticalLine(const uint16_t x, const uint16_t y, const uint16_t length) const {
        const uint16_t x_quotient = x / 8;
        const uint8_t x_remainder = 7 - (x % 8);

        for (long i = 0; i < length; ++i) {
            const long row = (y + i + 1) * rowBytes;
            const long offset = row + x_quotient;

            bitmap.baseAddr[offset] |= static_cast<u_char>(1 << x_remainder);
        }
    }

    // TODO: test handling of length that isn't divisible by 8
    void drawHorizontalLine(const uint16_t x, const uint16_t y, const uint16_t length) const {
        const uint16_t x_quotient = x / 8;
        const uint8_t x_remainder = (x % 8);

        const uint16_t length_quotient = length / 8;
        const uint8_t length_remainder = (length % 8);

        const long row = y * rowBytes;

        const long initialOffset = row + x_quotient;
        for (long i = 0; i < length_quotient; ++i) {
            const long offset = initialOffset + i;

            setByte(offset, std::numeric_limits<u_char>::max());
        }

        // If column has unaligned offset, fix the first and last bytes
        if (x_remainder != 0) {
            const u_char firstValue = std::numeric_limits<u_char>::max() >> (x_remainder);
            const long firstOffset = initialOffset;
            const u_char lastValue = std::numeric_limits<u_char>::max() << (7 - x_remainder);
            const long lastOffset = initialOffset + (length_quotient - 1);

            setByte(firstOffset, firstValue);
            setByte(lastOffset, lastValue);
        }

        if (length_remainder != 0) {
            const long finalOffset = initialOffset + length_quotient;
            const u_char value = std::numeric_limits<u_char>::max() << (7 - length_remainder);

            setByte(finalOffset, value);
        }
    }

    void drawVerticalGridLine(const uint16_t column, const uint16_t row, const uint16_t length) const {
        drawVerticalLine(column * CELL_SIZE, row * CELL_SIZE, length);
    }

    void drawHorizontalGridLine(const uint16_t column, const uint16_t y, const uint16_t length) const {
        drawHorizontalLine(column * CELL_SIZE, y * CELL_SIZE, length);
    }
};
