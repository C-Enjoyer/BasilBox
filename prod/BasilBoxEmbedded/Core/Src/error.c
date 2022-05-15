/*
 * error.c
 *
 *  Created on: 15.05.2022
 *      Author: FloMo
 */
#include "error.h"

error_log_t error_log[ERROR_SAVE_MAX];
uint32_t error_logNum = 0;

void error_handle(error_code_t code, error_handling_t handling)
{
	if(code >= error_max || handling >= error_handling_max)
	{
		return;
	}

	if(handling == error_void)
	{
		return;
	}

	if(error_logNum < ERROR_SAVE_MAX - 1)
	{
		error_log[error_logNum].code = code;
		rtc_getTs(&error_log[error_logNum].ts);
		error_logNum++;
	}
	else
	{
		error_log[ERROR_SAVE_MAX - 1].code = error_log_full;
		rtc_getTs(&error_log[ERROR_SAVE_MAX - 1].ts);
	}

	if(handling == error_hard)
	{
		__disable_irq();
		while(1)
		{
			asm("nop");
		}
	}
}

error_log_t* error_getLog(uint32_t* num)
{
	*num = error_logNum;
	return error_log;
}

void error_resetLog(void)
{
	error_logNum = 0;
}
