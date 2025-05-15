
/*
 *  ToolboxUtil.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985-86.
 *
 */

#ifndef	_ToolboxUtil_
#define _ToolboxUtil_
	
#ifndef	_Quickdraw_
#include "Quickdraw.h"
#endif

#define	sysPatListID	0

enum {
	iBeamCursor = 1,
	crossCursor,
	plusCursor,
	watchCursor
};

typedef	struct	
	{
	long	hiLong;
	long	loLong;
	} Int64Bit ;


/*  functions returning non-integral values  */
pascal StringHandle NewString();
pascal StringHandle GetString();
pascal Handle GetIcon();
pascal PatHandle GetPattern();
pascal CursHandle GetCursor();
pascal PicHandle GetPicture();
double Fix2X(Fixed);
double Frac2X(Fract);


#endif
