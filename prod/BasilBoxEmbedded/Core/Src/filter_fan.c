/*
 * filter_fan.c
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#include "filter_fan.h"
#include "gpio.h"
#include "error.h"

#define FILTER_FAN_TOP_PORT	FILTER_FAN_TOP_GPIO_Port
#define FILTER_FAN_TOP_PIN	FILTER_FAN_TOP_Pin
#define FILTER_FAN_BOT_PORT	FILTER_FAN_BOT_GPIO_Port
#define FILTER_FAN_BOT_PIN	FILTER_FAN_BOT_Pin

filterFan_state_t filterFan_state = filterFan_off;

bool filterFan_topPinLookUp[filterFan_max] = {[filterFan_off] = false, [filterFan_lvl1] = true, [filterFan_lvl2] = true};
bool filterFan_botPinLookUp[filterFan_max] = {[filterFan_off] = false, [filterFan_lvl1] = false, [filterFan_lvl2] = true};

void filterFan_set(filterFan_state_t newState)
{
	if(newState >= filterFan_max)
	{
		error_handle(error_filter_fan_invalid_type, error_soft);
		return;
	}
	HAL_GPIO_WritePin(FILTER_FAN_TOP_PORT, FILTER_FAN_TOP_PIN, gpio_getPinState(filterFan_topPinLookUp[newState]));

	HAL_GPIO_WritePin(FILTER_FAN_BOT_PORT, FILTER_FAN_BOT_PIN, gpio_getPinState(filterFan_botPinLookUp[newState]));
}
