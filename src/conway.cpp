/* ***CONWAYS GAME OF LIFE IN C FOR THE MACINTOSH*** */
/* ***BY CONNER SMITH, JOSHUA PURUSHOTHAMAN, ARIAN BAISHYA, AND CHATGPT (BECAUSE CONNER DOESN'T KNOW HOW TO CODE)*** */
/* ***WRITTEN IN 2025 BY THREE COLLEGE STUDENTS WITH THE INTENT OF PRESENTING AT VCF SW 2025 :)*** */

#include <cstdint>

// NOTE:
//  I wanted to split the `include`s into *multiple* namespaces, but long story short, putting
//  3rd party include files into namespaces doesn't really work unless you deal with a million
//  quirks that could all easily break it or just modify the headers.
#include <Quickdraw.h>
#include <Events.h>
#include <MacWindows.h>

#include "conway.h"
#include "Field.h"
#include "Button.h"
#include "MyBitMap.h"

namespace Buttons {
    namespace {
        constexpr uint8_t BUTTON_WIDTH = 80;
        constexpr uint8_t BUTTON_HEIGHT = 20;

        // Sets the coordinates and labels for the buttons (this one is for the step button).
        // It adds 10 to the width of the grid so that it's 10 to the right.
        constexpr uint16_t ALL_BUTTONS_LEFT_X_COORDINATE = MyGlobals::GRID_WIDTH + 10;
        constexpr uint16_t ALL_BUTTONS_RIGHT_X_COORDINATE = MyGlobals::GRID_WIDTH + BUTTON_WIDTH + 10;
    }

    MyGUI::Button stepButton("\pStep", ALL_BUTTONS_LEFT_X_COORDINATE, 20, ALL_BUTTONS_RIGHT_X_COORDINATE, 40);
    MyGUI::Button resetButton("\pReset", ALL_BUTTONS_LEFT_X_COORDINATE, 60, ALL_BUTTONS_RIGHT_X_COORDINATE, 80);
    MyGUI::Button blankButton("\pBlank", ALL_BUTTONS_LEFT_X_COORDINATE, 100, ALL_BUTTONS_RIGHT_X_COORDINATE, 120);
    MyGUI::Button fillButton("\pFill", ALL_BUTTONS_LEFT_X_COORDINATE, 140, ALL_BUTTONS_RIGHT_X_COORDINATE, 160);
}

// The qd global has been removed from the libraries, so we have to make our own
QDGlobals qd;

Field<MAX_ROWS, MAX_COLUMNS> fieldMatrix;

using GridBitMap = MyBitMap<
    MyGlobals::GRID_WIDTH,
    MyGlobals::GRID_HEIGHT
>;

namespace DirectDraw {
    inline Rect getRect(const uint16_t x, const uint16_t y) {
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

    void drawCell(const uint16_t x, const uint16_t y) {
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

    // TODO:
    //  the window itself has a 1px black border, which makes the top and left borders of the grid
    //  seem as if they're 2px wide, which looks a bit weird.
    //  Solution should be to shift *all* grid drawing 1px up and left
    void drawBlankCellGrid() {
        PenSize(1, 1);

        for (uint16_t c = 0; c <= MAX_COLUMNS; c++) {
            const int16_t columnX = c * CELL_SIZE;
            MoveTo(columnX, 0);
            LineTo(columnX, MyGlobals::GRID_HEIGHT);
        }
        for (uint16_t r = 0; r <= MAX_ROWS; r++) {
            const int16_t rowY = r * CELL_SIZE;
            MoveTo(0, rowY);
            LineTo(MyGlobals::GRID_WIDTH, rowY);
        }

        PenNormal();
    }
}

namespace BitMapDraw {
    class CachedQuickdrawOperationBitMap {
    private:
        GridBitMap bitmap; // NOTE: hardcoded bitmap template specialization

    public:
        template <typename Functor>
        explicit CachedQuickdrawOperationBitMap(const WindowPtr mainWindow, Functor&& draw) {
            bitmap.blank();

            draw();

            CopyBits(
                &mainWindow->portBits, &bitmap.getBits(),
                &MyGlobals::GRID_RECT, &bitmap.getBounds(), // NOTE: hardcoded reduced srcRect bounds
                srcCopy,
                nullptr
            );
        }

        void copyOnto(const GridBitMap& otherBitMap) {
            CopyBits(
                &bitmap.getBits(), &otherBitMap.getBits(),
                &bitmap.getBounds(), &otherBitMap.getBounds(),
                srcOr,
                nullptr
            );
        }
    };

    void drawCell(const GridBitMap& bitmap, const uint16_t x, const uint16_t y) {
        const bool prevCellState = fieldMatrix.getCellState_Current(x, y);
        const bool currentCellState = fieldMatrix.getCellState_Next(x, y);
        if (prevCellState != currentCellState) {
            bitmap.drawSquare(x, y, CELL_SIZE);
        }
    }

    void drawBlankCellGrid(const GridBitMap& bitmap) {
        static bool firstCall = true;
        static GridBitMap blankGridBitMap{};
        if (firstCall) {
            firstCall = false;

            blankGridBitMap.blank();
            for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
                if (c == 0) continue;
                blankGridBitMap.drawVerticalGridLine(c, 0, MyGlobals::GRID_HEIGHT);
            }
            for (uint16_t r = 0; r < MAX_ROWS; r++) {
                if (r == 0) continue;
                blankGridBitMap.drawHorizontalGridLine(0, r, MyGlobals::GRID_WIDTH);
            }
        }

        // Use srcOr to basically "add" the grid to whatever is already on the bitmap (think of it like an overlay).
        // Allows us to pass in a partially erased bitmap and "repair" the grid.
        CopyBits(
            &blankGridBitMap.getBits(), &bitmap.getBits(),
            &blankGridBitMap.getBounds(), &bitmap.getBounds(),
            srcOr,
            nullptr
        );
    }

    void drawAll(const GridBitMap& bitmap, const WindowPtr mainWindow) {
        bitmap.blank();
        const auto draw = [&bitmap] {
            for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
                for (uint16_t r = 0; r < MAX_ROWS; r++) {
                    drawCell(bitmap, c, r);
                }
            }
        };

        draw();

        CopyBits(
            &bitmap.getBits(), &mainWindow->portBits,
            &bitmap.getBounds(), &MyGlobals::GRID_RECT,
            srcXor,
            nullptr
        );
    }

    void drawBlank(const GridBitMap& bitmap, const WindowPtr mainWindow) {
        bitmap.blank();
        drawBlankCellGrid(bitmap);
        CopyBits(
            &bitmap.getBits(), &mainWindow->portBits,
            &bitmap.getBounds(), &MyGlobals::GRID_RECT,
            srcCopy,
            nullptr
        );
    }

    void drawFull(const GridBitMap& bitmap, const WindowPtr mainWindow) {
        bitmap.fill();
        CopyBits(
            &bitmap.getBits(), &mainWindow->portBits,
            &bitmap.getBounds(), &MyGlobals::GRID_RECT,
            srcCopy,
            nullptr
        );
    }
}

namespace UpdateField {
    enum class FieldState: std::uint8_t {
        Blank,
        Filled,
        Ambiguous,
    };

    FieldState calcFieldState() {
        bool isFieldBlank = true;
        bool isFieldFilled = true;
        for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
            const uint32_t columnState = fieldMatrix.getColumnState_Next(c);
            if (isFieldBlank && columnState != 0) {
                isFieldBlank = false;
            }
            if (isFieldFilled && columnState != std::numeric_limits<uint32_t>::max()) {
                isFieldFilled = false;
            }

            if (!isFieldBlank && !isFieldFilled) {
                break;
            }
        }

        if (isFieldBlank) {
            return FieldState::Blank;
        } else if (isFieldFilled) {
            return FieldState::Filled;
        } else {
            return FieldState::Ambiguous;
        }
    }

    // The current code for drawing the cells that have changed. It seems to work pretty well (as long as we're not wasting
    // time drawing a grid), but could probably be made better by only iterating over the cells that have changed.
    void update(const WindowPtr mainWindow) {
        static GridBitMap bitmap{};

        switch (calcFieldState()) {
        case FieldState::Blank:
            BitMapDraw::drawBlank(bitmap, mainWindow);
            break;
        case FieldState::Filled:
            BitMapDraw::drawFull(bitmap, mainWindow);
            break;
        case FieldState::Ambiguous:
            BitMapDraw::drawAll(bitmap, mainWindow);
            break;
        }

        // Update
        for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
            fieldMatrix.setColumnState_Current(c, fieldMatrix.getColumnState_Next(c));
        }
    }

    void updateSingleCell(const uint16_t column, const uint16_t row) {
        DirectDraw::drawCell(column, row);

        // FIXME: should this be using CellState (instead of ColumnState) ?
        fieldMatrix.setColumnState_Next(column, fieldMatrix.getColumnState_Current(column));
    }
}

WindowPtr init_window() {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitCursor();

    // Setting parameters for the window size and shit
    using namespace MyGlobals::WindowDimensions;
    constexpr Rect windowRect = {
        .top = WINDOW_TOP,
        .left = WINDOW_LEFT,
        .bottom = WINDOW_TOP + WINDOW_HEIGHT,
        .right = WINDOW_LEFT + WINDOW_WIDTH,
    };
    const WindowPtr mainWindow = NewWindow(
        nullptr,
        &windowRect,
        "\pConway's Game of Life", // Sets window title
        true,
        documentProc,
        (WindowPtr)-1L,
        true,
        0
    );
    SetPort(mainWindow);

    MyGUI::Button::setQuickdrawGlobal(&qd);

    return mainWindow;
}

inline void handleClick(Point where, const WindowPtr window) {
    using namespace Buttons;
    using namespace UpdateField;

    // QuickDraw function to convert global screen coordinates to local window coordinates.
    // I don't know what that means, honestly.
    GlobalToLocal(&where);

    if (stepButton.isPointInRect(where)) {
        /* "If Pointer In Rectangle -- IE a button "*/
        stepButton.draw(true);
        fieldMatrix.createNextGeneration();
        update(window);
        stepButton.draw(false);
    } else if (resetButton.isPointInRect(where)) {
        resetButton.draw(true);
        fieldMatrix.clearField();
        fieldMatrix.setRandomStart();
        update(window);
        resetButton.draw(false);
    } else if (blankButton.isPointInRect(where)) {
        blankButton.draw(true);
        fieldMatrix.clearField();
        update(window);
        blankButton.draw(false);
    } else if (fillButton.isPointInRect(where)) {
        fillButton.draw(true);
        fieldMatrix.fillField();
        update(window);
        fillButton.draw(false);
    } else {
        const int column = where.h / CELL_SIZE; // x coordinate
        const int row = where.v / CELL_SIZE; // y coordinate
        if (column >= 0 && column < MAX_COLUMNS && row >= 0 && row < MAX_ROWS) {
            fieldMatrix.toggleCell_Current(column, row);

            updateSingleCell(column, row);
        }
    }
}

/* In the beginning, there was main(). This is where the program starts. */
int main() {
    using namespace UpdateField;

    const WindowPtr mainWindow = init_window();

    // TODO: decide on which branch I wanna use
    // Init
    if constexpr (true) {
        fieldMatrix.clearField();
        update(mainWindow);
    } else {
        DirectDraw::drawBlankCellGrid();
    }

    fieldMatrix.setRandomStart();
    update(mainWindow);

    using namespace Buttons;
    stepButton.draw(false);
    resetButton.draw(false);
    blankButton.draw(false);
    fillButton.draw(false);

    bool done = false;
    while (!done) {
        EventRecord event;
        if (GetNextEvent(everyEvent, &event) != 0) {
            int windowPart;
            switch (event.what) {
            case mouseDown:
                WindowPtr whichWindow;
                windowPart = FindWindow(event.where, &whichWindow);
                switch (windowPart) {
                case inContent:
                    if (whichWindow == mainWindow)
                        handleClick(event.where, mainWindow);
                    break;
                case inDrag:
                    DragWindow(whichWindow, event.where, &qd.thePort->portRect);
                    break;
                case inGoAway:
                    done = TrackGoAway(whichWindow, event.where);
                    break;
                default:
                    // TODO: what should we do here (if anything)?
                    break;
                }
                break;
            case keyDown: /* Handle button presses */
                // FIXME: This was here when I got here, but I'm not sure closing
                //  the game is the right response to any key presses lol.
                done = true;
                break;
            default:
                // TODO: what should we do here (if anything)?
                break;
            }
        }
    }

    return 0;
}

/* Congratulations on scrolling to the bottom of this file! If you thought that was disgusting, you should see my social
 * skills after having made this! Please write to me at connersoftdev@gmail.com if you have any questions, such as:
 * "What is wrong with you? What made you do this?" Etc. */
/* Please don't give me the credit for all this software, as I couldn't have done it without the help of my friends Josh
 * and Arian. In fact, they probably wrote more code than I did (ChatGPT code does not count). All I did was direct the
 * project, of sorts. Despite the name of my email, I'm actually a pretty terrible software developer, so make sure you
 * support REAL developers who spend their days tirelessly writing code by hand and manually debugging software. */
/* Thank you! */
