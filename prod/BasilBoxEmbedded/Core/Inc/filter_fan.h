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

typedef enum filterFan_state
{
	filterFan_off,
	filterFan_lvl1,
	filterFan_lvl2,

	filterFan_max
}filterFan_state_t;

void filterFan_set(filterFan_state_t newState);

#endif /* INC_FILTER_FAN_H_ */
