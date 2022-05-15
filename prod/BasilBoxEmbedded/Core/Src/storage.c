/*
 * storage.c
 *
 *  Created on: 15.05.2022
 *      Author: FloMo
 */

#include "storage.h"
#include "sd.h"
#include "error.h"

#include <string.h>
#include <stdio.h>

bool _storage_encodeLine(char* line, storage_type_t type, void *value);
bool _storage_encodeError(char* line, storage_type_t type, void *value);

#define X(type, filename) filename,
char const *storage_filename[] = { storage_types };
#undef X

char const storage_seperation[] = " ; ";

char storage_ts[RTC_TS_STRING_SIZE];
char storage_line[STORAGE_MAX_LINE_LEN + 1];
FIL storage_file;
bool storage_fileOpened = false;


void storage_save(storage_type_t type, void *value)
{
	if(type >= storage_type_max)
	{
		error_handle(error_storage_invalid_type, error_soft);
		return;
	}

	if(!_storage_encodeLine(storage_line, type, value))
	{
		error_handle(error_storage_line_too_long_encode, error_soft);
		return;
	}

	if(storage_fileOpened)
	{
		error_handle(error_storage_file_already_openend, error_soft);
		return;
	}

	if(!sd_fOpen(&storage_file, storage_filename[type], FA_OPEN_APPEND | FA_WRITE))
	{
		return;
	}

	storage_fileOpened = true;

	if(!sd_fWrite(&storage_file, storage_line))
	{
		return;
	}

	if(!sd_fClose(&storage_file))
	{
		return;
	}

	storage_fileOpened = false;
}

bool _storage_encodeLine(char* line, storage_type_t type, void *value)
{
	bool ret = false;
	storage_line[0] = '\0';

	switch(type)
	{
		case storage_error: ret = _storage_encodeError(line, type, value);
		break;
		case storage_filter_fan_state:
		break;
		case storage_led_light_brightness:
		break;
		default: ret = false;
		break;
	}

	const char endLine[] = "\r\n";

	if(strlen(storage_line) + strlen(endLine) > STORAGE_MAX_LINE_LEN)
	{
		ret = false;
	}
	else
	{
		strcat(storage_line, endLine);
	}

	return ret;
}

bool _storage_encodeError(char* line, storage_type_t type, void *value)
{
	error_log_t error = *(error_log_t*) value;

	rtc_tsToString(storage_ts, error.ts);

	int ret = snprintf(line, STORAGE_MAX_LINE_LEN + 1, "%s%s%d", storage_ts, storage_seperation, error.code);

	if(ret >= STORAGE_MAX_LINE_LEN + 1 || ret < 0)
	{
		error_handle(error_storage_error_too_long_encode, error_soft);
		return false;
	}

	return true;
}
