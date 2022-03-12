/*
 * button.c
 *
 *  Created on: 26.02.2022
 *      Author: FloMo
 */

#include "button.h"
#include "gpio.h"
#include "tim.h"
#include "led.h"
#include "filter_fan.h"

#define BUTTON_PORT 	B1_GPIO_Port
#define BUTTON_PIN 	B1_Pin
#define BUTTON_ACTIVE_LOW //comment to use button as active high
#define BUTTON_DEBOUNCE_TIME 20	//[ms] time to debounce button

button_event_t _button_fromNewPinState(GPIO_PinState newState);

button_event_t button_event = button_none;

tim_scal_t button_debounce = {0, 0};

__weak void button_eventCallback(button_event_t event)
{

}

void button_exti(uint16_t pin)
{
	if(pin != BUTTON_PIN) //wrong pin
	{
		return;
	}

	if(button_event != button_none) //event beeing debounced
	{
		return;
	}

	GPIO_PinState newState = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);

	button_event = _button_fromNewPinState(newState);
	tim_scalStart(&button_debounce, BUTTON_DEBOUNCE_TIME);
}

void button_timer_1ms(void)
{
	if(tim_scalCount(&button_debounce))
	{
		tim_scalStop(&button_debounce);

		GPIO_PinState newState = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
		button_event_t newEvent = _button_fromNewPinState(newState);

		if(newEvent == button_event)
		{
			button_eventCallback(button_event);
		}

		button_event = button_none;
	}
}

button_event_t _button_fromNewPinState(GPIO_PinState newState)
{
#ifdef BUTTON_ACTIVE_LOW
	return (newState == GPIO_PIN_SET) ? (button_released) : (button_pressed);
#else
	return (newState == GPIO_PIN_SET) ? (button_pressed) : (button_released);
#endif
}
