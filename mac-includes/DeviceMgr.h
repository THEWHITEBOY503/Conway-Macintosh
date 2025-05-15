
/*
 *  DeviceMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985-86.
 *
 */

#ifndef _DeviceMgr_
#define _DeviceMgr_
	
#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

#ifndef _OSUtil_
#include "OSUtil.h"
#endif


/*  parameter block for Control/Status calls  */
typedef struct
	{
	QElemPtr	qLink;
	int			qType;
	int			ioTrap;
	Ptr			ioCmdAddr;
	ProcPtr		ioCompletion;
	OsErr		ioResult;
	StringPtr	ioNamePtr;
	int			ioVRefNum;
	int			ioRefNum;
	int			csCode;
	int			csParam[];
	} cntrlParam;

/*  csCode  */
enum {
	goodBye = -1,
	killCode = 1,
	accEvent = 64,
	accRun,
	accCursor,
	accMenu,
	accUndo,
	accCut = 70,
	accCopy,
	accPaste,
	accClear
};


/*  device control entry  */
typedef	struct	DCtlEntry
	{
	Ptr				dCtlDriver;
	int				dCtlFlags;
	QHdr			dCtlQHdr;
	long			dCtlPosition;
	Handle			dCtlStorage;
	int				dCtlRefNum;
	long			dCtlCurTicks;
	struct GrafPort	*dCtlWindow;
	int				dCtlDelay;
	int				dCtlEMask;
	int				dCtlMenu;
	} DCtlEntry ;
typedef DCtlEntry *	DCtlPtr;
typedef	DCtlPtr *	DCtlHandle;

/*  dCtlFlags bits  */
#define	dNeedLock		0x4000
#define dNeedTime		0x2000
#define dNeedGoodBye	0x1000
#define dStatEnable		0x0800
#define dCtlEnable		0x0400
#define dWritEnable		0x0200
#define dReadEnable		0x0100
#define drvrActive		0x0080
#define dRAMBased		0x0040
#define dOpened			0x0020

/*  traps  */
#define aRdCmd			2
#define aWrCmd			3
#define asyncTrpBit		0x0400
#define noQueueBit		0x0200

/*  result codes  */
enum {
	notOpenErr = -28,
	abortErr,
	dInstErr,
	dRemovErr,
	closeErr,
	openErr,
	unitEmptyErr,
	badUnitErr,
	writErr,
	readErr,
	statusErr,
	controlErr
};

/*  chooser interface  */
enum {
	chooserID,			/*  caller value for chooser  */
	newSelMsg = 12,		/*  message values  */
	fillListMsg,
	getSelMsg,
	selectMsg,
	deselectMsg,
	terminateMsg,
	buttonMsg = 19
};


/*  functions returning non-integral values  */
pascal DCtlHandle GetDCtlEntry();

/*  low-memory globals  */
extern DCtlHandle *UTableBase : 0x11C;
extern ProcPtr Lvl1DT[] : 0x192;
extern ProcPtr Lvl2DT[] : 0x1B2;
extern int UnitNtryCnt : 0x1D2;
extern Ptr VIA : 0x1D4;
extern Ptr SCCRd : 0x1D8;
extern Ptr SCCWr : 0x1DC;
extern Ptr IWM : 0x1E0;
extern ProcPtr ExtStsDT[] : 0x2BE;


#endif _DeviceMgr_
