/*
 * volreg.c
 *
 *  Created on: 12.03.2022
 *      Author: FloMo
 */

#include "volreg.h"
#include "gpio.h"
#include "tim.h"

#define VOLREG_TIM		htim3
#define VOLREG_CHANNEL	TIM_CHANNEL_2

void _volreg_setPwm(bool on);

uint32_t volreg_arr = 0;
bool volreg_pwmOn = false;

void volreg_init(void)
{
	volreg_arr = __HAL_TIM_GET_AUTORELOAD(&VOLREG_TIM) + 1;
	volreg_setRelative(0);
}

void volreg_setRelative(double percent)
{
	percent = (percent > 1) ? (1) : (percent);
	percent = (percent < 0) ? (0) : (percent);

	uint32_t comp = volreg_arr * percent;
	comp = (comp == 0) ? (0) : (comp - 1);

	_volreg_setPwm(true);
	__HAL_TIM_SET_COMPARE(&VOLREG_TIM, VOLREG_CHANNEL, comp);
}

void _volreg_setPwm(bool on)
{
	if(on && !volreg_pwmOn)
	{
		HAL_TIM_PWM_Start(&VOLREG_TIM, VOLREG_CHANNEL);
		volreg_pwmOn = true;
	}
	else if(!on && volreg_pwmOn)
	{
		HAL_TIM_PWM_Stop(&VOLREG_TIM, VOLREG_CHANNEL);
		volreg_pwmOn = false;
	}
}
