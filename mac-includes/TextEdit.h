
/*
 *  TextEdit.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published in
 *  "Inside Macintosh" by Apple Computer, Addison-Wesley,
 *  Reading (Mass.), 1985.
 *
 */

#ifndef	_TextEdit_
#define _TextEdit_

#ifndef	_Quickdraw_
#include "Quickdraw.h"
#endif

/* justifications */
enum { teJustRight = -1, teJustLeft, teJustCenter };

typedef	char Chars[1], *CharsPtr, **CharsHandle;

typedef	struct
	{
	Rect	destRect ;
	Rect	viewRect ;
	Rect	selRect ;
	int		lineHeight ;
	int		fontAscent ;
	Point	selPoint ;
	int		selStart ;
	int		selEnd ;
	int		active ;
	ProcPtr	wordBreak ;
	ProcPtr	clikLoop ;
	long	clickTime ;
	int		clickLoc ;
	long	caretTime ;
	int		caretState ;
	int		just ;
	int		teLength ;
	Handle	hText ;
	int		recalBack ;
	int		recalLines;
	int		clikStuff ;
	int		crOnly ;
	int		txFont ;
	char	txFace ;
	int		txMode ;
	int		txSize ;
	GrafPtr	inPort ;
	ProcPtr	highHook ;
	ProcPtr caretHook ;
	int		nLines ;
	int		lineStarts[];
	} TERec, *TEPtr, **TEHandle ;


/*  functions returning non-integral values  */
pascal TEHandle TENew();
pascal CharsHandle TEGetText();
pascal Handle TEScrapHandle();

/*  low-memory globals  */
extern int TEScrpLength : 0xAB0;
extern Handle TEScrpHandle : 0xAB4;


#endif
