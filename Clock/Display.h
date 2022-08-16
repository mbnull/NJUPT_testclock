#ifndef ___DISPLAY_H_
#define ___DISPLAY_H_

#include"Bsp_Clock.h"
#include"Bsp_Key.h"

typedef enum DisPlayGraph
{
	NormalClock=1,
	NormalMonth,
	Alarm,
	ConfMode,
	ConfMinute,
	ConfHour,
	ConfDate,
	ConfMonth,
	ConfAlarmHour,
	ConfAlarmMinute,
}DisplayGraphyType;

void DisPlay(DisplayGraphyType);


#endif
