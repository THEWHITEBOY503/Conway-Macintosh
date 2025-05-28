#include "GraphicsWorld.h"

GraphicsWorld::GraphicsWorld(WindowPtr windowPtr) {
}

GraphicsWorld::~GraphicsWorld() {
}

Rect GraphicsWorld::getPortImageRect(const Rect& rect) {
    // for CopyBits, create rectangles that exclude scroll bar areas
    Rect windowRect = rect;
    windowRect.bottom = windowRect.bottom - 15;
    windowRect.right = windowRect.right - 15;

    return windowRect;
}

void GraphicsWorld::copyWorld(const WindowPtr windowPtr) const {
    const Rect sourceRect = getPortImageRect(this->offscreenGWorld->portRect);
    const Rect destRect = getPortImageRect(windowPtr->portRect);

    // next, use CopyBits to transfer the offscreen image to the window
    CopyBits(
        //coerce graphics world's PixMap to a BitMap
        &(reinterpret_cast<GrafPtr>(this->offscreenGWorld)->portBits),
        &(windowPtr->portBits),
        &sourceRect,
        &destRect,
        srcCopy,
        nullptr
    );
}
