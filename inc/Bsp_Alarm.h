#ifndef	_BSP_ALARM_H_
#define _BSP_ALARM_H_


#include "Bsp_Clock.h"

void InitAlarm(void);

_Bool AlarmStatus(void);

void EnableAlarm(void);

void DisableAlarm(void);

_Bool TimeUpSign(Date CompareV);

void SetAlarm(Date Target);

Date GetCurrentAlarm(void);


#endif
