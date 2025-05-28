#include "BitMapDraw.h"

enum class DrawMode: short { // NOLINT(*-enum-size) : functions that use the underlying enum take a short
    ReplaceAll = srcCopy,
    CopySetPixels = srcOr,
    ToggleSetPixels = srcXor,
    EraseSetPixels = srcBic,
};

void copyBitMapToBitMap(const GridBitMap& srcBitMap, const GridBitMap& destBitMap, const DrawMode drawMode) {
    CopyBits(
        &srcBitMap.getBits(), &destBitMap.getBits(),
        &srcBitMap.getBounds(), &destBitMap.getBounds(),
        static_cast<short>(drawMode),
        nullptr
    );
}

void copyBitMapToWindow(const GridBitMap& srcBitMap, const WindowPtr window, const DrawMode drawMode) {
    CopyBits(
        &srcBitMap.getBits(), &window->portBits,
        &srcBitMap.getBounds(), &MyGlobals::GRID_RECT,
        static_cast<short>(drawMode),
        nullptr
    );
}

void BitMapDraw::drawBlankCellGridHelper(const bool erase) const {
    static bool firstCall = true;
    static GridBitMap blankGridBitMap{};
    if (firstCall) {
        firstCall = false;

        blankGridBitMap.blank();
        for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
            blankGridBitMap.drawVerticalGridLine(c, 0, MyGlobals::GRID_HEIGHT);
        }
        for (uint16_t r = 0; r < MAX_ROWS; r++) {
            blankGridBitMap.drawHorizontalGridLine(0, r, MyGlobals::GRID_WIDTH);
        }
    }

    // Made this a lambda so the result can be stored in a const variable, which makes me happy
    const auto modePicker = [erase] {
        if (erase) {
            // Use DrawMode::EraseSetPixels (srcBic) to "subtract" the grid from whatever is already on the bitmap.
            // Allows us to pass in a bitmap (that we intend to xor with the screen) and "repair" the grid.
            return DrawMode::EraseSetPixels;
        } else {
            // Use DrawMode::CopySetPixels (srcOr) to basically "add" the grid to whatever is already on the bitmap (think of it like an overlay).
            // Allows us to pass in a partially erased bitmap and "repair" the grid.
            return DrawMode::CopySetPixels;
        }
    };
    const DrawMode mode = modePicker();

    copyBitMapToBitMap(blankGridBitMap, this->bitmap, mode);
}

void BitMapDraw::drawBlankCellGrid() const {
    drawBlankCellGridHelper(false);
}

void BitMapDraw::eraseBlankCellGrid() const {
    drawBlankCellGridHelper(true);
}

BitMapDraw::BitMapDraw(const CellField& field): fieldMatrix(field) {
}

void BitMapDraw::drawCell(const uint16_t x, const uint16_t y) const {
    const bool prevCellState = fieldMatrix.getCellState_Current(x, y);
    const bool currentCellState = fieldMatrix.getCellState_Next(x, y);
    if (prevCellState != currentCellState) {
        bitmap.drawSquare(x, y, CELL_SIZE);
    }
}

void BitMapDraw::drawAll(const WindowPtr mainWindow) const {
    bitmap.blank();
    const auto draw = [this] {
        for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
            for (uint16_t r = 0; r < MAX_ROWS; r++) {
                drawCell(c, r);
            }
        }
    };

    draw();

    eraseBlankCellGrid();
    copyBitMapToWindow(bitmap, mainWindow, DrawMode::ToggleSetPixels);
}

void BitMapDraw::drawBlank(const WindowPtr mainWindow) const {
    bitmap.blank();
    drawBlankCellGrid();
    copyBitMapToWindow(bitmap, mainWindow, DrawMode::ReplaceAll);
}

void BitMapDraw::drawFull(const WindowPtr mainWindow) const {
    bitmap.fill();
    // Drawing grid would be a no-op since the whole thing is full anyway
    copyBitMapToWindow(bitmap, mainWindow, DrawMode::ReplaceAll);
}
