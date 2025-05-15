
/*
 *  ScrapMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985.
 *
 */

#ifndef	_ScrapMgr_
#define _ScrapMgr_
	
#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

/* result codes */
enum { noTypeErr = -102, noScrapErr = -100 };


typedef struct	ScrapStuff
	{
	long		scrapSize;
	Handle		scrapHandle;
	int			scrapCount;
	int 		scrapState;
	StringPtr 	scrapName; 
 	} ScrapStuff,* PScrapStuff;


/*  functions returning non-integral values  */
pascal PScrapStuff InfoScrap();

/*  low-memory globals  */
extern ScrapStuff ScrapInfo : 0x960;


#endif
