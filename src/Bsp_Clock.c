#include "Bsp_Clock.h"



///public variable
Date PublicDate;

///private variable
static Date ProtectedDate;

///public functions
void UpdateData(void);
Date GetTime(void);

//private functions
static void UpdateMonth(void);

static void UpdateDate(void);

static void UpdateHour(void);

static void UpdateMinute(void);

inline static _Bool RangeCheck(void);
//end private functions


/// Public Function
Date GetTime()
{
    UpdateData();
    return PublicDate;
}

void UpdateSecond()
{
    if (!RangeCheck())
    {
        return;
    }
    if (ProtectedDate.Second == 59)
    {
        ProtectedDate.Second = 0;
        UpdateMinute();
        return;
    }
    else
    {
        ProtectedDate.Second += 1;
        return;
    }
}

///Proteced Functions
void ReBuidProtecedDate(Date date)
{
    ProtectedDate.Month = date.Month;
    ProtectedDate.Day = date.Day;
    ProtectedDate.Hour = date.Hour;
    ProtectedDate.Minute = date.Minute;
    ProtectedDate.Second = date.Second;
    UpdateData();
}

/// Private Functions
Date InitDateStruct()
{
    ProtectedDate.Month = 1;
    ProtectedDate.Day = 1;
    ProtectedDate.Hour = 0;
    ProtectedDate.Minute = 0;
    ProtectedDate.Second = 0;
    UpdateData();
    return PublicDate;
}


void UpdateData()
{
    PublicDate.Month = ProtectedDate.Month;
    PublicDate.Day = ProtectedDate.Day;
    PublicDate.Hour = ProtectedDate.Hour;
    PublicDate.Minute = ProtectedDate.Minute;
    PublicDate.Second = ProtectedDate.Second;
    return;
}

//Correct 1 Wrong 0
inline _Bool RangeCheck()
{
    if (ProtectedDate.Month == 0 ||
        ProtectedDate.Day == 0 ||
        ProtectedDate.Hour >= 24 ||
        ProtectedDate.Minute >= 60 ||
        ProtectedDate.Second >= 60 ||
        ProtectedDate.Month > 12)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void UpdateMonth()
{
    if (!RangeCheck())
    {
        return;
    }
    if (ProtectedDate.Month != 12)
    {
        ProtectedDate.Month += 1;
        return;
    }
    else
    {
        ProtectedDate.Month = 1;
        return;
    }
}

void  UpdateDate()
{
    if (!RangeCheck())
    {
        return;
    }
    if (ProtectedDate.Month == 2)
    {
        if (ProtectedDate.Day > 28)
        {
            return;
        }
        else if (ProtectedDate.Day == 28)
        {
            ProtectedDate.Day = 1;
            UpdateMonth();
            return;
        }
        ProtectedDate.Day += 1;
    }
    else if (ProtectedDate.Month == 1 || ProtectedDate.Month == 3
        || ProtectedDate.Month == 5 || ProtectedDate.Month == 7
        || ProtectedDate.Month == 8 || ProtectedDate.Month == 10
        || ProtectedDate.Month == 12)
    {
        if (ProtectedDate.Day == 31)
        {
            ProtectedDate.Day = 1;
            UpdateMonth();
            return;
        }
        else
        {
            ProtectedDate.Day += 1;
            return;
        }
    }
    else
    {
        if (ProtectedDate.Day == 30)
        {
            ProtectedDate.Day = 1;
            UpdateMonth();
            return;
        }
        else
        {
            ProtectedDate.Day += 1;
            return;
        }
    }
}

void UpdateHour()
{
    if (!RangeCheck())
    {
        return;
    }
    if (ProtectedDate.Hour == 23)
    {
        ProtectedDate.Hour = 0;
        UpdateDate();
        return;
    }
    else
    {
        ProtectedDate.Hour += 1;
        return;
    }
}

void UpdateMinute()
{
    if (!RangeCheck())
    {
        return;
    }
    if (ProtectedDate.Minute == 59)
    {
        ProtectedDate.Minute = 0;
        UpdateHour();
        return;
    }
    else
    {
        ProtectedDate.Minute += 1;
        return;
    }
}

