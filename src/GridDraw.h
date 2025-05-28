#pragma once
#include <Windows.h>

#include "MyBitMap/MyBitMap.h"
#include "Field.h"

class DirectDraw final {
private:
    const CellField& fieldMatrix;

    static Rect getRect(const uint16_t x, const uint16_t y) {
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

public:
    explicit DirectDraw(const CellField& field): fieldMatrix(field) {
    }

    // ~DirectDraw() override = default;

    // TODO:
    //  the window itself has a 1px black border, which makes the top and left borders of the grid
    //  seem as if they're 2px wide, which looks a bit weird.
    //  Solution should be to shift *all* grid drawing 1px up and left
    static void drawBlankCellGrid() {
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

    void drawCell(const uint16_t x, const uint16_t y) const {
        const Rect cellRect = getRect(x, y);

        const bool prevCellState = fieldMatrix.getCellState_Current(x, y);
        const bool currentCellState = fieldMatrix.getCellState_Next(x, y);
        if (prevCellState != currentCellState) {
            // TODO: figure out which one of these is faster, or if there's no significant difference then just choose one
            if constexpr (true) {
                InvertRect(&cellRect);
            } else {
                if (currentCellState == ALIVE) {
                    FillRect(&cellRect, &qd.black);
                } else {
                    EraseRect(&cellRect);
                }
            }
        }
    }

    void drawAll() const {
        for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
            for (uint16_t r = 0; r < MAX_ROWS; r++) {
                drawCell(c, r);
            }
        }
    }

    static void drawBlank() {
        EraseRect(&MyGlobals::GRID_RECT);
    }

    static void drawFull() {
        // FIXME:
        //  im pretty sure this call to qd will not work and will cause issues.
        //  fix it the same way as I did in Button
        FillRect(&MyGlobals::GRID_RECT, &qd.black);
    }
};

using GridBitMap = MyBitMap<
    MyGlobals::GRID_WIDTH,
    MyGlobals::GRID_HEIGHT
>;

class BitMapDraw final {
private:
    enum class DrawMode: short { // NOLINT(*-enum-size) : functions that use the underlying enum take a short
        ReplaceAll = srcCopy,
        CopySetPixels = srcOr,
        ToggleSetPixels = srcXor,
        EraseSetPixels = srcBic,
    };

    const CellField& fieldMatrix;

    const GridBitMap bitmap{};

    /*
        // class CachedQuickdrawOperationBitMap {
    // private:
    //     GridBitMap bitmap{}; // NOTE: hardcoded bitmap template specialization
    //
        // public:
    //     template <typename Functor>
    //     explicit CachedQuickdrawOperationBitMap(const WindowPtr mainWindow, Functor&& draw) {
    //         bitmap.blank();
    //
        //         draw();
    //
        //         CopyBits(
    //             &mainWindow->portBits, &bitmap.getBits(),
    //             &MyGlobals::GRID_RECT, &bitmap.getBounds(), // NOTE: hardcoded reduced srcRect bounds
    //             srcCopy,
    //             nullptr
    //         );
    //     }
    //
        //     void copyOnto(const GridBitMap& otherBitMap) const {
        //         CopyBits(
    //             &bitmap.getBits(), &otherBitMap.getBits(),
    //             &bitmap.getBounds(), &otherBitMap.getBounds(),
    //             srcOr,
    //             nullptr
    //         );
    //     }
    // };
*/

    static void copyBitMapToBitMap(const GridBitMap& srcBitMap, const GridBitMap& destBitMap, const DrawMode drawMode) {
        CopyBits(
            &srcBitMap.getBits(), &destBitMap.getBits(),
            &srcBitMap.getBounds(), &destBitMap.getBounds(),
            static_cast<short>(drawMode),
            nullptr
        );
    }

    static void copyBitMapToWindow(const GridBitMap& srcBitMap, const WindowPtr window, const DrawMode drawMode) {
        CopyBits(
            &srcBitMap.getBits(), &window->portBits,
            &srcBitMap.getBounds(), &MyGlobals::GRID_RECT,
            static_cast<short>(drawMode),
            nullptr
        );
    }

    void drawBlankCellGridHelper(const bool erase) const {
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

    void drawBlankCellGrid() const {
        drawBlankCellGridHelper(false);
    }

    void eraseBlankCellGrid() const {
        drawBlankCellGridHelper(true);
    }

public:
    explicit BitMapDraw(const CellField& field): fieldMatrix(field) {
    }

    void drawCell(const uint16_t x, const uint16_t y) const {
        const bool prevCellState = fieldMatrix.getCellState_Current(x, y);
        const bool currentCellState = fieldMatrix.getCellState_Next(x, y);
        if (prevCellState != currentCellState) {
            bitmap.drawSquare(x, y, CELL_SIZE);
        }
    }

    void drawAll(const WindowPtr mainWindow) const {
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

    void drawBlank(const WindowPtr mainWindow) const {
        bitmap.blank();
        drawBlankCellGrid();
        copyBitMapToWindow(bitmap, mainWindow, DrawMode::ReplaceAll);
    }

    void drawFull(const WindowPtr mainWindow) const {
        bitmap.fill();
        // Drawing grid would be a no-op since the whole thing is full anyway
        copyBitMapToWindow(bitmap, mainWindow, DrawMode::ReplaceAll);
    }
};
