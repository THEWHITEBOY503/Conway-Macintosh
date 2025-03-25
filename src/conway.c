/* ***CONWAYS GAME OF LIFE IN C FOR THE MACINTOSH*** */
/* ***BY CONNER SMITH, JOSHUA PURUSHOTHAMAN, ARIAN BAISHYA, AND CHATGPT (BECAUSE CONNER DOESN'T KNOW HOW TO CODE)*** */
/* ***WRITTEN IN 2025 BY THREE COLLEGE STUDENTS WITH THE INTENT OF PRESENTING AT VCF SW 2025 :)*** */

/* Uses hacky tricks to make LSP happy when not running in the actual emulator */
#define NOT_ON_MAC

#ifdef NOT_ON_MAC
/* hackily deal with pascal keyword */
#define pascal extern

/* bruh */
#define true 1
#define false 0

#include "missing-things.h"

#else
#include "EventMgr.h"
#include "Quickdraw.h"
#include "WindowMgr.h"

#endif /* ifdef NOT_ON_MAC */

/* Define program-wide variables */
#define MAX_ROWS 32
#define MAX_COLUMNS 32
#define CELL_SIZE 8

/* These are for the rendering and not the program logic */
#define GRID_WIDTH (MAX_COLUMNS * CELL_SIZE)
#define GRID_HEIGHT (MAX_ROWS * CELL_SIZE)
#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 20

#define ALIVE 1
#define DEAD 0

/*
A word to the uninitated:

Field acts as a container for a 2D grid representation.

Field stores the size of the grid (rows and columns), the current state of the grid (current[MAX_COLUMNS]), and
the next state of the grid (next[MAX_COLUMNS]). We can access a field's properties with a pointer.

So, say I had MAX_COLUMNS set to 32 (as it is by default), which means I have [0]-[31].
If I wanted to refer to next at 7, I would use `field->next[7]`.
*/

/* This works as a sort of table -- As you see at the end, we call this 'table' "Field" */
typedef struct {
    /* Unsigned short can hold values from 0 to 65,535 */
    unsigned short rows; /* 2 bytes (16 bits)*/
    unsigned short columns;
    /* Unsigned long is a 32-bit signed value (This would be 64-bit on a modern system) */
    /* [MAX_COLUMNS] makes current an aray of the size of MAX_COLUMNS.
     * So, if MAX_COLUMNS is 32, then you would have current[0] through current[31]. */
    unsigned long current[MAX_COLUMNS]; /* 4 bytes (32 bits) * MAX_COLUMNS */
    unsigned long next[MAX_COLUMNS];

    /* FIXME: figure out what this is for? */
    unsigned long old[MAX_COLUMNS];
} Field;

/*
Boolean getCellState(unsigned long *arr, int x, int y) {
    return arr[x] & ((unsigned long)1 << y);
}
*/

/* Points to a window */
WindowPtr mainWindow;

/* Defines rectangles that the program needs to draw */
Rect stepButtonRect, resetButtonRect, blankButtonRect, fillButtonRect;
Field fieldMatrix;

/* FIXME: figure out what this is for? */
int running = 1;

/* New flag to always show grid */
int showGrid = 1;

static unsigned long rand_state = 123456789; /* RNG Seed */

/* Linear Congruential Generator (LCG) for generating random numbers */
unsigned rand_gen() {
    rand_state = rand_state * 1103515245 + 12345; /* Random bullshit happens here */
    return (rand_state / 65536) % 32768;
}

void setRandomStart(field) Field *field;
{
    int c, r;
    for (c = 0; c < field->columns; c++) {
        field->current[c] = 0; /* Empties out the entire column */
        /* Same as the above loop, but we're using r, and checking it against field->rows */
        for (r = 0; r < field->rows; r++) {
            if (rand_gen() % 2 == 0) { /* 50% chance of being true */
                field->next[c] |= ((unsigned long)1 << r);
                /* field->next is an unsigned long, so it's a 32-bit binary value.
                We start the entire column as 0, so that means it's now 00000000000000000000000000000000
                then, rand_gen() is run to return either a 1 or a 0 The % operator returns the remainder, so if we're
                dividing by 2, the remainder can only be either 1 (odd number) or 0 (even number). If the condition to
                make a cell alive is met (in our case it's 0, but it could just as easily be 1), the bit for that cell
                is set to 1. Let's say that we somehow manage to only have row 16 come alive. field->current[c] then
                becomes: 00000000000000001000000000000000 Because `|= ((unsigned long)1 << r` basically moves the
                'cursor' back r places, then sets the bit at its position to 1. */
            }
            /* Repeat for next row */
        }
        /* Repeat for next column */
    }
}

void drawCell(field, x, y) Field *field;
int x, y;
{
    /* Define a rectangle for the cell... */
    static Rect cellRect;

    /* ...then set parameters for it */
    cellRect.left = x * CELL_SIZE;              /* Left */
    cellRect.top = y * CELL_SIZE;               /* Top */
    cellRect.right = cellRect.left + CELL_SIZE; /* Right */
    cellRect.bottom = cellRect.top + CELL_SIZE; /* Bottom */
    if (field->next[x] &
        ((unsigned long)1 << y)) { /* Could this be turned into a case statement to optimize performance? */
        FillRect(&cellRect, black);
    } else if (showGrid) {
        /* showGrid is true but the conditions to fill in a cell (IE- alive) are not met, so we destroy the rectangle
         * and redraw it as blank */
        /* This is the old code for this, it sucks and is super slow, but it DOES at least draw a grid for us, which
        makes our lives easier. EraseRect(&cellRect); FrameRect(&cellRect);
        */
        FillRect(&cellRect, white); /* This is much faster, but doesn't draw in the grid.*/
    } else {
        /* This only fires if the showgrid flag is set to 1. As of right now, the grid doesn't show either way, so this
         * is irrelevant*/
        EraseRect(&cellRect);
    }
}

/* Going to comment this out since it's no longer needed- replaced in lieu of the update function
void drawField(field)
Field *field;
{
        int x, y;
        for (x = 0; x < field->columns; x++) {
                for (y = 0; y < field->rows; y++) {
                        drawCell(field, x, y);
                }
        }
}
*/

void clearField(field) Field *field;
{
    int c;
    for (c = 0; c < field->columns; c++) {
        field->next[c] = 0; /* Clear all cells - sets all iterations of field->current to 0*/
    }
}

void fillField(field) Field *field;
{
    int c, r;
    for (c = 0; c < field->columns; c++) {
        for (r = 0; r < field->rows; r++) {
            field->next[c] |= ((unsigned long)1 << r);
        }
    }
}

void createNextGeneration(
    field) /* This is the function that decides which cells in the next iteration are alive/dead. */
    Field *field;
{
    int c, r;
    for (c = 0; c < field->columns; c++) {
        unsigned long rowNext = 0;          /* Another 32-bit-long value */
        for (r = 0; r < field->rows; r++) { /* Basically checking "Is r lower than (the number of rows)?" */
            int currentState =
                (field->current[c] & ((unsigned long)1 << r)) !=
                0; /* Remember that the << operator shifts the bit it's operating on from right to left r times. */
            int neighbors = 0; /* Creates and defines our variable that will tell us how many alive neighbors we have */
            int x, y;
            /* We need to be checking in a 3x3 grid for a cells neighbors */
            for (x = c - 1; x <= c + 1; x++) { /* x is created with the result of c-1, then each time the loop iterates
                                                  it checks if its greater than or equal to the result of c+1. So, if
                                                  we're in column 5, it's going to check columns 3, 4 and 5. */
                for (y = r - 1; y <= r + 1; y++) { /* Same deal as above but with rows */
                    if (x == c && y == r)
                        continue; /* We don't count the cell that we're checking so we skip it. You can't be your own
                                     neighbor. */
                    /* These two lines make sure x and y are within bounds */
                    if (x >= 0 && x < field->columns && y >= 0 && y < field->rows &&
                        /* Check if the cell from column x at position y is alive */
                        (field->current[x] & ((unsigned long)1 << y))) {
                        /* If it is, increase neighbors by 1 */
                        neighbors++;
                    }
                }
            }
            if (currentState) {
                /* If the cell is alive (currentState == 1), we check if it has 2 or 3 alive neighbors. If it does, it
                 * stays alive (inserts a 1 into its position in the row). */
                if (neighbors == 2 || neighbors == 3)
                    rowNext |= (unsigned long)1 << r;
            } else {
                /* If the cell is dead (currentState == 0), we check if it has 3 alive neighbors, and if it does it
                 * comes alive (a 1 is inserted in its postion in the row)*/
                if (neighbors == 3)
                    rowNext |= (unsigned long)1 << r;
            }
        }
        /* Commit the results of the above function into the next iteration */
        field->next[c] = rowNext;
    }
}

/* The current code for drawing the cells that have changed. It seems to work pretty well (as long as we're not wasting
 * time drawing a grid), but could probably be made better by only iterating over the cells that have changed. */
void update(field) Field *field;
{
    unsigned long previousColumn;
    short c, r;
    for (c = 0; c < field->columns; c++) {
        previousColumn = field->current[c];
        field->current[c] = field->next[c];
        for (r = 0; r < field->rows; r++) {
            unsigned long prevCellState = (previousColumn & ((unsigned long)1 << r));
            unsigned long currentCellState = (field->current[c] & ((unsigned long)1 << r));
            if (prevCellState != currentCellState) {
                drawCell(field, c, r);
            }
        }
    }
}

/* QuickDraw shenanigans. This draws buttons, just like the name implies. I don't think I need to explain what this does
 * as anyone with half a brain can figure it out. */
void drawButton(buttonRect, label, highlighted) Rect *buttonRect;
char *label;
int highlighted;
{
    PenSize(2, 2);
    EraseRect(buttonRect);

    if (highlighted) {
        FillRect(buttonRect, black);
        PenPat(white);
        MoveTo(buttonRect->left + 10, buttonRect->top + 15);
        TextMode(srcBic); /* Invert text on black background */
        DrawString(label);
        TextMode(srcOr); /* Reset text mode */
    } else {
        FrameRect(buttonRect);
        MoveTo(buttonRect->left + 10, buttonRect->top + 15);
        DrawString(label);
    }

    /* Ensure border is always visible */
    PenPat(black);
    FrameRect(buttonRect);

    /* Reset PenSize to default (1, 1) */
    PenSize(1, 1);
}

void init_window() {
    Rect windowRect;

    InitGraf(&thePort);
    InitFonts();
    InitWindows();
    InitCursor();
    SetRect(&windowRect, /* Setting parameters for the window size and shit */
            50, 50, 50 + GRID_WIDTH + 120, 50 + GRID_HEIGHT + 40);
    mainWindow = NewWindow(0L, &windowRect, "\pConway's Game of Life", true, /* Sets window title */
                           documentProc, (WindowPtr)-1L, true, 0);
    SetPort(mainWindow);
    SetRect(&stepButtonRect, /* Sets the cordinates for the buttons (this one is for the step button), it adds 10 to the
                                width of the grid so that it's 10 to the right. */
            GRID_WIDTH + 10, 20, GRID_WIDTH + BUTTON_WIDTH + 10, 40);

    SetRect(&resetButtonRect, GRID_WIDTH + 10, 60, GRID_WIDTH + BUTTON_WIDTH + 10, 80);
    SetRect(&blankButtonRect, GRID_WIDTH + 10, 100, GRID_WIDTH + BUTTON_WIDTH + 10, 120);
    SetRect(&fillButtonRect, GRID_WIDTH + 10, 140, GRID_WIDTH + BUTTON_WIDTH + 10, 160);
}

void handleClick(where) Point where;
{
    GlobalToLocal(&where); /* QuickDraw function to convert global screen coordinates to local window coordinates. I
                              don't know what that means, honestly. */

    if (PtInRect(where, &stepButtonRect)) { /* "If Pointer In Rectangle -- IE a button "*/
        drawButton(&stepButtonRect, "\pStep", 1);
        createNextGeneration(&fieldMatrix);
        update(&fieldMatrix);
        drawButton(&stepButtonRect, "\pStep", 0);
    } else if (PtInRect(where, &resetButtonRect)) {
        drawButton(&resetButtonRect, "\pReset", 1);
        clearField(&fieldMatrix);
        setRandomStart(&fieldMatrix);
        update(&fieldMatrix);
        drawButton(&resetButtonRect, "\pReset", 0);
    } else if (PtInRect(where, &blankButtonRect)) {
        drawButton(&blankButtonRect, "\pBlank", 1);
        clearField(&fieldMatrix);
        update(&fieldMatrix);
        drawButton(&blankButtonRect, "\pBlank", 0);
    } else if (PtInRect(where, &fillButtonRect)) {
        drawButton(&fillButtonRect, "\pFill", 1);
        fillField(&fieldMatrix);
        update(&fieldMatrix);
        drawButton(&fillButtonRect, "\pFill", 0);
    } else {
        int x = where.h / CELL_SIZE;
        int y = where.v / CELL_SIZE;
        if (x >= 0 && x < MAX_COLUMNS && y >= 0 && y < MAX_ROWS) {
            fieldMatrix.current[x] ^= ((unsigned long)1 << y);
            fieldMatrix.next[x] = fieldMatrix.current[x];
            drawCell(&fieldMatrix, x, y);
        }
    }
}

int main() /* In the beginning, there was main(). This is where the program starts. */
{
    EventRecord event;
    WindowPtr whichWindow;
    int done = 0;
    int windowPart;
    Point where;
    init_window();
    fieldMatrix.rows = MAX_ROWS;
    fieldMatrix.columns = MAX_COLUMNS;
    setRandomStart(&fieldMatrix);
    update(&fieldMatrix);
    drawButton(&stepButtonRect, "\pStep", 0); /* If I need to explain to you what this line does, I'll cry. */
    drawButton(&resetButtonRect, "\pReset", 0);
    drawButton(&blankButtonRect, "\pBlank", 0);
    drawButton(&fillButtonRect, "\pFill", 0);
    while (!done) {
        if (GetNextEvent(everyEvent, &event)) {
            switch (event.what) {
            case mouseDown:
                windowPart = FindWindow(event.where, &whichWindow);
                switch (windowPart) {
                case inContent:
                    if (whichWindow == mainWindow)
                        handleClick(event.where);
                    break;
                case inDrag:
                    DragWindow(whichWindow, event.where, &thePort->portRect);
                    break;
                case inGoAway:
                    done = TrackGoAway(whichWindow, event.where);
                    break;
                }
                break;
            case keyDown: /* Handle button presses */
                done = 1;
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
