
/*
 *  MenuMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985.
 *
 */

#ifndef	_MenuMgr_
#define _MenuMgr_
	
#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

#define maxItem		31

/* special chars */
enum {
	noMark,
	commandMark = 0x11,
	checkMark,
	diamondMark,
	appleMark
};

/*  menu defproc messages  */
enum { mDrawMsg, mChooseMsg, mSizeMsg };

/*  MDEF id  */
#define textMenuProc	0

typedef struct
	{
	int		menuID;
	int		menuWidth;
	int		menuHeight;
	Handle 	menuProc;
	long	enableFlags; 
	Str255	menuData;
	} MenuInfo,* MenuPtr, ** MenuHandle;


/*  functions returning non-integral values  */
pascal MenuHandle NewMenu();
pascal MenuHandle GetMenu();
pascal Handle GetNewMBar();
pascal Handle GetMenuBar();
pascal MenuHandle GetMHandle();

/*  low-memory globals  */
extern Handle MenuList : 0xA1C;
extern int MBarEnable : 0xA20;
extern int MenuFlash : 0xA24;
extern int TheMenu : 0xA26;
extern ProcPtr MBarHook : 0xA2C;
extern ProcPtr MenuHook : 0xA30;


#endif
