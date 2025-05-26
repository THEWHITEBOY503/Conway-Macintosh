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

    void blank() const {
        std::fill_n(bitmap.baseAddr, numBytes, 0);
    }

    void fill() const {
        std::fill_n(bitmap.baseAddr, numBytes, std::numeric_limits<u_char>::max());
    }

    // TODO: I don't think this works for sizes other than CELL_SIZE (8) rn lol
    void drawSquare(const uint16_t column, const uint16_t row, const uint8_t size) const {
        for (long i = 1; i < size; ++i) {
            const long y = ((row * 8) + i) * rowBytes;
            const long offset = y + column;

            bitmap.baseAddr[offset] = std::numeric_limits<char>::max();
        }
    }

    void drawDot(const uint16_t column, const uint16_t row, const uint8_t size) const {
        const long i = size / 2;
        const long y = ((row * 8) + i) * rowBytes;
        const long offset = y + column;

        bitmap.baseAddr[offset] |= 0b00001000;
    }

    void drawWeirdLineThing(const uint16_t column, const uint16_t row, const uint8_t size) const {
        drawVerticalLine((column * CELL_SIZE) + 4, row * CELL_SIZE);
    }

    // TODO: test this with values of x not divisible by 8
    void drawVerticalLine(const uint16_t x, const uint16_t y, const uint16_t length) const {
        const uint16_t x_quotient = x / 8;
        const uint8_t x_remainder = 7 - (x % 8);

        for (long i = 0; i < length; ++i) {
            const long row = (y + i + 1) * rowBytes;
            const long offset = row + (x_quotient);

            bitmap.baseAddr[offset] |= static_cast<u_char>(1 << x_remainder);
        }
    }

    void drawHorizontalLine(const uint16_t x, const uint16_t y, const uint16_t length) const {
        const uint16_t x_quotient = x / 8;
        const uint8_t x_remainder = 7 - (x % 8);

        const uint16_t length_quotient = length / 8;
        const uint8_t length_remainder = (length % 8);

        const long row = y * rowBytes;

        long offset = row + x_quotient;
        for (long i = 0; i < length_quotient; ++i) {
            offset += i;

            // TODO: shr column by x_remainder # of bits (this has a performance hit)
            // TODO: handle length that isn't divisible by 8
            bitmap.baseAddr[offset] = std::numeric_limits<u_char>::max();
        }

        if (length_remainder != 0) {
            bitmap.baseAddr[offset + 1] = (std::numeric_limits<u_char>::max() << (7 - length_remainder));
        }
    }

    void drawVerticalGridLine(const uint16_t column, const uint16_t row, const uint16_t length) const {
        drawVerticalLine(column * CELL_SIZE, row * CELL_SIZE, length);
    }

    void drawHorizontalGridLine(const uint16_t column, const uint16_t y, const uint16_t length) const {
        drawHorizontalLine(column * CELL_SIZE, y * CELL_SIZE, length);
    }
};
