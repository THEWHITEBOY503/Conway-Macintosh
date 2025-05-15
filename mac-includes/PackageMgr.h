
/*
 *  PackageMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985-86.
 *
 */

#ifndef	_PackageMgr_
#define _PackageMgr_

#ifndef _MacTypes_
#include "MacTypes.h"
#endif

/* package IDs */
enum {
	listMgr,
	dskInit = 2,
	stdFile,
	flPoint,
	trFunc,
	intUtil,
	bdConv
};

/*  low-memory globals  */
extern Handle AppPacks[] : 0xAB8;


#endif


