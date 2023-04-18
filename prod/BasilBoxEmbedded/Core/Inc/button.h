/*
 * button.h
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum button_event
{
	button_none,
	button_released,
	button_pressed,

	button_event_max
}button_event_t;

void button_init(void);
void button_eventCallback(button_event_t event);
void button_exti(uint16_t pin);
void button_timer_1ms(void);



#endif /* INC_BUTTON_H_ */
