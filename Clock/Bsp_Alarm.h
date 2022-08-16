#ifndef ___BSP_ALARM_H_
#define ___BSP_ALARM_H_

#include "Bsp_Clock.h"


/// <summary>
/// Init Alarm and set off
/// </summary>
void InitAlarm();

/// <summary>
/// Get Alarm Status
/// </summary>
/// <returns>
/// true :Alarm on,Flase Alarm off
/// </returns>
_Bool AlarmStatus();

/// <summary>
/// Enable Alarm
/// </summary>
void EnableAlarm();

/// <summary>
/// Disable Alarm
/// </summary>
void DisableAlarm();

/// <summary>
/// Get Alarm click or nor
/// </summary>
/// <param name="compare"> Input Compare Variables</param>
/// <returns>ture means time up; false means not at the time</returns>
_Bool TimeUpSign(Date);

/// <summary>
/// Set Alarm
/// </summary>
/// <param name="Target">Set</param>
void SetAlarm(Date);

/// <summary>
/// Get Alarm Date
/// </summary>
/// <returns>Alarm Date</returns>
Date GetCurrentAlarm();

#endif // !___BSP_ALARM_H_
