#include "Bsp_Alarm.h"

//private variables
_Bool AlarmBool = 0;
Date AlarmDate;

//puvlic functions
void InitAlarm()
{
	AlarmDate.Hour = 0;
	AlarmDate.Minute = 0;
	AlarmDate.Second = 0;
	AlarmDate.Day = 1;
	AlarmDate.Month = 1;
	AlarmBool = 0;
}

_Bool AlarmStatus()
{
	return AlarmBool;
}

void EnableAlarm()
{
	AlarmBool = 1;
}

void DisableAlarm()
{
	AlarmBool = 0;
}

_Bool TimeUpSign(Date CompareV)
{
	if (!AlarmBool)
	{
		return 0;
	}
	else
	{
		if (CompareV.Hour == AlarmDate.Hour)
		{
			if (CompareV.Minute - AlarmDate.Minute <= 3 && CompareV.Minute - AlarmDate.Minute >= 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
}

void SetAlarm(Date Target)
{
	AlarmDate.Hour = Target.Hour;
	AlarmDate.Minute = Target.Minute;
}

Date GetCurrentAlarm()
{
	Date TmpDate;
	TmpDate.Month = AlarmDate.Month;
	TmpDate.Day = AlarmDate.Day;
	TmpDate.Hour = AlarmDate.Hour;
	TmpDate.Minute = AlarmDate.Minute;
	TmpDate.Second = AlarmDate.Second;

	return TmpDate;
}
