/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "error.h"
#include "led_light.h"
#include <string.h>
#include <stdio.h>

bool _rtc_isTime(rtc_time_t time);
bool _rtc_isDate(rtc_date_t date);
bool _rtc_isTs(rtc_ts_t ts);
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 23;
  sAlarm.AlarmTime.Minutes = 59;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
	rtc_stopAlarm(RTC_ALARM_A);
  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void rtc_setTime(rtc_time_t time)
{
	if(!_rtc_isTime(time))
	{
		return;
	}

	RTC_TimeTypeDef rtcTime;
	rtcTime.Hours = time.hour;
	rtcTime.Minutes = time.min;
	rtcTime.Seconds = time.sec;
	rtcTime.SubSeconds = 0;
	rtcTime.SecondFraction = 0;
	rtcTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	rtcTime.StoreOperation = RTC_STOREOPERATION_SET;

	rtcTime.TimeFormat = RTC_HOURFORMAT_24;

	if(HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		error_handle(error_rtc_cannot_set_time, error_soft);
	}
}

void rtc_getTime(rtc_time_t* time)
{
	RTC_TimeTypeDef rtcTime;

	if (HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		error_handle(error_rtc_cannot_get_time, error_soft);
		return;
	}

	time->hour = rtcTime.Hours;
	time->min = rtcTime.Minutes;
	time->sec = rtcTime.Seconds;
}

void rtc_setDate(rtc_date_t date)
{
	if(!_rtc_isDate(date))
	{
		return;
	}

	RTC_DateTypeDef rtcDate;
	rtcDate.Date = date.day;
	rtcDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	rtcDate.Month = date.month;
	rtcDate.Year = date.year;

	if(HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		error_handle(error_rtc_cannot_set_date, error_soft);
	}
}

void rtc_getDate(rtc_date_t* date)
{
	RTC_DateTypeDef rtcDate;

	if (HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		error_handle(error_rtc_cannot_get_date, error_soft);
		return;
	}

	date->day = rtcDate.Date;
	date->month = rtcDate.Month;
	date->year = rtcDate.Year;
}

void rtc_setTs(rtc_ts_t ts)
{
	rtc_setTime(ts.time);
	rtc_setDate(ts.date);
}

void rtc_getTs(rtc_ts_t* ts)
{
	rtc_getTime(&ts->time);
	rtc_getDate(&ts->date);
}

// time ... either RTC_ALARM_A or RTC_ALARM_B
void rtc_setTimeAlarm(rtc_time_t time, uint32_t alarm)
{
	if (!_rtc_isTime(time))
	{
		return;
	}
	
	RTC_AlarmTypeDef sAlarm = { 0 };
	
	sAlarm.AlarmTime.Hours = time.hour;
	sAlarm.AlarmTime.Minutes = time.min;
	sAlarm.AlarmTime.Seconds = time.sec;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = alarm;
	
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		error_handle(error_rtc_cannot_set_alarm, error_soft);
	}
}

void rtc_stopAlarm(uint32_t alarm)
{
	if (HAL_RTC_DeactivateAlarm(&hrtc, alarm) != HAL_OK)
	{
		error_handle(error_rtc_cannot_stop_alarm, error_soft);
	}
}

void rtc_getTsAsString(char* string)
{
	rtc_ts_t ts;
	rtc_getTs(&ts);

	rtc_tsToString(string, ts);
}

bool rtc_tsToString(char* string, rtc_ts_t ts)
{
	string[0] = '\0';

	if(!_rtc_isTs(ts))
	{
		return false;
	}

	int ret = snprintf(string, RTC_TS_STRING_SIZE, "%02d:%02d:%02d %02d.%02d.%02d", ts.time.hour, ts.time.min, ts.time.sec, ts.date.day, ts.date.month, ts.date.year);

	if(ret >= RTC_TS_STRING_SIZE || ret < 0)
	{
		string[0] = '\0';
		error_handle(error_rtc_encode_string_too_long, error_soft);
		return false;
	}

	return true;
}

bool rtc_timeToString(char* string, rtc_time_t time)
{
	string[0] = '\0';

	if(!_rtc_isTime(time))
	{
		return false;
	}

	int ret = snprintf(string, RTC_TS_STRING_SIZE, "%02d:%02d:%02d", time.hour, time.min, time.sec);

	if(ret >= RTC_TS_STRING_SIZE || ret < 0)
	{
		string[0] = '\0';
		error_handle(error_rtc_encode_string_too_long, error_soft);
		return false;
	}

	return true;
}

bool rtc_timeFromString(rtc_time_t* time, char* string)
{
	int ret = sscanf(string, "%02d:%02d:%02d", (int*) &(time->hour), (int*) &(time->min), (int*) &(time->sec));

	if(ret < 3 || !_rtc_isTime(*time)) // 3 parameters to read
	{
		time->hour = 0;
		time->min = 0;
		time->sec = 0;
		error_handle(error_rtc_cannot_decode_time, error_soft);
		return false;
	}

	return true;
}

bool _rtc_isTime(rtc_time_t time)
{
	if (!IS_RTC_HOUR24(time.hour) || !IS_RTC_MINUTES(time.min) || !IS_RTC_SECONDS(time.sec))
	{
		error_handle(error_rtc_invalid_time_format, error_soft);
		return false;
	}

	return true;
}

bool _rtc_isDate(rtc_date_t date)
{
	if (!IS_RTC_YEAR(date.year) || !IS_RTC_MONTH(date.month) || !IS_RTC_DATE(date.day))
	{
		error_handle(error_rtc_invalid_date_format, error_soft);
		return false;
	}

	return true;
}

bool _rtc_isTs(rtc_ts_t ts)
{
	if(!_rtc_isTime(ts.time) || !_rtc_isDate(ts.date))
	{
		return false;
	}

	return true;
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	ledLight_rtcAlarm();
}

/* USER CODE END 1 */
