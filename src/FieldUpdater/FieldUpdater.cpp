#include "FieldUpdater.h"

#include <cstdint>
#include <limits>

#include "conway.h"
#include "DirectDraw.h"

enum class FieldState: std::uint8_t {
    Blank,
    Filled,
    Ambiguous,
};

FieldState calcFieldState(CellField& field) {
    uint16_t blankColumnCount = 0;
    uint16_t fullColumnCount = 0;
    uint16_t ambiguousColumnCount = 0;
    for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
        const uint32_t columnState = field.getColumnState_Next(c);
        if (columnState == 0) {
            blankColumnCount++;
        } else if (columnState == std::numeric_limits<uint32_t>::max()) {
            fullColumnCount++;
        } else {
            ambiguousColumnCount++;
        }

        if (ambiguousColumnCount > 0) {
            return FieldState::Ambiguous;
        }
    }

    const bool isFieldBlank = blankColumnCount == MAX_COLUMNS;
    const bool isFieldFilled = fullColumnCount == MAX_COLUMNS;

    if (isFieldBlank) {
        return FieldState::Blank;
    } else if (isFieldFilled) {
        return FieldState::Filled;
    } else {
        // I don't think this will ever be hit, but I'm leaving it in because:
        //  1. just in case i'm wrong
        //  2. to keep the linter happy
        return FieldState::Ambiguous;
    }
}

// The current code for drawing the cells that have changed. It seems to work pretty well (as long as we're not wasting
// time drawing a grid), but could probably be made better by only iterating over the cells that have changed.
void FieldUpdater::drawAndUpdate() {
    switch (calcFieldState(field)) {
    case FieldState::Blank:
        bitmapDraw.drawBlank(window);
        break;
    case FieldState::Filled:
        bitmapDraw.drawFull(window);
        break;
    case FieldState::Ambiguous:
        bitmapDraw.drawAll(window, field);
        break;
    }

    // Update
    for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
        field.setColumnState_Current(c, field.getColumnState_Next(c));
    }
}

FieldUpdater::FieldUpdater(const WindowPtr _window)
    : field{}, window(_window), bitmapDraw{} {
}

void FieldUpdater::toggleSingleCell(const uint16_t column, const uint16_t row) {
    // Update field
    field.toggleCell_Current(column, row);

    // Draw
    const static auto directDraw = DirectDraw(field);
    directDraw.drawCell(column, row);

    // Update generation
    const auto currentState = field.getCellState_Current(column, row);
    field.setCell_Next(column, row, currentState);
}

void FieldUpdater::nextGeneration() {
    // Update field
    field.createNextGeneration();

    // Draw
    // Update generation
    drawAndUpdate();
}

void FieldUpdater::randomResetField() {
    // Update field
    field.setRandomStart();

    // Draw
    // Update generation
    drawAndUpdate();
}

void FieldUpdater::clearField() {
    // Update field
    field.clearField();

    // Draw
    bitmapDraw.drawBlank(window);

    // Update generation
    // OPTIMIZE: add support for a bulk array copy from `next` to `current`
    for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
        field.setColumnState_Current(c, field.getColumnState_Next(c));
    }
}

void FieldUpdater::fillField() {
    // Update field
    field.fillField();

    // Draw
    bitmapDraw.drawFull(window);

    // Update generation
    // OPTIMIZE: add support for a bulk array copy from `next` to `current`
    for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
        field.setColumnState_Current(c, field.getColumnState_Next(c));
    }
}
