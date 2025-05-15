
/*
 *  SCSIMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1986.
 *
 */

#ifndef	_SCSIMgr_
#define _SCSIMgr_


/*  transfer instruction op codes  */
enum {
	scInc = 1,
	scNoInc,
	scAdd,
	scMove,
	scLoop,
	scNop,
	scStop,
	scComp
};

/*  result codes  */
enum {
	scCommErr = 2,
	scBadParmsErr = 4,
	scPhaseErr,
	scCompareErr
};


typedef struct SCSIInstr {
	int			scOpcode;
	long		scParam1;
	long		scParam2;
} SCSIInstr;


#endif