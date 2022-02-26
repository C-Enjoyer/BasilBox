/*
 * led.c
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */
#include <limits.h>

#include "led.h"
#include "gpio.h"
#include "tim.h"

#define LED_PORT	LD2_GPIO_Port
#define LED_PIN	LD2_Pin

void _led_set(bool on);
void _led_resetParams(void);

tim_scal_t led_onScal = {0};
tim_scal_t led_offScal = {0};
uint32_t led_onTime = 0;
uint32_t led_offTime = 0;

uint32_t led_nblinkCnt = 0;
bool led_on = false;
bool led_isFlash = false;


void led_set(bool on)
{
	_led_resetParams();
	_led_set(on);
}

void led_blink(uint32_t on, uint32_t off)
{
	_led_resetParams();
	led_nblinkCnt = UINT32_MAX;
	led_onTime = on;
	led_offTime = off;
	_led_set(true);
	tim_scalStart(&led_onScal, led_onTime);
}
void led_nblink(uint32_t on, uint32_t off, uint32_t n)
{
	_led_resetParams();
	if(n == 0)
	{
		return;
	}
	led_nblinkCnt = n;
	led_onTime = on;
	led_offTime = off;
	_led_set(true);
	tim_scalStart(&led_onScal, led_onTime);
}

void led_flash(uint32_t on)
{
	_led_resetParams();
	led_isFlash = true;
	led_onTime = on;
	_led_set(true);
	tim_scalStart(&led_onScal, led_onTime);
}

void _led_set(bool on)
{
	HAL_GPIO_WritePin(LED_PORT, LED_PIN, gpio_getPinState(on));
	led_on = on;
}

void _led_resetParams(void)
{
	tim_scalStop(&led_onScal);
	tim_scalStop(&led_offScal);

	led_onTime = 0;
	led_offTime = 0;

	led_nblinkCnt = 0;
	led_isFlash = false;
}

void led_timer_1ms(void)
{
	if(tim_scalCount(&led_onScal))
	{
		_led_set(false);
		tim_scalStop(&led_onScal);
		if(led_isFlash)
		{
			_led_resetParams();
		}
		else
		{
			tim_scalStart(&led_offScal, led_offTime);
		}
	}
	if(tim_scalCount(&led_offScal))
	{
		tim_scalStop(&led_offScal);
		if(led_nblinkCnt != UINT32_MAX)
		{
			led_nblinkCnt--;
		}

		if(led_nblinkCnt != 0)
		{
			_led_set(true);
			tim_scalStart(&led_onScal, led_onTime);
		}
		else
		{
			_led_set(false);
			_led_resetParams();
		}
	}
}
