#ifndef ___BSP_CLOCK_H_
#define ___BSP_CLOCK_H_

///Public

#define MINUTE_NUM 11
#define HOUR_NUM 21
#define DATE_NUM 31
#define MONTH_NUM 41

typedef  struct _MyDate
{
    unsigned char Month;
    unsigned char Date;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
}Date;

Date InitDateStruct();

Date GetTime();

void UpdateSecond();

extern void ReBuidProtecedDate(Date);

#endif /*CLOCK.H*/