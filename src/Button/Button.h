#pragma once

#include <Quickdraw.h>

namespace MyGUI {

class Button {
private:
    const static QDGlobals *qd;

public:
    const unsigned char *label;
    const Rect buttonRect;

    // public:
    explicit Button(const unsigned char *label,
                    const short left,
                    const short top,
                    const short right,
                    const short bottom);

    static void setQuickdrawGlobal(const QDGlobals *qd);

    [[nodiscard]] bool isPointInRect(const Point point) const;

    /**
     * QuickDraw shenanigans. This draws buttons, just like the name implies.
     * I don't think I need to explain what this does
     * as anyone with half a brain can figure it out.
     */
    void draw(const bool isHighlighted) const;
};
}

