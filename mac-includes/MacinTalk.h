/*
 *  MacinTalk.h
 *
 *  Copyright (c) 1986 THINK Technologies, Inc.
 *  These interfaces are based on information published by Apple Computer. 
 *
 */

#ifndef	_MacTypes_
#include "MacTypes.h"
#endif

typedef int SpeechErr;

typedef char SpeechRecord[100];

typedef SpeechRecord *SpeechPointer, **SpeechHandle;

typedef enum { Male, Female } Sex;

typedef enum { Natural, Robotic, NoChange } FOMode;

typedef enum { xEnglish, French, Spanish, German, Italian } Language;

typedef struct {
	Sex		theSex;
	Language	theLanguage;
	int			theRate;
	int			thePitch;
	FOMode		theMode;
	Str255		theName;
	long		refCon;
	} VoiceRecord, *VoicePtr;
	
pascal	SpeechErr	SpeechOn();
pascal	void		SpeechOff();
pascal	void		SpeechRate();
pascal	void		SpeechPitch();
pascal	void		SpeechSex();
pascal	int			Reader();
pascal	int			MacinTalk();
