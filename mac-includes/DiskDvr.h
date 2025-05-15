
/*
 *  DiskDvr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985.
 *
 */

#ifndef	_DiskDvr_
#define _DiskDvr_
	
/*  positioning  */
#define	currPos		0		/*  fsAtMark  */
#define	absPos		1		/*  fsFromStart  */
#define	relPos		3		/*  fsFromMark  */

typedef	struct
	{
	int				track;
	char			writeProt;
	char			diskInPlace;
	char			installed;
	char			sides;
	struct QElem 	*qLink;
	int				qType;
	int				dQDrive;
	int				dQRefNum;
	int				dQFSID;
	char			twoSideFmt;
	char			needsFlush;
	int				diskErrs;
	} DrvSts;

/*  result codes  */
#define firstDskErr		(-84)
enum {
	sectNFErr = -81,
	seekErr,
	spdAdjErr,
	twoSideErr,
	initIWMErr,
	tk0BadErr,
	cantStepErr,
	wrUnderrun,
	badDBtSlp,
	badDCksum,
	noDtaMkErr,
	badBtSlpErr,
	badCksmErr,
	dataVerErr,
	noAdrMkErr,
	noNybErr,
	offLinErr,
	noDriveErr
};
#define lastDskErr		(-64)


#endif