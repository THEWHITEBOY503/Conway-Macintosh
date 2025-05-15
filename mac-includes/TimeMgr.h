
/*
 *  TimeMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1986.
 *
 */

#ifndef	_TimeMgr_
#define _TimeMgr_

#ifndef	_MacTypes_
#include "MacTypes.h"
#endif


typedef struct TMTask {
	struct QElem		*qLink;
	int					qType;
	ProcPtr				tmAddr;
	int					tmCount;
} TMTask;


#endif