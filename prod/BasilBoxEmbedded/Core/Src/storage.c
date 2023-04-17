/*
 * storage.c
 *
 *  Created on: 15.05.2022
 *      Author: FloMo
 */

#include "storage.h"
#include "error.h"
#include "filter_fan.h"

#include <string.h>
#include <stdio.h>
#include <inttypes.h>

bool _storage_encodeLine(char* line, storage_type_t type, void *value);
bool _storage_encodeError(char* line, storage_type_t type, void *value);
bool _storage_encodeFilterFanSettings(char* line, storage_type_t type, void *value);

#define X(type, filename, openFlags) filename,
const TCHAR* storage_filename[] = { storage_types };
#undef X

#define X(type, filename, openFlags) openFlags,
BYTE storage_openFlags[] = { storage_types };
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

	if(!sd_fOpen(&storage_file, storage_filename[type], storage_openFlags[type]))
	{
		return;
	}

	storage_fileOpened = true;

	if(!sd_fWrite(&storage_file, storage_line))
	{
		return;
	}

	if(!sd_fTruncate(&storage_file))
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
		case storage_filterFan: ret = _storage_encodeFilterFanSettings(line, type, value);
		break;
		case storage_ledLight:
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

bool _storage_encodeFilterFanSettings(char* line, storage_type_t type, void *value)
{
	filterFan_storageStruct_t settings = *(filterFan_storageStruct_t*) value;

	int ret = snprintf(line, STORAGE_MAX_LINE_LEN + 1, "%d%s%"PRIu32"%s%d%s%"PRIu32"",
			settings.onInterval.state, storage_seperation,
			settings.onInterval.time, storage_seperation,
			settings.offInterval.state, storage_seperation,
			settings.offInterval.time);

	if(ret >= STORAGE_MAX_LINE_LEN + 1 || ret < 0)
	{
		error_handle(error_storage_filterFan_too_long_encode, error_soft);
		return false;
	}

	return true;
}
