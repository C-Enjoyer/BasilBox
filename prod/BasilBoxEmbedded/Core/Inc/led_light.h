/*
 * led_light.h
 *
 *  Created on: 12.03.2022
 *      Author: FloMo
 */

#ifndef INC_LED_LIGHT_H_
#define INC_LED_LIGHT_H_

#include <stdint.h>
#include "rtc.h"

typedef struct ledLight_interval
{
	double percent;
	rtc_time_t dayTime; //end time

} ledLight_interval_t;

typedef struct ledLight_storageStruct
{
	ledLight_interval_t onInterval;
	ledLight_interval_t offInterval;

} ledLight_storageStruct_t;

void ledLight_init(void);
void ledLight_mainLoop(void);
void ledLight_setBrightness(double percent);
void ledLight_rtcTimeChanged(void);
void ledLight_rtcAlarm(void);

#endif /* INC_LED_LIGHT_H_ */
