#pragma once

#include "BitMapDraw.h"
#include "Field.h"

class FieldUpdater final {
private:
    CellField field;
    const WindowPtr window;
    const BitMapDraw bitmapDraw;

    void draw() const;

public:
    void redraw() const;

    explicit FieldUpdater(const WindowPtr _window);

    void toggleSingleCell(const uint16_t column, const uint16_t row);

    void nextGeneration();

    void randomResetField();

    void clearField();

    void fillField();
};


