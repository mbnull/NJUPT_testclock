#include "Bsp_Key.h"

///delete when use
_Bool ReadPin();
int GetTick();

/// <summary>
/// define of proteced functions
/// </summary>
void ReBuidProtecedDate(Date);

///private struct

/*three kind of modes :	
* 1:normal clock (with two func,1:normal time, 2:date&month)
* 2:alarm (with only one func 1:start/stop alarm)
* 3:confclock (
* 4:confalarm(hided,with 2 func 1:conf minute,2: conf hour)
*/
typedef struct __Status
{
	int Mode;
	int FuncKey;
	_Bool ConfigEnable;///while this variable is true ,means is enabled conf mode ,when it's false ,means change mode;
	int ConfigClockAlarm;///while this value equil 0,no conf ready,1 means conf clock,2 means conf alarm
}Status;

///private variable
Date PrivateDate;
Date AlarmDateCopy;
Status PrivateStatus;


///private functions extern

void UpdateDisplay();
void ChangeLocalClok(int Type);
void ResetStatus();

/// <summary>
/// public functions
/// </summary>


void KeyInit()
{
	PrivateDate = GetTime();
	AlarmDateCopy=GetCurrentAlarm();

	PrivateDate.Second = 0;


	PrivateStatus.Mode = 1;
	PrivateStatus.FuncKey = 1;
	PrivateStatus.ConfigClockAlarm = 0;
	PrivateStatus.ConfigEnable = 0;
}



void ModeKeyDown()
{
	if (ReadPin())
	{
		if (PrivateStatus.Mode == 1)
		{
			PrivateStatus.Mode += 1;
		}
		else if (PrivateStatus.Mode == 2)
		{
			PrivateStatus.Mode += 1;
		}
		else if (PrivateStatus.Mode == 3)
		{
			if (PrivateStatus.ConfigEnable)
			{
				PrivateStatus.Mode += 1;
			}
			else
			{
				ResetStatus();
			}
		}
		else
		{
			switch (PrivateStatus.ConfigClockAlarm)
			{
			case 0:
				if (PrivateStatus.Mode == 4)
				{
					ResetStatus();
				}
				break;
			case 1:
			case 2:
				if (PrivateStatus.Mode == 6)
				{
					ResetStatus();
				}
				else
				{
					PrivateStatus.Mode += 1;
				}
				break;
			}
		}
	}
	UpdateDisplay();
	return;
}



void FuncKeyDown()
{
	if (PrivateStatus.Mode == 1 || PrivateStatus.Mode == 2)
	{
		PrivateStatus.FuncKey = PrivateStatus.FuncKey == 1 ? 2 : 1;
	}
	else if (PrivateStatus.Mode == 3)
	{
		if (PrivateStatus.ConfigEnable)
		{
			if (PrivateStatus.ConfigClockAlarm == 0)
			{
				PrivateStatus.ConfigClockAlarm = 1;
			}
			else
			{
				ChangeLocalClok(MINUTE_NUM);
			}
		}
		else
		{
			PrivateStatus.ConfigEnable = 1;
		}
	}
	else if (PrivateStatus.Mode == 4)
	{
		if (PrivateStatus.ConfigClockAlarm == 0)
		{
			PrivateStatus.ConfigClockAlarm = 2;
		}
		else if (PrivateStatus.ConfigClockAlarm == 1)
		{
			ChangeLocalClok(HOUR_NUM);
		}
		else if (PrivateStatus.ConfigClockAlarm == 2)
		{
			return;
		}
	}
	else if (PrivateStatus.Mode == 5)
	{
		if (PrivateStatus.ConfigClockAlarm == 1)
		{
			ChangeLocalClok(DATE_NUM);
		}
		else if (PrivateStatus.ConfigClockAlarm == 2)
		{
			ChangeLocalClok(MINUTE_NUM);
		}
	}
	else if (PrivateStatus.Mode == 6)
	{
		if (PrivateStatus.ConfigClockAlarm == 1)
		{
			ChangeLocalClok(MONTH_NUM);
		}
		else if (PrivateStatus.ConfigClockAlarm == 2)
		{
			ChangeLocalClok(HOUR_NUM);
		}
	}
	else
	{
		KeyInit();
	}
}

/// <summary>
/// private funtions
/// </summary>

void ChangeLocalClok(int Type)
{
	switch (Type)
	{
	case MINUTE_NUM:
		if (PrivateStatus.ConfigClockAlarm == 1)
		{
			if (PrivateDate.Minute == 60)
			{
				PrivateDate.Minute = 1;
			}
			else
			{
				PrivateDate.Minute += 1;
			}
		}
		else if (PrivateStatus.ConfigClockAlarm == 2)
		{
			if (AlarmDateCopy.Minute == 60)
			{
				AlarmDateCopy.Minute = 1;
			}
			else
			{
				AlarmDateCopy.Minute += 1;
			}
		}
		break;
	case HOUR_NUM:
		if (PrivateStatus.ConfigClockAlarm == 1)
		{
			if (PrivateDate.Hour == 23)
			{
				PrivateDate.Hour = 0;
			}
			else
			{
				PrivateDate.Hour += 1;
			}
		}
		else if (PrivateStatus.ConfigClockAlarm == 2)
		{
			if (AlarmDateCopy.Hour == 23)
			{
				AlarmDateCopy.Hour = 0;
			}
			else
			{
				AlarmDateCopy.Hour += 1;
			}
		}
		break;
	case DATE_NUM:
		switch (PrivateDate.Month)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if (PrivateDate.Date == 31)
			{
				PrivateDate.Date = 1;
			}
			else
			{
				PrivateDate.Date += 1;
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if (PrivateDate.Date == 30)
			{
				PrivateDate.Date = 1;
			}
			else
			{
				PrivateDate.Date += 1;
			}
		case 2:
			if (PrivateDate.Date == 28)
			{
				PrivateDate.Date = 1;
			}
			else
			{
				PrivateDate.Date += 1;
			}
			break;
		default:
			break;
		}
		break;
	case MONTH_NUM:
		if (PrivateDate.Month == 12)
		{
			PrivateDate.Hour = 1;
		}
		else
		{
			PrivateDate.Hour += 1;
		}
		break;
	default:
		break;
	}
}

inline void ResetStatus()
{
	PrivateStatus.Mode = 1;
	PrivateStatus.ConfigClockAlarm = 0;
	PrivateStatus.ConfigEnable = 0;
}

void UpdateDisplay()
{
	return;
}



_Bool ReadPin()
{
	return 1;
}

int GetTick()
{
	return 1000;
}
