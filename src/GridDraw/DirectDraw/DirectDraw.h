#pragma once

#include <cstdint>

class DirectDraw final {
public:
    explicit DirectDraw();

    static void drawBlankCellGrid();

    static void toggleCell(const uint16_t x, const uint16_t y);

    static void drawAll();

    static void drawBlank();
};
