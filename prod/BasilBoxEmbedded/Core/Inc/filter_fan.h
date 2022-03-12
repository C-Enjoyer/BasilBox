/*
 * filter_fan.h
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#ifndef INC_FILTER_FAN_H_
#define INC_FILTER_FAN_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum filter_fan_state
{
	filter_fan_off,
	filter_fan_lvl1,
	filter_fan_lvl2,
	filter_fan_max
}filter_fan_state_t;

bool filter_fan_set(filter_fan_state_t newState);

#endif /* INC_FILTER_FAN_H_ */
