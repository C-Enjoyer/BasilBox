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
	error_main_errorHandle,					//errorhandler in main

	error_com_message_too_long_decode,		//input message is longer than expected
	error_com_message_too_long_encode,		//output message is longer than expected
	error_com_message_decode_failed,		//couldn't decode input message
	error_com_message_encode_failed,		//couldn't encode message
	error_com_unknown_message_decode,		//unknown input message type
	error_com_unknown_message_encode,		//unknown message type for encoding
	error_com_ts_invalid,					//got invalid timestamp from input message

	error_usart_transmit_buffer_full,		//transmit buffer is full, head hitting tail
	error_usart_cannot_convert_buffer,		//cannot convert ringbuf to linear buf, since linear buf is too small

	error_rtc_invalid_time_format,			//tried to set rtc with invalid time format
	error_rtc_cannot_set_time,				//cannot set rtc time
	error_rtc_cannot_get_time,				//cannot get rtc time
	error_rtc_invalid_date_format,			//tried to set rtc with invalid date format
	error_rtc_cannot_set_date,				//cannot set rtc date
	error_rtc_cannot_get_date,				//cannot get rtc date

	error_filter_fan_invalid_type,			//invalid enum used

	error_sd_cannot_mount,					//cannot mount
	error_sd_cannot_unmount,				//cannot unmount
	error_sd_cannot_open_file,				//cannot open file
	error_sd_cannot_write_file,				//cannot write file
	error_sd_cannot_read_file,				//cannot read file
	error_sd_cannot_lseek_file,				//cannot lseek file
	error_sd_cannot_truncate_file,			//cannot truncate file
	error_sd_cannot_delete_file,			//cannot delete file
	error_sd_cannot_close_file,				//cannot close file
	error_sd_cannot_open_dir,				//cannot open dir
	error_sd_cannot_read_dir,				//cannot read dir
	error_sd_cannot_close_dir, 				//cannot close dir


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
