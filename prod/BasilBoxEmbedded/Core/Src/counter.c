#include "counter.h"

#include <stddef.h>

void counter_initialize(counter_t* pCounter)
{
	pCounter->autoReload = false;
	pCounter->callback = NULL;
	pCounter->value = 0;
	pCounter->maximum = 0;
}

void counter_setCallback(counter_t* pCounter, counter_cb_t cb)
{
	pCounter->callback = cb;
}

void counter_clearCallback(counter_t* pCounter)
{
	pCounter->callback = NULL;
}

void counter_setAutoReload(counter_t* pCounter, bool autoReload)
{
	pCounter->autoReload = autoReload;
}

void counter_start(counter_t* pCounter, uint32_t maximum)
{
	pCounter->value = 0;
	pCounter->maximum = maximum;
}

void counter_tick(counter_t* pCounter)
{
	if (!counter_isActive(pCounter))
	{
		return;
	}

	pCounter->value++;

	if (pCounter->value >= pCounter->maximum)
	{
		if (pCounter->callback != NULL)
		{
			pCounter->callback();
		}

		if (pCounter->autoReload)
		{
			pCounter->value = 0;
		}
		else
		{
			counter_stop(pCounter);
		}
	}
}

void counter_stop(counter_t* pCounter)
{
	pCounter->value = 0;
	pCounter->maximum = 0;
}

bool counter_isActive(counter_t* pCounter)
{
	return (pCounter->maximum != 0);
}