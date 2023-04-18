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

#define _FILTER_FAN_TOP_PORT	FILTER_FAN_TOP_GPIO_Port
#define _FILTER_FAN_TOP_PIN		FILTER_FAN_TOP_Pin
#define _FILTER_FAN_BOT_PORT	FILTER_FAN_BOT_GPIO_Port
#define _FILTER_FAN_BOT_PIN		FILTER_FAN_BOT_Pin

#define _FILTER_FAN_DEFAULT_SETTINGS		{ { filterFan_lvl1, 5 * 60 }, { filterFan_off, 115 * 60 } }

void _filterFan_refreshSettings(void);

filterFan_state_t filterFan_state = filterFan_off;

const bool filterFan_topPinLookUp[filterFan_max] = {[filterFan_off] = false, [filterFan_lvl1] = true, [filterFan_lvl2] = true};
const bool filterFan_botPinLookUp[filterFan_max] = {[filterFan_off] = false, [filterFan_lvl1] = false, [filterFan_lvl2] = true};

const filterFan_storageStruct_t filterFan_defaultSettings = _FILTER_FAN_DEFAULT_SETTINGS;
filterFan_storageStruct_t filterFan_settings = _FILTER_FAN_DEFAULT_SETTINGS;

void filterFan_init(void)
{
	_filterFan_refreshSettings();
}

void filterFan_mainLoop(void)
{
	
}

void filterFan_timer1ms(void)
{
	
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
