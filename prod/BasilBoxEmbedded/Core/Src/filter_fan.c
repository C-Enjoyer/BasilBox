/*
 * filter_fan.c
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#include "filter_fan.h"
#include "gpio.h"
#include "error.h"
#include "storage.h"
#include "counter.h"

#define _FILTER_FAN_TOP_PORT	FILTER_FAN_TOP_GPIO_Port
#define _FILTER_FAN_TOP_PIN		FILTER_FAN_TOP_Pin
#define _FILTER_FAN_BOT_PORT	FILTER_FAN_BOT_GPIO_Port
#define _FILTER_FAN_BOT_PIN		FILTER_FAN_BOT_Pin

#define _FILTER_FAN_DEFAULT_SETTINGS		{ { filterFan_lvl1, 5 * 60 }, { filterFan_off, 115 * 60 } }

void _filterFan_checkCounterCallback(void);
void _filterFan_counterCallback(void);
void _filterFan_startInterval(filterFan_interval_t interval);
void _filterFan_restart(void);
void _filterFan_start(void);
void _filterFan_stop(void);
void _filterFan_refreshSettings(void);

filterFan_state_t filterFan_state = filterFan_off;

const bool filterFan_topPinLookUp[filterFan_max] = { [filterFan_off] = false, [filterFan_lvl1] = true, [filterFan_lvl2] = true };
const bool filterFan_botPinLookUp[filterFan_max] = { [filterFan_off] = false, [filterFan_lvl1] = false, [filterFan_lvl2] = true };

const filterFan_storageStruct_t filterFan_defaultSettings = _FILTER_FAN_DEFAULT_SETTINGS;
filterFan_storageStruct_t filterFan_settings = _FILTER_FAN_DEFAULT_SETTINGS;

counter_t filterFan_counter;
bool filterFan_isOffInterval = false;
bool filterFan_gotCounterCallback = false;

void filterFan_init(void)
{
	_filterFan_restart();
}

void filterFan_mainLoop(void)
{
	_filterFan_checkCounterCallback();
}

void filterFan_timer1ms(void)
{
	counter_tick(&filterFan_counter);
}

void filterFan_set(filterFan_state_t newState)
{
	if(newState >= filterFan_max)
	{
		error_handle(error_filter_fan_invalid_type, error_soft);
		return;
	}
	
	HAL_GPIO_WritePin(_FILTER_FAN_TOP_PORT, _FILTER_FAN_TOP_PIN, gpio_getPinState(filterFan_topPinLookUp[newState]));
	HAL_GPIO_WritePin(_FILTER_FAN_BOT_PORT, _FILTER_FAN_BOT_PIN, gpio_getPinState(filterFan_botPinLookUp[newState]));
}

void _filterFan_checkCounterCallback(void)
{
	if (!filterFan_gotCounterCallback)
	{
		return;
	}
	
	filterFan_gotCounterCallback = false;
		
	filterFan_isOffInterval = !filterFan_isOffInterval;
	
	if (filterFan_isOffInterval)
	{
		_filterFan_startInterval(filterFan_settings.offInterval);
	}
	else
	{
		_filterFan_startInterval(filterFan_settings.onInterval);
	}
}

void _filterFan_counterCallback(void)
{
	filterFan_gotCounterCallback = true;
}

void _filterFan_startInterval(filterFan_interval_t interval)
{
	filterFan_set(interval.state);
	counter_start(&filterFan_counter, interval.time * 1000);	// convert [s] to [ms]
}

void _filterFan_restart(void)
{
	_filterFan_refreshSettings();
	_filterFan_start();
}

void _filterFan_start(void)
{
	counter_initialize(&filterFan_counter);
	counter_setAutoReload(&filterFan_counter, false);
	counter_setCallback(&filterFan_counter, _filterFan_counterCallback);
	
	filterFan_isOffInterval = false;
	_filterFan_startInterval(filterFan_settings.onInterval);
}

void _filterFan_stop(void)
{
	counter_stop(&filterFan_counter);
	
	filterFan_gotCounterCallback = false;
	filterFan_isOffInterval = false;
	
	filterFan_set(filterFan_off);
}

void _filterFan_refreshSettings(void)
{
	if (storage_read(storage_filterFan, &filterFan_settings))
	{
		return;	//success
	}
	
	// cannot read file => replace settings with default settings
	filterFan_settings = filterFan_defaultSettings;
	storage_save(storage_filterFan, &filterFan_settings);
}
