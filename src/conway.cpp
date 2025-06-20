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
#include <TextEdit.h>
#include <Devices.h>
#include <Resources.h>
#include <string>

#include "conway.h"
#include "Button.h"
#include "FieldUpdater.h"

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
    MyGUI::Button demoButton("\pDemo", ALL_BUTTONS_LEFT_X_COORDINATE, 180, ALL_BUTTONS_RIGHT_X_COORDINATE, 200);
}

bool inDemoMode = false;

enum MenuItem: short { // NOLINT(*-enum-size)
    kMenuApple = 128,
    kMenuFile,
    kMenuEdit
};

enum MenuItemType: short { // NOLINT(*-enum-size)
    kItemAbout = 1,

    kItemNewDoc = 1,
    kItemNewRounded = 2,
    kItemNewCustomFromStub = 3,
    kItemNewCustomFromRes = 4,
    kItemClose = 5,
    kItemQuit = 7
};

// The qd global has been removed from the libraries, so we have to make our own
QDGlobals qd;

void MyInitMenus() {
    InitMenus();

    const MenuBarHandle menuBar = GetNewMBar(128);
    SetMenuBar(menuBar);
    DisposeHandle(menuBar);

    DrawMenuBar();
}

WindowPtr init_window() {
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    MyInitMenus();
    TEInit();
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

inline void handleClick(Point where, FieldUpdater& fieldUpdater) {
    using namespace Buttons;

    // QuickDraw function to convert global screen coordinates to local window coordinates.
    // I don't know what that means, honestly.
    GlobalToLocal(&where);

    /* "If Pointer In Rectangle -- IE a button "*/
    if (stepButton.isPointInRect(where)) {
        stepButton.draw(true);
        fieldUpdater.nextGeneration();
        stepButton.draw(false);
    } else if (resetButton.isPointInRect(where)) {
        resetButton.draw(true);
        fieldUpdater.randomResetField();
        resetButton.draw(false);
    } else if (blankButton.isPointInRect(where)) {
        blankButton.draw(true);
        fieldUpdater.clearField();
        blankButton.draw(false);
    } else if (fillButton.isPointInRect(where)) {
        fillButton.draw(true);
        fieldUpdater.fillField();
        fillButton.draw(false);
    } else if (demoButton.isPointInRect(where)) {
        demoButton.draw(true);
        fieldUpdater.clearField();
        fieldUpdater.randomResetField();
        inDemoMode = !inDemoMode;
        demoButton.draw(false);
    } else {
        const int column = where.h / CELL_SIZE; // x coordinate
        const int row = where.v / CELL_SIZE; // y coordinate
        if (column >= 0 && column < MAX_COLUMNS && row >= 0 && row < MAX_ROWS) {
            fieldUpdater.toggleSingleCell(column, row);
        }
    }
}

void showAboutBox() {
    WindowRef previousPort;
    GetPort(&previousPort);

    const WindowRef w = GetNewWindow(128, nullptr, (WindowPtr)-1);
    MoveWindow(w,
               (qd.screenBits.bounds.right / 2) - (w->portRect.right / 2),
               (qd.screenBits.bounds.bottom / 2) - (w->portRect.bottom / 2),
               false);
    ShowWindow(w);
    SetPort(w);

    {
        // Create box rect
        Rect r = w->portRect;
        InsetRect(&r, 10, 10);

        // Get + Lock resource
        Handle h = GetResource('TEXT', 128);
        HLock(h);

        // Use resource
        TETextBox(*h, GetHandleSize(h), &r, teJustLeft);

        // Release resource
        ReleaseResource(h);
    }

    // I believe this will wait until a mouse button has gone up, then wait
    // until it has gone down, before continuing (and closing the window).
    //
    // If I'm correct, then it basically spins until the next mouse click.
    while (Button() == 0) {
    }
    while (Button() != 0) {
    }

    FlushEvents(everyEvent, 0);

    DisposeWindow(w);

    SetPort(previousPort);
}

void handleMenuCommand(const long menuResult) {
    const auto menuID = static_cast<short>(menuResult >> 16);
    const auto menuItem = static_cast<short>(menuResult & 0xFFFF);

    Str255 str;
    if (menuID == kMenuApple) {
        if (menuItem == kItemAbout) {
            showAboutBox();
        } else {
            GetMenuItemText(GetMenu(128), menuItem, str);
            OpenDeskAcc(str);
        }
    }

    HiliteMenu(0);
}

void drawButtons() {
    using namespace Buttons;
    stepButton.draw(false);
    resetButton.draw(false);
    blankButton.draw(false);
    fillButton.draw(false);
    demoButton.draw(false);
}

void updateScreen(const WindowRef mainWindow, const FieldUpdater& fieldUpdater) {
    BeginUpdate(mainWindow);

    fieldUpdater.redraw();

    drawButtons();

    EndUpdate(mainWindow);
}

/* In the beginning, there was main(). This is where the program starts. */
int main() {
    const WindowPtr mainWindow = init_window();

    FieldUpdater fieldUpdater{mainWindow};

    // Init
    fieldUpdater.clearField();

    fieldUpdater.randomResetField();

    drawButtons();

    int demoModeStepCounter = 0;
    bool done = false;
    while (!done) {
        if (inDemoMode) {
            constexpr u_long numSeconds = 1;
            constexpr u_long numTicks = numSeconds * 60;

            if (demoModeStepCounter >= 30) {
                demoModeStepCounter = 0;
                fieldUpdater.randomResetField();
            } else {
                demoModeStepCounter++;
                fieldUpdater.nextGeneration();

                // TODO:
                //  improve this so user input isn't thoroughly slowed
                //  down by this delay.
                Delay(numTicks, nullptr);
            }
        }

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
                        handleClick(event.where, fieldUpdater);
                    break;
                case inDrag:
                    DragWindow(whichWindow, event.where, &qd.thePort->portRect);
                    break;
                case inGoAway:
                    done = TrackGoAway(whichWindow, event.where);
                    break;
                case inMenuBar:
                    // UpdateMenus();
                    handleMenuCommand(MenuSelect(event.where));
                    updateScreen(mainWindow, fieldUpdater);
                    break;
                default:
                    // ignore other events
                    break;
                }
                break;
            case keyDown: /* Handle button presses */
                // TODO: This was here when I got here, but I'm not sure closing
                //  the game is the right response to any key presses lol.
                done = true;
                break;
            default:
                // ignore other events
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
