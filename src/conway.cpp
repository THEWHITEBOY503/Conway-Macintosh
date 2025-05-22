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

namespace MyGlobals {
    // These are for the rendering and not the program logic
    constexpr uint16_t GRID_WIDTH = (MAX_COLUMNS * CELL_SIZE);
    constexpr uint16_t GRID_HEIGHT = (MAX_ROWS * CELL_SIZE);

    // Flag to always show grid
    constexpr bool SHOW_GRID = true;
}

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

inline Rect getRect(const uint16_t x, const uint16_t y) {
    if constexpr (MyGlobals::SHOW_GRID) {
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
    } else {
        const auto rectLeft = static_cast<short>(x * CELL_SIZE);
        const auto rectTop = static_cast<short>(y * CELL_SIZE);

        /* Define a rectangle for the cell... */
        return Rect{
            .top = rectTop,
            .left = rectLeft,
            .bottom = static_cast<short>(rectTop + CELL_SIZE),
            .right = static_cast<short>(rectLeft + CELL_SIZE),
        };
    }
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
void initBlankCellGrid() {
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

// The current code for drawing the cells that have changed. It seems to work pretty well (as long as we're not wasting
// time drawing a grid), but could probably be made better by only iterating over the cells that have changed.
void update(WindowPtr window) {
    const auto drawAndUpdate = [] {
        for (uint16_t c = 0; c < MAX_COLUMNS; c++) {
            for (uint16_t r = 0; r < MAX_ROWS; r++) {
                drawCell(c, r);
            }
            fieldMatrix.setColumnState_Current(c, fieldMatrix.getColumnState_Next(c));
        }
    };

    drawAndUpdate();
}

void updateSingleCell(const uint16_t column, const uint16_t row) {
    drawCell(column, row);

    fieldMatrix.setColumnState_Next(column, fieldMatrix.getColumnState_Current(column));
}

WindowPtr init_window() {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitCursor();
    // Setting parameters for the window size and shit
    constexpr Rect windowRect = {
        .top = 50,
        .left = 50,
        .bottom = 50 + MyGlobals::GRID_HEIGHT + 40,
        .right = 50 + MyGlobals::GRID_WIDTH + 120
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

void handleClick(Point where, WindowPtr window) {
    using namespace Buttons;

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
            // fieldMatrix.current[x] ^= ((unsigned long)1 << y);
            fieldMatrix.toggleCell_Current(column, row);

            updateSingleCell(column, row);
        }
    }
}

/* In the beginning, there was main(). This is where the program starts. */
int main() {
    const WindowPtr mainWindow = init_window();

    if constexpr (MyGlobals::SHOW_GRID) {
        initBlankCellGrid();
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
                }
                break;
            case keyDown: /* Handle button presses */
                done = true;
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
