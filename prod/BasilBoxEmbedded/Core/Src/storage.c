/*
 * storage.c
 *
 *  Created on: 15.05.2022
 *      Author: FloMo
 */

#include "storage.h"
#include "error.h"
#include "filter_fan.h"
#include "led_light.h"
#include "rtc.h"

#include <string.h>
#include <stdio.h>
#include <inttypes.h>

bool _storage_encodeLine(char* line, storage_type_t type, void *value);
bool _storage_encodeError(char* line, storage_type_t type, void *value);
bool _storage_encodeFilterFanSettings(char* line, storage_type_t type, void *value);
bool _storage_encodeLedLightSettings(char* line, storage_type_t type, void *value);
bool _storage_decodeLine(char* line, storage_type_t type, void *value);
bool _storage_decodeFilterFanSettings(char* line, storage_type_t type, void *value);
bool _storage_decodeLedLightSettings(char* line, storage_type_t type, void *value);
bool _storage_decodeRtcSettings(char* line, storage_type_t type, void *value);
char* _storage_getFormatString(const char* format);

#define X(type, filename, openFlags) filename,
const TCHAR* storage_filename[] = { storage_types };
#undef X

#define X(type, filename, openFlags) openFlags,
BYTE storage_openFlags[] = { storage_types };
#undef X

char const storage_seperation[] = " ; ";

char storage_ts[2][RTC_TS_STRING_SIZE];
char storage_line[STORAGE_MAX_LINE_LEN + 1];
FIL storage_file;
bool storage_fileOpened = false;

char storage_formatString[STORAGE_MAX_FORMAT_STRING_LEN + 1];


bool storage_save(storage_type_t type, void *value)
{
	if(type >= storage_type_max)
	{
		error_handle(error_storage_invalid_type, error_soft);
		return false;
	}

	if(!_storage_encodeLine(storage_line, type, value))
	{
		error_handle(error_storage_line_too_long_encode, error_soft);
		return false;
	}

	if(storage_fileOpened)
	{
		error_handle(error_storage_file_already_openend, error_soft);
		return false;
	}

	if(!sd_fOpen(&storage_file, storage_filename[type], storage_openFlags[type]))
	{
		return false;
	}

	storage_fileOpened = true;

	if(!sd_fWrite(&storage_file, storage_line))
	{
		return false;
	}

	if(!sd_fTruncate(&storage_file))
	{
		return false;
	}

	if(!sd_fClose(&storage_file))
	{
		return false;
	}

	storage_fileOpened = false;
	
	return true;
}

bool storage_read(storage_type_t type, void *value)
{
	if(type >= storage_type_max)
	{
		error_handle(error_storage_invalid_type, error_soft);
		return false;
	}

	if(storage_fileOpened)
	{
		error_handle(error_storage_file_already_openend, error_soft);
		return false;
	}

	if(!sd_fOpen(&storage_file, storage_filename[type], FA_READ))
	{
		return false;
	}

	storage_fileOpened = true;

	if(!sd_fGets(&storage_file, storage_line, STORAGE_MAX_LINE_LEN + 1))
	{
		return false;
	}

	if(!_storage_decodeLine(storage_line, type, value))
	{
		return false;
	}

	if(!sd_fClose(&storage_file))
	{
		return false;
	}

	storage_fileOpened = false;

	return true;
}

bool storage_delete(storage_type_t type)
{
	if (type >= storage_type_max)
	{
		error_handle(error_storage_invalid_type, error_soft);
		return false;
	}

	if (storage_fileOpened)
	{
		error_handle(error_storage_file_already_openend, error_soft);
		return false;
	}
	
	if (!sd_fDelete(storage_filename[type]))
	{
		return false;	
	}
	
	return true;
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
		case storage_ledLight: ret = _storage_encodeLedLightSettings(line, type, value);
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

	rtc_tsToString(storage_ts[0], error.ts);

	int ret = snprintf(line, STORAGE_MAX_LINE_LEN + 1, _storage_getFormatString("%s%d"), storage_ts[0], error.code);

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

	int ret = snprintf(line, STORAGE_MAX_LINE_LEN + 1, _storage_getFormatString("%d%"PRIu32"%d%"PRIu32),
			settings.onInterval.state,
			settings.onInterval.time,
			settings.offInterval.state,
			settings.offInterval.time);

	if(ret >= STORAGE_MAX_LINE_LEN + 1 || ret < 0)
	{
		error_handle(error_storage_filterFan_too_long_encode, error_soft);
		return false;
	}

	return true;
}

bool _storage_encodeLedLightSettings(char* line, storage_type_t type, void *value)
{
	ledLight_storageStruct_t settings = *(ledLight_storageStruct_t*) value;

	rtc_timeToString(storage_ts[0], settings.onInterval.dayTime);
	rtc_timeToString(storage_ts[1], settings.offInterval.dayTime);

	int ret = snprintf(line, STORAGE_MAX_LINE_LEN + 1, _storage_getFormatString("%lf%s%lf%s"),
			settings.onInterval.percent,
			storage_ts[0],
			settings.offInterval.percent,
			storage_ts[1]);

	if(ret >= STORAGE_MAX_LINE_LEN + 1 || ret < 0)
	{
		error_handle(error_storage_ledLight_too_long_encode, error_soft);
		return false;
	}

	return true;
}

bool _storage_decodeLine(char* line, storage_type_t type, void *value)
{
	bool ret = false;

	switch(type)
	{
		case storage_filterFan: ret = _storage_decodeFilterFanSettings(line, type, value);
		break;
		case storage_ledLight: ret = _storage_decodeLedLightSettings(line, type, value);
		break;
		case storage_rtc: ret = _storage_decodeRtcSettings(line, type, value);
		break;
		default: ret = false;
		break;
	}

	return ret;
}

bool _storage_decodeFilterFanSettings(char* line, storage_type_t type, void *value)
{
	filterFan_storageStruct_t* settings = (filterFan_storageStruct_t*) value;

	int ret = sscanf(line, _storage_getFormatString("%d%"PRIu32"%d%"PRIu32),
			&(settings->onInterval.state),
			&(settings->onInterval.time),
			&(settings->offInterval.state),
			&(settings->offInterval.time));

	if(ret < 4) // 4 parameters to read
	{
		error_handle(error_storage_filterFan_cannot_decode, error_soft);
		return false;
	}

	return true;
}

bool _storage_decodeLedLightSettings(char* line, storage_type_t type, void *value)
{
	ledLight_storageStruct_t* settings = (ledLight_storageStruct_t*) value;

	int ret = sscanf(line, _storage_getFormatString("%lf%s%lf%s"),
			&(settings->onInterval.percent),
			storage_ts[0],
			&(settings->offInterval.percent),
			storage_ts[1]);

	if(ret < 4 || // 4 parameters to read
			!rtc_timeFromString(&(settings->onInterval.dayTime), storage_ts[0]) ||
			!rtc_timeFromString(&(settings->offInterval.dayTime), storage_ts[1]))
	{
		error_handle(error_storage_ledLight_cannot_decode, error_soft);
		return false;
	}

	return true;
}

bool _storage_decodeRtcSettings(char* line, storage_type_t type, void *value)
{
	rtc_storageStruct_t* settings = (rtc_storageStruct_t*) value;

	int ret = sscanf(line, _storage_getFormatString("%[^\n]"), storage_ts[0]);

	if (ret < 1 || // 1 parameter to read
			!rtc_tsFromString(&(settings->ts), storage_ts[0]))
	{
		error_handle(error_storage_rtc_cannot_decode, error_soft);
		return false;
	}

	return true;
}

char* _storage_getFormatString(const char* format)
{
	storage_formatString[0] = '\0';

	size_t formatLen = strlen(format);
	size_t sepLen = strlen(storage_seperation);
	size_t curChar = 0;

	bool foundFirst = false;

	for(size_t i = 0; i < formatLen; i++)
	{
		if(format[i] == '%')
		{
			if(foundFirst)
			{
				if(curChar + sepLen > STORAGE_MAX_FORMAT_STRING_LEN)
				{
					error_handle(error_storage_format_too_long, error_soft);
					return NULL;
				}

				strcat(storage_formatString, storage_seperation);
				curChar += sepLen;
			}
			else
			{
				foundFirst = true;
			}
		}

		if(curChar + 1 > STORAGE_MAX_FORMAT_STRING_LEN)
		{
			error_handle(error_storage_format_too_long, error_soft);
			return NULL;
		}

		storage_formatString[curChar] = format[i];
		curChar++;
		storage_formatString[curChar] = '\0';

	}

	return storage_formatString;
}
