/*
 * debug.c
 *
 *  Created on: 17.04.2023
 *      Author: FloMo
 */

#include "debug.h"
#include "button.h"
#include "filter_fan.h"
#include "led_light.h"
#include "led.h"
#include "storage.h"

#define DEBUG_BUTTON	// define this to handle the button in debug.c (could result in compile errors if callback is uses elsewhere)

#ifdef DEBUG_BUTTON
void _debug_handleButton(button_event_t event);
void _debug_checkButton(void);
#endif

#ifdef DEBUG_BUTTON
bool debug_eventPending[button_event_max] = { false };
#endif

void debug_init(void)
{

}

void debug_mainLoop(void)
{
#ifdef DEBUG_BUTTON
	_debug_checkButton();
#endif
}

#ifdef DEBUG_BUTTON
void _debug_handleButton(button_event_t event)
{
	if(event == button_released)
	{

	}
	else if(event == button_pressed)
	{

	}
}

void _debug_checkButton(void)
{
	for(button_event_t event = 0; event < button_event_max; event++)
	{
		if(debug_eventPending[event])
		{
			debug_eventPending[event] = false;
			_debug_handleButton(event);
		}
	}
}

void button_eventCallback(button_event_t event)
{
	debug_eventPending[event] = true;
}
#endif
