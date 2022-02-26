/*
 * led.h
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdint.h>
#include <stdbool.h>

void led_set(bool on);
void led_blink(uint32_t on, uint32_t off);
void led_nblink(uint32_t on, uint32_t off, uint32_t n);
void led_flash(uint32_t on);
void led_timer_1ms(void);

#endif /* INC_LED_H_ */
