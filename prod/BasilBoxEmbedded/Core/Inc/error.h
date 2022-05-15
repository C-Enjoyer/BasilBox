/*
 * error.h
 *
 *  Created on: 15.05.2022
 *      Author: FloMo
 */

#ifndef INC_ERROR_H_
#define INC_ERROR_H_

#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"

#define ERROR_SAVE_MAX	(20)

typedef enum error_code
{
	error_none,								//no error, just for test reasons
	error_log_full,							//error_log is full
	error_com_message_too_long,				//input message is longer than expected
	error_com_message_decode_failed,		//couldn't decode input message

	error_max
}error_code_t;

typedef enum error_handling
{
	error_void,
	error_soft,
	error_hard,

	error_handling_max
}error_handling_t;

typedef struct error_log
{
	error_code_t code;
	rtc_ts_t ts;

}error_log_t;

void error_handle(error_code_t code, error_handling_t handling);
error_log_t* error_getLog(uint32_t* num);
void error_resetLog(void);

#endif /* INC_ERROR_H_ */
