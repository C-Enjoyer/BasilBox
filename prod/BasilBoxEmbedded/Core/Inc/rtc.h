/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */
#define RTC_TS_STRING_SIZE	(17 + 1) //hh:mm:ss DD.MM.YY

typedef struct rtc_time
{
	uint8_t hour, min, sec;
}rtc_time_t;

typedef struct rtc_date
{
	uint8_t day, month, year;
}rtc_date_t;

typedef struct rtc_ts
{
	rtc_time_t time;
	rtc_date_t date;
}rtc_ts_t;
/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
void rtc_setTime(rtc_time_t time);
void rtc_getTime(rtc_time_t* time);
void rtc_setDate(rtc_date_t date);
void rtc_getDate(rtc_date_t* date);
void rtc_setTs(rtc_ts_t ts);
void rtc_getTs(rtc_ts_t* ts);
void rtc_setTimeAlarm(rtc_time_t time, uint32_t alarm);
void rtc_stopAlarm(uint32_t alarm);
void rtc_getTsAsString(char* string);
bool rtc_tsToString(char* string, rtc_ts_t ts);
bool rtc_timeToString(char* string, rtc_time_t time);
bool rtc_timeFromString(rtc_time_t* time, char* string);
bool rtc_isTimeInBetween(rtc_time_t start, rtc_time_t end, rtc_time_t now);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

