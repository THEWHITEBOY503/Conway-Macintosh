#pragma once

#include <Windows.h>
#include "Field.h"

class DirectDraw final {
private:
    const CellField& fieldMatrix;

    static Rect getRect(const uint16_t x, const uint16_t y);

public:
    explicit DirectDraw(const CellField& field);

    // ~DirectDraw() override = default;

    static void drawBlankCellGrid();

    void drawCell(const uint16_t x, const uint16_t y) const;

    void drawAll() const;

    static void drawBlank();

    static void drawFull();
};
