/*
 * button.c
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#include "button.h"
#include "gpio.h"

#define BUTTON_PORT 	B1_GPIO_Port
#define BUTTON_PIN 	B1_Pin

void button_exti(uint16_t pin)
{
	if(pin != BUTTON_PIN)
	{
		return;
	}


}
