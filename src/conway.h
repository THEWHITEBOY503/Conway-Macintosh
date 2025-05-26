#pragma once

constexpr bool ALIVE = true;
constexpr bool DEAD = false;

/* Define program-wide variables */
constexpr uint16_t MAX_ROWS = 32;
constexpr uint16_t MAX_COLUMNS = 32;
constexpr uint8_t CELL_SIZE = 8;

namespace MyGlobals {
    // These are for the rendering and not the program logic
    constexpr uint16_t GRID_WIDTH = (MAX_COLUMNS * CELL_SIZE);
    constexpr uint16_t GRID_HEIGHT = (MAX_ROWS * CELL_SIZE);

    // Window constants
    namespace WindowDimensions {
        constexpr short WINDOW_TOP = 50;
        constexpr short WINDOW_LEFT = 50;
        constexpr short WINDOW_HEIGHT = GRID_HEIGHT + 40;
        constexpr short WINDOW_WIDTH = GRID_WIDTH + 120;
    }

    constexpr Rect GRID_RECT = {
        .top = 0,
        .left = 0,
        .bottom = GRID_HEIGHT,
        .right = GRID_WIDTH,
    };
}
