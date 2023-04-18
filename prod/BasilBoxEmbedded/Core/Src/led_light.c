/*
 * led_light.c
 *
 *  Created on: 12.03.2022
 *      Author: FloMo
 */

#include "led_light.h"
#include "volreg.h"

#define LED_LIGHT_MIN		0.16
#define LED_LIGHT_MAX		1.00

#define _LED_LIGHT_K		(LED_LIGHT_MAX - LED_LIGHT_MIN)
#define _LED_LIGHT_D		(LED_LIGHT_MIN)

double _ledLight_convertLinear(double percent);

void ledLight_setBrightness(double percent)
{
	volreg_setRelative(_ledLight_convertLinear(percent));
}

double _ledLight_convertLinear(double percent)
{
	return (percent == 0) ? (0) : (percent * _LED_LIGHT_K + _LED_LIGHT_D);
}
