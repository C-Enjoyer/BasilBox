/*
 * filter_fan.c
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#include "filter_fan.h"
#include "gpio.h"

#define FILTER_FAN_TOP_PORT	FILTER_FAN_TOP_GPIO_Port
#define FILTER_FAN_TOP_PIN	FILTER_FAN_TOP_Pin
#define FILTER_FAN_BOT_PORT	FILTER_FAN_BOT_GPIO_Port
#define FILTER_FAN_BOT_PIN	FILTER_FAN_BOT_Pin

filter_fan_state_t filter_fan_state = filter_fan_off;

bool filter_fan_topPinLookUp[filter_fan_max] = {[filter_fan_off] = false, [filter_fan_lvl1] = true, [filter_fan_lvl2] = true};
bool filter_fan_botPinLookUp[filter_fan_max] = {[filter_fan_off] = false, [filter_fan_lvl1] = false, [filter_fan_lvl2] = true};

bool filter_fan_set(filter_fan_state_t newState)
{
	HAL_GPIO_WritePin(FILTER_FAN_TOP_PORT, FILTER_FAN_TOP_PIN, gpio_getPinState(filter_fan_topPinLookUp[newState]));

	HAL_GPIO_WritePin(FILTER_FAN_BOT_PORT, FILTER_FAN_BOT_PIN, gpio_getPinState(filter_fan_botPinLookUp[newState]));
}
