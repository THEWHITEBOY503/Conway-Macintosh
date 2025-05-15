
/*
 *  IntlPkg.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985.
 *
 */


#ifndef	_IntlPkg_
#define _IntlPkg_

#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

#define	currSymLead		16
#define	currNegSym		32
#define	currTrailingZ	64
#define	currLeadingZ	128

enum { mdy, dmy, ymd };

#define	century		128
#define	mntLdingZ	64
#define	dayLdingZ	32

#define	hrLeadingZ	128
#define	minLeadingZ	64
#define	secLeadingZ	32

enum {
	verUS,
	verFrance,
	verBritain,
	verGermany,
	verItaly,
	verNetherlands,
	verBelgiumLux,
	verSweden,
	verSpain,
	verDenmark,
	verPortugal,
	verFrCanada,
	verNorway,
	verIsrael,
	verJapan,
	verAustralia,
	verArabia,
	verFinland,
	verFrSwiss,
	verGrSwiss,
	verGreece,
	verIceland,
	verMalta,
	verCyprus,
	verTurkey,
	verYugoslavia
};


typedef	struct	Intl0Rec
	{
	char	decimalPt;
	char	thousSep;
	char	listSep;
	char	currSym1;
	char	currSym2;
	char	currSym3;
	char	currFmt	;
	char	dateOrder;
	char	shortDateFmt;
	char	dateSep	;
	char	timeCycle;
	char	timeFmt	;
	char	mornStr[4];
	char	eveStr[4];
	char	timeSep;
	char	time1Suff;
	char	time2Suff;
	char	time3Suff;
	char	time4Suff;
	char	time5Suff;
	char	time6Suff;
	char	time7Suff;
	char	time8Suff;
	char	metricSys;
	int		Intl0Vers;
	} Intl0Rec,* Intl0Ptr,** Intl0Hndl ;

typedef	struct	Intl1Rec
	{
	char	days[7][16];
	char	months[12][16];
	char	suppressDay;
	char	longDateFmt;
	char	dayleading0;
	char	abbrLen;
	char	st0[4];   /* check to see if these should be long */
	char	st1[4];
	char	st2[4];
	char	st3[4];
	char	st4[4];
	int		intl1Vers;
	int		localRtn;
	} Intl1Rec,* Intl1Ptr,** Intl1Hndl ;


typedef enum { shortDate, longDate, abbrevDate } DateForm;


/*  functions returning non-integral values  */
pascal Handle IUGetIntl();


#endif
