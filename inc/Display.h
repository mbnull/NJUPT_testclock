#ifndef ___DISPLAY_H_
#define ___DISPLAY_H_

#include "u8g2.h"

void InitDisplay(void);

typedef enum DisPlayGraph
{
	NormalClock=1,
	NormalMonth,
	Alarm,
	ConfMode,
	ConfOn,
	ConfMinute,
	ConfHour,
	ConfDate,
	ConfMonth,
	ConfAlarmHour,
	ConfAlarmMinute,
}DisplayGraphyType;

void ChangeDisplayMode(DisplayGraphyType);

void Display(u8g2_t* u8g2);

#endif
