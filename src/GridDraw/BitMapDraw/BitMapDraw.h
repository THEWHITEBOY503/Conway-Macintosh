#pragma once

#include "MyBitMap.h"
#include "Field.h"

using GridBitMap = MyBitMap<
    MyGlobals::GRID_WIDTH,
    MyGlobals::GRID_HEIGHT
>;

class BitMapDraw final {
private:
    const GridBitMap bitmap;

    void drawBlankCellGridHelper(const bool erase) const;

    void drawBlankCellGrid() const;

    void eraseBlankCellGrid() const;

public:
    explicit BitMapDraw();

    void drawCell(const CellField& field, const uint16_t x, const uint16_t y) const;

    void drawAll(const WindowPtr mainWindow, const CellField& field) const;
    void drawAllForcefully(const WindowPtr mainWindow, const CellField& field) const;

    void drawBlank(const WindowPtr mainWindow) const;

    void drawFull(const WindowPtr mainWindow) const;
};
