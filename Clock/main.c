#include<stdio.h>
#include<stdbool.h>
#include<time.h>
#include<Windows.h>

#include "Main.h"

int main()
{
	Date MDate = InitDateStruct();
	while (true)
	{
		MDate = GetTime();
		UpdateSecond();
		//printf("%d/%d \n%d:%d:%d\n", MDate.Month, MDate.Date, MDate.Hour, MDate.Minute, MDate.Second);
		if (MDate.Month == 1)
		{
		}
	}
	return 0;
}