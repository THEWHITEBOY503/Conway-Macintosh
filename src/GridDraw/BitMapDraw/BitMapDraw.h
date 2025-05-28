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

    void drawBlankCellGridHelper(const bool erase) const;

    void drawBlankCellGrid() const;

    void eraseBlankCellGrid() const;

public:
    explicit BitMapDraw();

    void drawCell(const CellField& field, const uint16_t x, const uint16_t y) const;

    void drawAll(const WindowPtr mainWindow, const CellField& field) const;

    void drawBlank(const WindowPtr mainWindow) const;

    void drawFull(const WindowPtr mainWindow) const;
};
