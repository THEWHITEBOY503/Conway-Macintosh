
/*
 *  VRetraceMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985.
 *
 */

#ifndef _VRetraceMgr_
#define _VRetraceMgr_

#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

typedef	struct VBLTask
	{
	struct QElem *	qLink;
 	int				qType;
	ProcPtr			vblAddr;
	int				vblCount;
	int				vblPhase;
	} VBLTask , *VBLQElPtr;

#define inVBL	0x40


/*  functions returning non-integral values  */
pascal struct QHdr *GetVBLQHdr();

/*  low-memory globals  */
extern struct QHdr VBLQueue : 0x160;


#endif