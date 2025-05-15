
/*
 *  OSUtil.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985-86.
 *
 */

#ifndef	_OSUtil_
#define _OSUtil_

#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

/*  result codes  */
enum {
	prInitErr = -88,
	prWrErr,
	clkWrErr,
	clkRdErr,
	vTypErr = -2,
	qErr
};

/* queue types */
enum {
	vType = 1,
	ioQType,
	drvType,
	evType,
	fsQType
};

/* machine types */
enum { macXLMachine, macMachine };

/* trap types */
enum { OSTrap, ToolTrap };

/* serial port use */
enum { useFree, useATalk, useAsync };

typedef	struct	
	{
	char		valid;
	char		aTalkA;
	char		aTalkB;
	char		config;
	int			portA;
	int			portB;
	long		alarm;
	int			font;
	int			kbdPrint;
	int			volClik;
	int			misc;
	} SysParmType,* SysPPtr ;

typedef	struct QElem
	{
	struct QElem *qLink;
	int			qType;
	char		qData[];
	} QElem, *QElemPtr;

typedef	struct QHdr
    {		
    int			qFlags;
    QElemPtr	qHead;
    QElemPtr	qTail;
    } QHdr,* QHdrPtr ;
	
typedef	struct	
	{
	int			year;
	int			month;
	int			day;
	int			hour;
	int			minute;
	int			second;
	int			dayOfWeek;
	} DateTimeRec ;


/*  access A5 from interrupt level  */
#define SetUpA5()		asm {	move.l	a5,-(sp)	\
								move.l	0x904,a5	}
#define RestoreA5()		asm {	move.l	(sp)+,a5	}


/*  functions returning non-integral values  */
pascal SysPPtr GetSysPPtr();

/*  low-memory globals  */
extern int SysVersion : 0x15A;
extern SysParmType SysParam : 0x1F8;
extern long Time : 0x20C;


#endif
