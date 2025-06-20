#include "Menus.r"
#include "MacWindows.r"
#include "Processes.r"

#define mApple 128

resource 'MENU' (mApple, preload) { // resource ID, preload resource
    mApple, textMenuProc, // uses standard menu definition procedure
    allEnabled,
    enabled, // enable menu title
    apple, // menu title
    {
        // first menu item
        "About Conway's Game of Life...", // text of menu item
            // item characteristics
            noicon, // icon number (if any) or script code (if any)
            nokey, // keyboard equivalent (if any)
            nomark, // marking character (if any) or menu ID of submenu (if any)
            plain; // style of menu item text

        // second menu item
        "-", // item text (divider)
            // item characteristics
            noicon,
            nokey,
            nomark,
            plain
    }
};

resource 'MBAR' (mApple) {
    { mApple };
};

resource 'WIND' (128) {
    {0, 0, 220, 320},
    dBoxProc;
    invisible;
    goAway;
    0,
    "About Conway's Game of Life";
    centerMainScreen;
};

data 'TEXT' (128) {
    "Port of Conway's Game of Life for old Macs\r\r"
    "YouTube: YouTube.com/@ConnahComputerSmith\r"
    "Source Code: github.com/THEWHITEBOY503/Conway-Macintosh\r\r"
    "Credits: Conner Smith and Arian Baishya"
};

// TODO: figure out if these are correct
#define minSizeKB 30
#define prefSizeKB 50

resource 'SIZE' (-1) {
	reserved,
    acceptSuspendResumeEvents, // accepts suspend & resume events
	reserved,
    canBackground, // can use background null events
    doesActivateOnFGSwitch, // activates own windows in response to OS events
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreChildDiedEvents,
	is32BitCompatible,
	isHighLevelEventAware,
	onlyLocalHLEvents,
	notStationeryAware,
	dontUseTextEditServices,
	reserved,
	reserved,
	reserved,
    prefSizeKB * 1024, // preferred memory size
    minSizeKB * 1024 // minimum memory size
};
