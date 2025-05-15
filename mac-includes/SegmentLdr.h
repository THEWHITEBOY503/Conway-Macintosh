
/*
 *  SegmentLdr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985.
 *
 */

#ifndef	_SegmentLdr_
#define _SegmentLdr_

#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

enum { appOpen, appPrint };

typedef	struct
	{
	int		vRefNum;
	OSType	fType;
	int		versNum;
	Str255	fName;
	} AppFile ;
	
/*  low-memory globals  */
extern char FinderName[] : 0x2E0;
extern int CurApRefNum : 0x900;
extern char CurApName[] : 0x910;
extern int CurJTOffset : 0x934;
extern int CurPageOption : 0x936;
extern Handle AppParmHandle : 0xAEC;


#endif