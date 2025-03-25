#include "../mac-includes/EventMgr.h"
#include "../mac-includes/Quickdraw.h"
#include "../mac-includes/WindowMgr.h"

Boolean TrackGoAway(WindowPtr theWindow, Point thePt);
int FindWindow(Point thePoint, WindowPtr *theWindow);
void DragWindow(WindowPtr theWindow, Point startPt, Rect *boundsRect);

Boolean GetNextEvent(int mask, EventRecord *theEvent);

Boolean PtInRect(Point pt, Rect *r);

void InitFonts();
void InitWindows();
void InitCursor();

/*
 GrafPort Routines
 */
void InitGraf(Ptr globalPtr);
void OpenPort(GrafPtr port);
void InitPort(GrafPtr port);
void ClosePort(GrafPtr port);
void SetPort(GrafPtr port);
void GetPort(GrafPtr *port);
void GrafDevice(int device);
void SetPortBits(BitMap *bm);
void PortSize(int width, int height);
void MovePortTo(int leftGlobal, int topGlobal);

void TextFace(Style face);
void TextMode(int mode);
void TextSize(int size);
void SpaceExtra(Fixed extra);
void DrawChar(char ch);
void DrawString(Str255 s);
void DrawText(Ptr textBuf, int firstByte, int byteCount);

void LocalToGloba(Point *pt);
void GlobalToLocal(Point *pt);

/* Rectangle Calculations
 */
void SetRect(Rect *r, int left, int top, int right, int bottom);
void FillRect(Rect *r, Pattern pat);
void FrameRect(Rect *r);
void EraseRect(Rect *r);

void PenSize(int width, int height);
void PenMode(int mode);
void PenPat(Pattern pat);
void PenNormal();
void MoveTo(int h, int v);
void Move(int dh, int dv);
void LineTo(int h, int v);
void Line(int dh, int dv);
