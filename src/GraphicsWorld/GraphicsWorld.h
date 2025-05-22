#pragma once

#include <Quickdraw.h>
#include <QDOffscreen.h>
#include <MacWindows.h>

class GraphicsWorld {
private:
    GWorldPtr offscreenGWorld = nullptr;

    static Rect getPortImageRect(const Rect &rect);

    void copyWorld(const WindowPtr windowPtr) const;

public:
    explicit GraphicsWorld(WindowPtr windowPtr);

    template <typename Functor>
    int draw(const WindowPtr windowPtr, Functor &&functor);

    ~GraphicsWorld();
};

template <typename Functor>
int GraphicsWorld::draw(const WindowPtr windowPtr, Functor &&functor) {
    // save window's graphics port
    CGrafPtr origPort = nullptr;
    GDHandle origDev = nullptr;
    GetGWorld(&origPort, &origDev);

    const QDErr err = NewGWorld(
        &(this->offscreenGWorld),
        1,
        &(windowPtr->portRect),
        nullptr,
        nullptr,
        0
        );

    if (err != 0) {
        SetWTitle(windowPtr, "\perror occurred");
        return err;
    } else if (this->offscreenGWorld == nullptr) {
        // BRUH I THINK THIS IS BC I'M ON SYSTEM 6 AND OFFSCREEN GRAPHICS WORLDS WERE ADDED IN 7 ???!!?!?!?!?
        SetWTitle(windowPtr, "\pnullptr without err :(");
        return -1;
    }

    // Make offscreen graphics world the current port
    SetGWorld(this->offscreenGWorld, nullptr);

    // Get handle to offscreen pixel image, then lock it
    PixMapHandle offPixMapHandle = GetGWorldPixMap(this->offscreenGWorld);
    const Boolean good = LockPixels(offPixMapHandle);
    if (good) {
        // draw things
        EraseRect(&(this->offscreenGWorld->portRect)); /*initialize its pixel image*/

        // Actual drawing
        functor();

        // restore window's graphics port (make window the current port)
        SetGWorld(origPort, origDev);

        this->copyWorld(windowPtr);

        UnlockPixels(offPixMapHandle);
    }

    DisposeGWorld(this->offscreenGWorld);
    this->offscreenGWorld = nullptr;

    return 0;
}
