#include "DirectDraw.h"

#include <MacWindows.h>

#include "conway.h"

Rect getRect(const uint16_t x, const uint16_t y) {
    /* Create outline by erasing a slightly smaller inner part of the cell */
    constexpr uint8_t PEN_WIDTH = 1;

    const auto rectLeft = static_cast<short>((x * CELL_SIZE) + PEN_WIDTH);
    const auto rectTop = static_cast<short>((y * CELL_SIZE) + PEN_WIDTH);

    /* Define a rectangle for the cell... */
    return Rect{
        .top = rectTop,
        .left = rectLeft,
        .bottom = static_cast<short>(rectTop + CELL_SIZE - PEN_WIDTH),
        .right = static_cast<short>(rectLeft + CELL_SIZE - PEN_WIDTH),
    };
}

DirectDraw::DirectDraw() = default;

// TODO:
//  the window itself has a 1px black border, which makes the top and left borders of the grid
//  seem as if they're 2px wide, which looks a bit weird.
//  Solution should be to shift *all* grid drawing 1px up and left
void DirectDraw::drawBlankCellGrid() {
    PenSize(1, 1);

    for (uint16_t c = 0; c <= MAX_COLUMNS; c++) {
        const auto columnX = static_cast<short>(c * CELL_SIZE);
        MoveTo(columnX, 0);
        LineTo(columnX, MyGlobals::GRID_HEIGHT);
    }
    for (uint16_t r = 0; r <= MAX_ROWS; r++) {
        const auto rowY = static_cast<short>(r * CELL_SIZE);
        MoveTo(0, rowY);
        LineTo(MyGlobals::GRID_WIDTH, rowY);
    }

    PenNormal();
}

void DirectDraw::toggleCell(const uint16_t x, const uint16_t y) {
    const Rect cellRect = getRect(x, y);

    InvertRect(&cellRect);
}

void DirectDraw::drawAll() {
    for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
        for (uint16_t r = 0; r < MAX_ROWS; r++) {
            toggleCell(c, r);
        }
    }
}

void DirectDraw::drawBlank() {
    EraseRect(&MyGlobals::GRID_RECT);
}
