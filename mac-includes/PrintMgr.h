
/*
 *  PrintMgr.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985; and on the Public Toolbox Interface
 *  Source Code, copyright (c) 1985 Apple Computer, licensed
 *  to THINK Technologies by Apple Computer.
 *
 */

#ifndef _PrintMgr_
#define _PrintMgr_

#ifndef _DialogMgr_
#include "DialogMgr.h"
#endif


/* printing methods */
enum { bDraftLoop, bSpoolLoop, bUser1Loop, bUser2Loop };

/* printers */
enum { bDevCItoh = 1, bDevDaisy, bDevLaser };
#define iDevCItoh	0x0100
#define iDevDaisy	0x0200
#define iDevLaser	0x0300

/* PrCtlCall parameters */
#define iPrBitsCtl				 4
#define lScreenBits		0x00000000
#define lPaintBits		0x00000001
#define lHiScreenBits	0x00000010
#define lHiPaintBits	0x00000011
#define iPrIOCtl				 5
#define iPrEvtCtl				 6
#define lPrEvtAll		0x0002FFFD
#define lPrEvtTop		0x0001FFFD
#define iPrDevCtl				 7
#define lPrReset		0x00010000
#define lPrPageEnd		0x00020000
#define lPrLineFeed		0x00030000
#define lPrLFSixth		0x0003FFFF
#define lPrLFEighth		0x0003FFFE
#define iFMgrCtl				 8

/* result codes */
#define iPrSavPFil	(-1)
#define iIOAbort	(-27)
#define iMemFullErr	(-108)
#define iPrAbort	128

/* other constants */
#define sPrDrvr		"\p.Print"
#define iPrDrvrRef	(-3)
#define iPrPgFract	 120
#define iPrPgFst	   1
#define iPrPgMax	9999
#define iPrRelease	   3
#define iPfMaxPgs	 128


typedef struct TPrPort {
	GrafPort	gPort;
	QDProcs		gProcs;
	long		lGParam1;
	long		lGParam2;
	long		lGParam3;
	long		lGParam4;
	Boolean		fOurPtr;
	Boolean		fOurBits;
} TPrPort, *TPPrPort;

typedef struct TPrInfo {
	int			iDev;
	int			iVRes;
	int			iHRes;
	Rect		rPage;
} TPrInfo, *TPPrInfo;

typedef struct TPrJob {
	int			iFstPage;
	int			iLstPage;
	int			iCopies;
	SignedByte	bJDocLoop;
	Boolean		fFromUsr;
	ProcPtr		pIdleProc;
	StringPtr	pFileName;
	int			iFileVol;
	SignedByte	bFileVers;
	SignedByte	bJobX;
} TPrJob, *TPPrJob;

typedef enum { feedCut, feedFanfold, feedMechCut, feedOther} TFeed;

typedef struct TPrStl {
	int			wDev;
	int			iPageV;
	int			iPageH;
	SignedByte	bPort;
	TFeed		feed;
} TPrStl, *TPPrStl;

typedef enum { scanTB, scanBT, scanLR, scanRL} TScan;

typedef struct  TPrXInfo {
	int			iRowBytes;
	int			iBandV;
	int			iBandH;
	int			iDevBytes;
	int			iBands;
	SignedByte	bPatScale;
	SignedByte	bULThick;
	SignedByte	bULOffset;
	SignedByte	bULShadow;
	TScan		scan;
	SignedByte	bXInfoX;
} TPrXInfo, *TPPrXInfo;

typedef Rect *TPRect;

typedef struct TPrint {
	int			iPrVersion;
	TPrInfo		prInfo;
	Rect		rPaper;
	TPrStl		prStl;
	TPrInfo		prInfoPT;
	TPrXInfo	prXInfo;
	TPrJob		prJob;
	int			printX[19];
} TPrint, *TPPrint, **THPrint;

typedef struct TPrStatus {
	int			iTotPages;
	int			iCurPage;
	int			iTotCopies;
	int			iCurCopy;
	int			iTotBands;
	int			iCurBand;
	Boolean		fPgDirty;
	Boolean		fImaging;
	THPrint		hPrint;
	TPPrPort	pPrPort;
	PicHandle	hPic;
} TPrStatus, *TPPrStatus;

typedef struct TPfPgDir {
	int			iPages;
	long		lPgPos[iPfMaxPgs+1];
} TPfPgDir, *TPPfPgDir, **THPfPgDir;

typedef struct TPfHeader {
	TPrint		print;
	TPfPgDir	pfPgDir;
} TPfHeader, *TPPfHeader, **THPfHeader;

typedef struct TPrDlg {
	DialogRecord dlg;
	ProcPtr		pFltrProc;
	ProcPtr		pItemProc;
	THPrint		hPrintUsr;
	Boolean		fDoIt;
	Boolean		fDone;
	long		lUser1;
	long		lUser2;
	long		lUser3;
	long		lUser4;
  /* plus more stuff needed by the particular printing dialog */
} TPrDlg, *TPPrDlg;


/*  functions returning non-integral values  */
pascal TPPrPort PrOpenDoc();
pascal Handle PrDrvrDCE();

/*  low-memory globals  */
extern int PrintErr : 0x944;


#endif _PrintMgr_