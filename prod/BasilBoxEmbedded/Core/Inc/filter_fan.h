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
} filterFan_state_t;

typedef struct filterFan_interval
{
	filterFan_state_t state;
	uint32_t time;	// [s]

} filterFan_interval_t;

typedef struct filterFan_storageStruct
{
	filterFan_interval_t onInterval;
	filterFan_interval_t offInterval;

} filterFan_storageStruct_t;

void filterFan_init(void);
void filterFan_mainLoop(void);
void filterFan_timer1ms(void);
void filterFan_set(filterFan_state_t newState);

#endif /* INC_FILTER_FAN_H_ */
