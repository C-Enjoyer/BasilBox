/*
 * led_light.c
 *
 *  Created on: 12.03.2022
 *      Author: FloMo
 */

#include "led_light.h"
#include "volreg.h"
#include "storage.h"
#include "rtc.h"

#include <stdbool.h>

#define _LED_LIGHT_MIN		0.16
#define _LED_LIGHT_MAX		1.00

#define _LED_LIGHT_K		(_LED_LIGHT_MAX - _LED_LIGHT_MIN)
#define _LED_LIGHT_D		(_LED_LIGHT_MIN)

#define _LED_LIGHT_ALARM	(RTC_ALARM_A)
 
#define _LED_LIGHT_DEFAULT_SETTINGS		{ { 0.75, { 21, 0, 0 } }, { 0, { 3, 0, 0 } } }

double _ledLight_convertLinear(double percent);
void _ledLight_checkAlarm(void);
void _ledLight_startInterval(ledLight_interval_t interval);
void _ledLight_restart(void);
void _ledLight_start(void);
void _ledLight_stop(void);
void _ledLight_refreshSettings(void);

const ledLight_storageStruct_t ledLight_defaultSettings = _LED_LIGHT_DEFAULT_SETTINGS;
ledLight_storageStruct_t ledLight_settings = _LED_LIGHT_DEFAULT_SETTINGS;

bool ledLight_isOffInterval = false;
bool ledLight_gotRtcAlarm = false;

bool ledLight_isInitialized = false;

void ledLight_init(void)
{
	if (ledLight_isInitialized)
	{
		return;	
	}
	
	volreg_init();
	_ledLight_restart();
	
	ledLight_isInitialized = true;
}

void ledLight_mainLoop(void)
{
	_ledLight_checkAlarm();
}

void ledLight_setBrightness(double percent)
{
	volreg_setRelative(_ledLight_convertLinear(percent));
}

double _ledLight_convertLinear(double percent)
{
	return (percent == 0) ? (0) : (percent * _LED_LIGHT_K + _LED_LIGHT_D);
}

void _ledLight_checkAlarm(void)
{
	if (!ledLight_gotRtcAlarm)
	{
		return;
	}
	
	ledLight_gotRtcAlarm = false;
		
	ledLight_isOffInterval = !ledLight_isOffInterval;
	
	if (ledLight_isOffInterval)
	{
		_ledLight_startInterval(ledLight_settings.offInterval);
	}
	else
	{
		_ledLight_startInterval(ledLight_settings.onInterval);
	}
}

void ledLight_rtcTimeChanged(void)
{
	if (!ledLight_isInitialized)
	{
		return;
	}

	_ledLight_stop();
	_ledLight_start();
}

void ledLight_rtcAlarm(void)
{
	ledLight_gotRtcAlarm = true;
}

void _ledLight_startInterval(ledLight_interval_t interval)
{
	ledLight_setBrightness(interval.percent);
	rtc_setTimeAlarm(interval.dayTime, _LED_LIGHT_ALARM);
}

void _ledLight_restart(void)
{
	_ledLight_refreshSettings();
	_ledLight_start();
}

void _ledLight_start(void)
{
	rtc_time_t now;
	rtc_getTime(&now);
	
	if (rtc_isTimeInBetween(ledLight_settings.offInterval.dayTime, ledLight_settings.onInterval.dayTime, now))
	{
		ledLight_isOffInterval = false;
		_ledLight_startInterval(ledLight_settings.onInterval);
	}
	else
	{
		ledLight_isOffInterval = true;
		_ledLight_startInterval(ledLight_settings.offInterval);
	}
}

void _ledLight_stop(void)
{
	rtc_stopAlarm(_LED_LIGHT_ALARM);
	
	ledLight_gotRtcAlarm = false;
	ledLight_isOffInterval = false;
	
	ledLight_setBrightness(0);
}

void _ledLight_refreshSettings(void)
{
	if (storage_read(storage_ledLight, &ledLight_settings))
	{
		return;	//success
	}
	
	// cannot read file => replace settings with default settings
	ledLight_settings = ledLight_defaultSettings;
	storage_save(storage_ledLight, &ledLight_settings);
}
