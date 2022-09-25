/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	u8g2_t u8g2;
	RTC_TimeTypeDef rtcTD;
	RTC_DateTypeDef rtcDD;
	RTC_AlarmTypeDef rtcAD;
	
	RTC_TimeTypeDef PrivatertcTD;
	RTC_DateTypeDef PrivatertcDD;
	RTC_AlarmTypeDef PrivatertcAD;
	
	uint8_t GlobalStatus;
	uint8_t GlobalAlarmStatus;
	uint8_t ConfigStatus[2]={0};
	
	///0 display 1 alarm 2 config
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Init()
{
	PrivatertcTD.Hours = 0x0;
  PrivatertcTD.Minutes = 0x0;
  PrivatertcTD.Seconds = 0x0;
  PrivatertcTD.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  PrivatertcTD.StoreOperation = RTC_STOREOPERATION_RESET;
	
	
	rtcDD.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  rtcDD.Month = RTC_MONTH_SEPTEMBER;
  rtcDD.Date = 0x14;
  rtcDD.Year = 0x22;


	PrivatertcAD.AlarmTime.Hours = 0x0;
  PrivatertcAD.AlarmTime.Minutes = 0x10;
  PrivatertcAD.AlarmTime.Seconds = 0x0;
  PrivatertcAD.AlarmTime.SubSeconds = 0x0;
  PrivatertcAD.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  PrivatertcAD.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  PrivatertcAD.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_SECONDS;
  PrivatertcAD.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  PrivatertcAD.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  PrivatertcAD.AlarmDateWeekDay = 0x1;
  PrivatertcAD.Alarm = RTC_ALARM_A;
	
}

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
    U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
    U8X8_UNUSED void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_GPIO_AND_DELAY_INIT:
    HAL_Delay(1);
    break;
  case U8X8_MSG_DELAY_MILLI:
    HAL_Delay(arg_int);
    break;
  case U8X8_MSG_GPIO_DC:
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, arg_int);
    break;
  case U8X8_MSG_GPIO_RESET:
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, arg_int);    break;
  }
  return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
    void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
    HAL_SPI_Transmit(&hspi1, (uint8_t *) arg_ptr, arg_int, HAL_MAX_DELAY);
    break;
  case U8X8_MSG_BYTE_INIT:
    break;
  case U8X8_MSG_BYTE_SET_DC:
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, arg_int);
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
		HAL_GPIO_WritePin(OLED_NSS_GPIO_Port, OLED_NSS_Pin, GPIO_PIN_RESET);
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
		HAL_GPIO_WritePin(OLED_NSS_GPIO_Port, OLED_NSS_Pin, GPIO_PIN_SET);
    break;
  default:
    return 0;
  }
  return 1;
}

///bcd2dec
int decimal_bcd_code(int decimal)
{
    return (decimal - (decimal>>4) * 6);
}

///dec2bcd
int bcd_decimal_code( int bcd)
{
    return (bcd + (bcd/10) * 6);
}



void ConfigAlarm(uint8_t CS)
{
	switch(CS)
	{
	case 1:
		if(decimal_bcd_code(PrivatertcAD.AlarmTime.Minutes)==59)
		{
			PrivatertcAD.AlarmTime.Minutes=0;
		}
		else
		{
			int tmp=decimal_bcd_code(PrivatertcAD.AlarmTime.Minutes);
			PrivatertcAD.AlarmTime.Minutes=bcd_decimal_code(++tmp);
		}
		break;
	case 2:
		if(decimal_bcd_code(PrivatertcAD.AlarmTime.Hours)==23)
		{
			PrivatertcAD.AlarmTime.Hours=0;
		}
		else
		{
			int tmp=decimal_bcd_code(PrivatertcAD.AlarmTime.Hours);
			PrivatertcAD.AlarmTime.Hours=bcd_decimal_code(++tmp);
		}
		break;
	default:
		break;
	}
}

void ConfigDate(uint8_t CS)
{
	int month=PrivatertcDD.Month;
	switch(CS)
	{
	case 3:
		if(PrivatertcDD.Month==12)
		{
			PrivatertcDD.Month=1;
		}
		else
		{
			int tmp=PrivatertcDD.Month;
			PrivatertcDD.Month=++tmp;
		}
		break;
	case 4:
		{
			int tnum=0;
			switch(month)
			{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				tnum=31;
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				tnum=30;
			case 2:
				tnum=28;
			}
			if(PrivatertcDD.Date==bcd_decimal_code(tnum))
			{
				PrivatertcDD.Date=1;
			}
			else
			{
				int tmp=decimal_bcd_code(PrivatertcDD.Date);
				PrivatertcDD.Date=bcd_decimal_code(++tmp);
			}
		}
		break;
	default:
		GlobalStatus=0;
		ConfigStatus[1]=0;
		break;
	}
}

void ConfigClock(uint8_t CS)
{
	switch(CS)
	{
	case 1:
		if(decimal_bcd_code(PrivatertcTD.Minutes==60))
		{
			PrivatertcTD.Minutes=0;
		}
		else
		{
			int tmp=decimal_bcd_code(PrivatertcTD.Minutes);
			PrivatertcTD.Minutes=bcd_decimal_code(++tmp);
		}
		break;
	case 2:
		if(decimal_bcd_code(PrivatertcTD.Hours==23))
		{
			PrivatertcTD.Minutes=0;
		}
		else
		{
			int tmp=decimal_bcd_code(PrivatertcTD.Hours);
			PrivatertcTD.Hours=bcd_decimal_code(++tmp);
		}
		break;
	default:
		ConfigDate(CS);
		break;
	}
}



void FuncKey()
{
	switch(GlobalStatus)
	{
	case 0:
		GlobalAlarmStatus=~GlobalAlarmStatus;
		break;
	case 1:
		if(ConfigStatus[0])
		{
			ConfigAlarm(ConfigStatus[0]);
		}
		else
		{
			ConfigStatus[0]=1;
			HAL_RTC_GetAlarm(&hrtc,&PrivatertcAD,RTC_ALARM_A,RTC_FORMAT_BCD);
		}
		break;
	case 2:
		if(ConfigStatus[1]==1||ConfigStatus[1]==2)
		{
			ConfigClock(ConfigStatus[1]);
		}
		else if(ConfigStatus[1]==3||ConfigStatus[1]==4)
		{
			ConfigDate(ConfigStatus[1]);
		}
		else
		{
			ConfigStatus[1]=1;
			HAL_RTC_GetDate(&hrtc,&PrivatertcDD,RTC_FORMAT_BCD);
			HAL_RTC_GetTime(&hrtc,&PrivatertcTD,RTC_FORMAT_BCD);
		}
		break;
	default:
		GlobalStatus=0;
		break;
	}
}

void ModeKey()
{	
	if(GlobalStatus==2)
	{
		if(ConfigStatus[1])
		{
			switch(ConfigStatus[1])
			{
			case 1:
			case 2:
				ConfigStatus[1]++;

				break;
			case 3:
				ConfigStatus[1]++;
				break;
			case 4:
				ConfigStatus[1]=0;	
				GlobalStatus=0;
				HAL_RTC_SetDate(&hrtc,&PrivatertcDD,RTC_FORMAT_BCD);
				PrivatertcTD.Seconds=0x00;
				HAL_RTC_SetTime(&hrtc,&PrivatertcTD,RTC_FORMAT_BCD);
				break;
			default:
				ConfigStatus[1]=0;
				GlobalStatus=0;
				break;
			}
		}
		else
		{
			GlobalStatus=0;
			ConfigStatus[1]=0;
		}
	}
	else if(GlobalStatus==1)
	{
		if(ConfigStatus[0])
		{
			if(ConfigStatus[0]==1)
			{
				ConfigStatus[0]++;
			}
			else if(ConfigStatus[0]==2)
			{
				ConfigStatus[0]=0;
				HAL_RTC_SetAlarm(&hrtc,&PrivatertcAD,RTC_FORMAT_BCD);
				GlobalStatus=0;
			}
		}
		else
		{
			HAL_RTC_SetAlarm(&hrtc,&PrivatertcAD,RTC_FORMAT_BCD);
			GlobalStatus++;
		}
	}
	else
	{
		GlobalStatus++;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	//MX_TIM10_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	Init();
	
	u8g2_Setup_ssd1306_128x64_noname_f(&u8g2,U8G2_R0,u8x8_byte_4wire_hw_spi,u8x8_stm32_gpio_and_delay);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2,0);
	

	
	
	__HAL_TIM_CLEAR_IT(&htim10,TIM_IT_UPDATE);
	__HAL_TIM_CLEAR_IT(&htim11,TIM_IT_UPDATE);
	
	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Base_Start_IT(&htim11);
	
//	char *aa=malloc(50);
//	memset(aa,0,50);
//	int a[2]={0};
	int flag[3]={0};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		///mode key 
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)
			{
				HAL_Delay(50);
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)
				{
					flag[0]=1;
				}
			}
			if(flag[0])
			{
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET)
				{
					HAL_Delay(50);
					if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET)
					{
						flag[0]=0;
						ModeKey();
					}
				}
			}
		///func key
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_RESET)
		{
			HAL_Delay(50);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_RESET)
			{
				flag[1]=1;
			}
		}
		if(flag[1])
		{
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET)
			{
				HAL_Delay(50);
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_SET)
				{
					flag[1]=0;
					FuncKey();
				}
			}
		}
		
		
		///alarm check
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void GeneralDisplay(char *Time,char *Date,char *Alarm)
{
		u8g2_SetFont(&u8g2,u8g2_font_luIS18_tf);

		sprintf(Time,"%d:%d:%d",decimal_bcd_code(rtcTD.Hours),decimal_bcd_code(rtcTD.Minutes),decimal_bcd_code(rtcTD.Seconds));
		u8g2_DrawStr(&u8g2,20,53,Time);
		
		u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);

		sprintf(Date,"%d/%d",rtcDD.Month,decimal_bcd_code(rtcDD.Date));
		u8g2_DrawStr(&u8g2,10,10,Date);
		
		
		if(GlobalAlarmStatus)
			sprintf(Alarm,"%d:%d",decimal_bcd_code(rtcAD.AlarmTime.Hours),decimal_bcd_code(rtcAD.AlarmTime.Minutes));
		else
			sprintf(Alarm,"-:-");
			
		u8g2_DrawStr(&u8g2,100,20,Alarm);
		u8g2_DrawStr(&u8g2,94,10,"Alarm");
		
}

//void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
//{

//	u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
//	u8g2_DrawStr(&u8g2,20,30,"Time UP");
//	u8g2_SendBuffer(&u8g2);
//	GlobalAlarmStatus=~GlobalAlarmStatus;
//	if(GlobalAlarmStatus)
//	{
//		u8g2_DrawStr(&u8g2,20,30,"Time UP");
//		u8g2_SendBuffer(&u8g2);
//		uint32_t time=HAL_GetTick();
//		while(HAL_GetTick()-time<15000)
//		{
//			HAL_GPIO_WritePin(BEE_GPIO_Port,BEE_Pin,GPIO_PIN_RESET);
//			if((HAL_GetTick()-time)%1000==0)
//			{
//				HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
//			}
//		}
//		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
//	}
//	else
//	{
//	
//	}
//	return;
//}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	
	char Alarm[7]={0};
	char Time[9]={0};
	char Date[6]={0};
	char TestData[20]={0};
	
	HAL_RTC_GetTime(hrtc,&rtcTD,RTC_FORMAT_BCD);
	HAL_RTC_GetDate(hrtc,&rtcDD,RTC_FORMAT_BCD);
	HAL_RTC_GetAlarm(hrtc,&rtcAD,RTC_ALARM_A,RTC_FORMAT_BCD);
	
	u8g2_ClearBuffer(&u8g2);
	
	if(!ConfigStatus[0]&&!ConfigStatus[1])
	{
		if(GlobalAlarmStatus)
		{
			if(rtcTD.Hours==rtcAD.AlarmTime.Hours)
			{
				if(decimal_bcd_code(rtcTD.Minutes)-decimal_bcd_code(rtcAD.AlarmTime.Minutes)<5&&
				decimal_bcd_code(rtcTD.Minutes)-decimal_bcd_code(rtcAD.AlarmTime.Minutes)>=0)
				{
					u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
					u8g2_DrawStr(&u8g2,20,30,"Time UP");
				}
			}
		}
	}

	
	
	if(GlobalStatus==0)
	{
		GeneralDisplay(Time,Date,Alarm);
		u8g2_SendBuffer(&u8g2);
	}
	else if(GlobalStatus==1)
	{
		if(!ConfigStatus[0])
		{
			GeneralDisplay(Time,Date,Alarm);
			
			u8g2_DrawStr(&u8g2,10,20,"press func key ");
			u8g2_DrawStr(&u8g2,10,30,"to config alarm");
			
			u8g2_SendBuffer(&u8g2);
		}
		else
		{
			u8g2_ClearBuffer(&u8g2);
			switch(ConfigStatus[0])
			{
			case 1:
				u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
				u8g2_DrawStr(&u8g2,30,20,"Config Minutes");
				break;
			case 2:
				u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
				u8g2_DrawStr(&u8g2,30,20,"Config Hours");
			default:
				break;
			}
			u8g2_SetFont(&u8g2,u8g2_font_logisoso28_tn);
			sprintf(Alarm,"%d:%d",decimal_bcd_code(PrivatertcAD.AlarmTime.Hours),decimal_bcd_code(PrivatertcAD.AlarmTime.Minutes));
			u8g2_DrawStr(&u8g2,30,55,Alarm);
			u8g2_SendBuffer(&u8g2);
		}

	}
	else
	{
		if(!ConfigStatus[1])
		{
			GeneralDisplay(Time,Date,Alarm);
			
			u8g2_DrawStr(&u8g2,10,20,"press func key ");
			u8g2_DrawStr(&u8g2,10,30,"to set clock&date");
			
			u8g2_SendBuffer(&u8g2);
		}
		else
		{
			u8g2_ClearBuffer(&u8g2);
			switch(ConfigStatus[1])
			{
			case 1:
				u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
				u8g2_DrawStr(&u8g2,30,20,"Config Minutes");
				u8g2_SetFont(&u8g2,u8g2_font_logisoso28_tn);
				sprintf(Time,"%d:%d",decimal_bcd_code(PrivatertcTD.Hours),decimal_bcd_code(PrivatertcTD.Minutes));
				u8g2_DrawStr(&u8g2,30,55,Time);
				break;
			case 2:
				u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
				u8g2_DrawStr(&u8g2,30,20,"Config Hours");
				u8g2_SetFont(&u8g2,u8g2_font_logisoso28_tn);
				sprintf(Time,"%d:%d",decimal_bcd_code(PrivatertcTD.Hours),decimal_bcd_code(PrivatertcTD.Minutes));
				u8g2_DrawStr(&u8g2,30,55,Time);
				break;
			case 3:
				u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
				u8g2_DrawStr(&u8g2,30,20,"Config Month");
				u8g2_SetFont(&u8g2,u8g2_font_logisoso28_tn);
				sprintf(Date,"%d/%d",decimal_bcd_code(PrivatertcDD.Month),decimal_bcd_code(PrivatertcDD.Date));
				u8g2_DrawStr(&u8g2,30,55,Date);
			case 4:
				u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
				u8g2_DrawStr(&u8g2,30,20,"Config Date");
				u8g2_SetFont(&u8g2,u8g2_font_logisoso28_tn);
				sprintf(Date,"%d/%d",decimal_bcd_code(PrivatertcDD.Month),decimal_bcd_code(PrivatertcDD.Date));
				u8g2_DrawStr(&u8g2,30,55,Date);
			default:
				break;
			}
			
		}
//		u8g2_SetFont(&u8g2,u8g2_font_5x7_tf);
//		sprintf(TestData,"%d",ConfigStatus[1]);
//		u8g2_DrawStr(&u8g2,30,10,TestData);
		u8g2_SendBuffer(&u8g2);
	}
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM10)
	{
		//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	}
	if(htim->Instance==TIM11)
	{
		
	}
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		HAL_Delay(100);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
