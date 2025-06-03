#pragma once

#include "BitMatrix/BitMatrix.h"

template <uint16_t length>
class SwappableBuffers {
private:
    using buffer_t = BitMatrix<uint32_t, length, length>;

    // Backing storage, don't touch
    buffer_t first{};
    buffer_t second{};

    buffer_t* current = &first;
    buffer_t* next = &second;

public:
    [[nodiscard]] buffer_t& getCurrent() const {
        return *current;
    }

    [[nodiscard]] buffer_t& getNext() const {
        return *next;
    }

    void swap() {
        buffer_t* swap = current;
        current = next;
        next = swap;
    }
};

/*
A word to the uninitiated:

Field acts as a container for a 2D grid representation.

Field stores the size of the grid (rows and columns), the current state of the grid (current[MAX_COLUMNS]), and
the next state of the grid (next[MAX_COLUMNS]). We can access a field's properties with a pointer.

So, say I had MAX_COLUMNS set to 32 (as it is by default), which means I have [0]-[31].
If I wanted to refer to next at 7, I would use `field->next[7]`.
*/

/* This works as a sort of table -- As you see at the end, we call this 'table' "Field" */

// Unsigned 16-bit data type can hold values from 0 to 65,535
template <uint16_t rowCount, uint16_t columnCount>
class Field {
    using CellRow = uint32_t;
    using CellColumn = uint32_t;
    using CellState = bool;
    using CellIndex = uint16_t;

private:
    SwappableBuffers<columnCount> buffers{};

    /****** getters and setters for `next` generation ******/
    [[nodiscard]] bool getCellState_Next(const uint16_t column, const uint16_t row) const;

    void setColumnState_Next(const uint16_t column, const uint32_t value);

public:
    [[nodiscard]] bool cellHasChangedSinceLastGeneration(const uint16_t column, const uint16_t row) const;

    /****** getters and setters for `current` generation ******/
    [[nodiscard]] bool getCellState_Current(const uint16_t column, const uint16_t row) const;

    void setCell_Current(const uint16_t column, const uint16_t row, const bool value);

    void toggleCell_Current(const uint16_t column, const uint16_t row);

    [[nodiscard]] uint32_t getColumnState_Current(const uint16_t column) const;

    void setColumnState_Current(const uint16_t column, const uint32_t value);

    void setColumn_Current(const uint16_t column, const bool value);

    void setRow_Current(const uint16_t column, const bool value);

    void setField_Current(const bool value);

    /****** end of generic getters and setters ******/

    void setRandomStart();

    void clearField();

    void fillField();

    // This is the function that decides which cells in the next iteration are alive/dead.
    void createNextGeneration();
};

#include "conway.h"

using CellField = Field<MAX_ROWS, MAX_COLUMNS>;
