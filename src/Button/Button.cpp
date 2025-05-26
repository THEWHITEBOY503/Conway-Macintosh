#include "Button.h"

namespace MyGUI {

const QDGlobals *Button::qd = nullptr;

Button::Button(const unsigned char *label,
               const short left,
               const short top,
               const short right,
               const short bottom):
    _label(label),
    buttonRect{.top = top, .left = left, .bottom = bottom, .right = right} {
}

void Button::setQuickdrawGlobal(const QDGlobals *qd) {
    Button::qd = qd;
}

bool Button::isPointInRect(const Point point) const {
    return PtInRect(point, &buttonRect);
}

void Button::draw(const bool isHighlighted) const {
    const auto *const BLACK = &(qd->black);
    const auto *const WHITE = &(qd->white);

    PenSize(2, 2);
    EraseRect(&buttonRect);

    if (isHighlighted) {
        FillRect(&buttonRect, BLACK);
        PenPat(WHITE);

        MoveTo(static_cast<short>(buttonRect.left + 10), static_cast<short>(buttonRect.top + 15));
        TextMode(srcBic); /* Invert text on black background */
        DrawString(_label);
        TextMode(srcOr); /* Reset text mode */
    } else {
        FrameRect(&buttonRect);

        MoveTo(static_cast<short>(buttonRect.left + 10), static_cast<short>(buttonRect.top + 15));
        DrawString(_label);
    }

    /* Ensure border is always visible */
    PenPat(BLACK);
    FrameRect(&buttonRect);

    /* Reset PenSize to default (1, 1) */
    PenSize(1, 1);
}
}
