#include "Display.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "gpio.h"
#include "main.h"
#include "u8g2.h"



/////private variables
char *myitoa(int num,char *str,int radix) 
{  
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned unum; 
	int i=0,j,k; 
	if(radix==10&&num<0)
	{ 
		unum=(unsigned)-num; 
		str[i++]='-'; 
	} 
	else unum=(unsigned)num; 
	do  
	{ 
		str[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	str[i]='\0'; 
	if(str[0]=='-') k=1; 
	else k=0; 
	char temp; 
	for(j=k;j<=(i-k-1)/2.0;j++) 
	{ 
		temp=str[j]; 
		str[j]=str[i-j-1]; 
		str[i-j-1]=temp; 
	} 
	return str; 
}

static Date DisplayDate;

DisplayGraphyType	pDGT=1;


void ChangeDisplayMode(DisplayGraphyType Trigger)
{
	pDGT=Trigger;
	return;
}

char *ll(int num)
{
	char *a="0";
	char b='0';
	b+=num;
	*a=b;
	return a;
}

void Display(u8g2_t* u8g2)
{
	DisplayDate=GetTime();
	char *minute=(char*)malloc(20);
	memset(minute, 0, 20);
	char *sec=(char*)malloc(20);
	memset(sec, 0, 20);
	char *hour=(char*)malloc(20);
	memset(hour, 0, 20);
	char *result = (char *)malloc(100);
	memset(result, 0, 100);
	myitoa(DisplayDate.Hour,hour,10);
	myitoa(DisplayDate.Minute,minute,10);
	myitoa(DisplayDate.Second,sec,10);
	sprintf(result,"%s:%s:%s",hour,minute,sec);
	static int a=0;
	a++;
	switch(pDGT)
	{
	case NormalClock:
		a=3;
		u8g2_ClearBuffer(u8g2);
		u8g2_SetFont(u8g2,u8g2_font_courR14_tf);
		sprintf(result,"%d",a);
		u8g2_DrawStr(u8g2,15,15,result);
		u8g2_SendBuffer(u8g2);
		//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		//HAL_Delay(100);
		break;
	case NormalMonth:
		break;
	case Alarm:
		
		break;
	case ConfMode:
		break;
	case ConfOn:
		break;
	case ConfMinute:
		break;
	case ConfHour:
		break;
	case ConfDate:
		break;
	case ConfMonth:
		break;
	case ConfAlarmHour:
		break;
	case ConfAlarmMinute:
		break;
	default:
		return;
		break;
	}
}


/////private funcs
//char *ToStringDouble(int list)
//{
//	char *str={0};
//	int site=0;
//	if(list<10)
//	{
//		str[site++]='0';
//	}
//	if(list==0)
//	{
//		str[0]='0';
//		str[1]='0';
//		return str;
//	}
//	return str;
//}


//void DisplayClock()
//{
//	ssd1306_Fill(Black);
//	char *str;
//	str=ToStringDouble(DisplayDate.Hour);
//	const char *tmp=ToStringDouble(DisplayDate.Minute);
//	str+=':';
//	strcat(str,tmp);
//	str+=':';
//	const char *sec=ToStringDouble(DisplayDate.Second);
//	strcat(str,sec);
//	ssd1306_SetCursor(20,22);
//	///00:00:00
//	ssd1306_WriteString(str,Font_11x18,White);
//}

//void DisplayMonth()
//{
//	ssd1306_Fill(Black);
//	char *str;
//	str=ToStringDouble(DisplayDate.Month);
//	const char *tmp=ToStringDouble(DisplayDate.Day);
//	str+='/';
//	strcat(str,tmp);
//	ssd1306_SetCursor(20,22);
//	ssd1306_WriteString(str,Font_11x18,White);
//}

//void DisplayAlarm()
//{
//	
//	DisplayDate=GetCurrentAlarm();
//	ssd1306_Fill(Black);
//	char *str;
//	str=ToStringDouble(DisplayDate.Hour);
//	const char *tmp=ToStringDouble(DisplayDate.Minute);
//	str+=':';
//	strcat(str,tmp);
//	ssd1306_SetCursor(24,19);
//	ssd1306_WriteString(str,Font_16x26,White);
//	if(AlarmStatus())
//	{
//		ssd1306_SetCursor(0,0);
//		ssd1306_WriteString("on",Font_7x10,White);
//	}
//	else
//	{
//		ssd1306_SetCursor(0,0);
//		ssd1306_WriteString("off",Font_7x10,White);
//	}
//}

//void DisplayConfMode()
//{
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *tmp=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,tmp);
//		str+=':';
//		const char *sec=ToStringDouble(DisplayDate.Second);
//		strcat(str,sec);
//		ssd1306_SetCursor(20,22);
//		///00:00:00
//		ssd1306_WriteString(str,Font_11x18,White);
//}

//void DisplayConfOn()
//{
//	static uint8_t count=0;
//	count++;
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
//	if(count%5==0)
//	{
//		ssd1306_Fill(Black);
//		count = 0;
//	}
//	else
//	{
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *tmp=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,tmp);
//		str+=':';
//		const char *sec=ToStringDouble(DisplayDate.Second);
//		strcat(str,sec);
//		ssd1306_SetCursor(20,22);
//		///00:00:00
//		ssd1306_WriteString(str,Font_11x18,White);
//	}
//}

//void DisplayConfMinute()
//{
//	static uint8_t count=0;
//	count++;
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
//	if(count%5==0)
//	{
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *Minute="  ";
//		str+=':';
//		strcat(str,Minute);
//		str+=':';
//		const char *sec=ToStringDouble(DisplayDate.Second);
//		strcat(str,sec);
//		ssd1306_SetCursor(20,22);
//		///00:  :00
//		ssd1306_WriteString(str,Font_11x18,White);
//		count = 0;
//	}
//	else
//	{
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *tmp=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,tmp);
//		str+=':';
//		const char *sec="00";
//		strcat(str,sec);
//		ssd1306_SetCursor(20,22);
//		///00:00:00
//		ssd1306_WriteString(str,Font_11x18,White);
//	}
//}

//void DisplayConfHour()
//{
//	static uint8_t count=0;
//	count++;
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
//	if(count%5==0)
//	{
//		ssd1306_Fill(Black);
//		char *str;
//		str="  ";
//		const char *Minute=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,Minute);
//		str+=':';
//		const char *sec=ToStringDouble(DisplayDate.Second);
//		strcat(str,sec);
//		ssd1306_SetCursor(20,22);
//		///00:  :00
//		ssd1306_WriteString(str,Font_11x18,White);
//		count = 0;
//	}
//	else
//	{
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *tmp=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,tmp);
//		str+=':';
//		const char *sec="00";
//		strcat(str,sec);
//		ssd1306_SetCursor(20,22);
//		///00:00:00
//		ssd1306_WriteString(str,Font_11x18,White);
//	}
//}

//void DisplayConfDay()
//{
//	static uint8_t count=0;
//	count++;
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
//	if(count%5==0)
//	{
//			ssd1306_Fill(Black);
//			char *str;
//			str=ToStringDouble(DisplayDate.Month);
//			const char *tmp="  ";
//			str+='/';
//			strcat(str,tmp);
//			ssd1306_SetCursor(20,22);
//			ssd1306_WriteString(str,Font_11x18,White);
//			count = 0;
//	}
//	else
//	{
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Month);
//		const char *tmp=ToStringDouble(DisplayDate.Day);
//		str+='/';
//		strcat(str,tmp);
//		ssd1306_SetCursor(20,22);
//		ssd1306_WriteString(str,Font_11x18,White);
//	}
//}

//void DisplayConfMonth()
//{
//	static uint8_t count=0;
//	count++;
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
//	if(count%5==0)
//	{
//			ssd1306_Fill(Black);
//			char *str;
//			str="  ";
//			const char *tmp=ToStringDouble(DisplayDate.Day);
//			str+='/';
//			strcat(str,tmp);
//			ssd1306_SetCursor(20,22);
//			ssd1306_WriteString(str,Font_11x18,White);
//			count = 0;
//	}
//	else
//	{
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Month);
//		const char *tmp=ToStringDouble(DisplayDate.Day);
//		str+='/';
//		strcat(str,tmp);
//		ssd1306_SetCursor(20,22);
//		ssd1306_WriteString(str,Font_11x18,White);
//	}
//}

//void DisplayConfAlarmMinute()
//{
//	static uint8_t count=0;
//	count++;
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
//	if(count%5==0)
//	{
//		DisplayDate=GetCurrentAlarm();
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *tmp="  ";
//		str+=':';
//		strcat(str,tmp);
//		ssd1306_SetCursor(24,19);
//		ssd1306_WriteString(str,Font_16x26,White);
//	}
//	else
//	{
//		DisplayDate=GetCurrentAlarm();
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *tmp=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,tmp);
//		ssd1306_SetCursor(24,19);
//		ssd1306_WriteString(str,Font_16x26,White);
//	}
//}

//void DisplayConfAlarmHour()
//{
//	static uint8_t count=0;
//	count++;
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
//	if(count%5==0)
//	{
//		DisplayDate=GetCurrentAlarm();
//		ssd1306_Fill(Black);
//		char *str;
//		str="  ";
//		const char *tmp=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,tmp);
//		ssd1306_SetCursor(24,19);
//		ssd1306_WriteString(str,Font_16x26,White);
//	}
//	else
//	{
//		DisplayDate=GetCurrentAlarm();
//		ssd1306_Fill(Black);
//		char *str;
//		str=ToStringDouble(DisplayDate.Hour);
//		const char *tmp=ToStringDouble(DisplayDate.Minute);
//		str+=':';
//		strcat(str,tmp);
//		ssd1306_SetCursor(24,19);
//		ssd1306_WriteString(str,Font_16x26,White);
//	}
//}
/////public functions

//void InitDisplay()
//{
//	DisplayDate = GetTime();
//}

//void ChangeDisplayMode(DisplayGraphyType Type)
//{
//	pDGT=Type;
//}

//void Display()
//{
//	DisplayDate=GetTime();
//	switch (pDGT)
//	{
//	case NormalClock:
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
//		DisplayClock();
//		break;
//	case NormalMonth:
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
//		DisplayMonth();
//		break;
//	case Alarm:
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
//		DisplayAlarm();
//		break;
//	case ConfMode:
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
//		DisplayConfMode();
//		break;
//	case ConfOn:
//		DisplayConfOn();
//		break;
//	case ConfMinute:
//		DisplayConfMinute();
//		break;
//	case ConfHour:
//		DisplayConfHour();
//		break;
//	case ConfDate:
//		DisplayConfDay();
//		break;
//	case ConfMonth:
//		DisplayConfMonth();
//		break;
//	case ConfAlarmMinute:
//		DisplayConfAlarmMinute();
//		break;
//	case ConfAlarmHour:
//		DisplayConfAlarmHour();
//		break;
//	default:
//		break;
//	}
//	ssd1306_UpdateScreen(&hi2c1);
//	return;
//}
