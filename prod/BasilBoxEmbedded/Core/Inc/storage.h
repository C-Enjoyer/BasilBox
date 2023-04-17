/*
 * storage.h
 *
 *  Created on: 15.05.2022
 *      Author: FloMo
 */

#ifndef INC_STORAGE_H_
#define INC_STORAGE_H_

#include <stdint.h>
#include <stdbool.h>

#include "sd.h"

#define STORAGE_MAX_LINE_LEN				100
#define STORAGE_MAX_FORMAT_STRING_LEN	(STORAGE_MAX_LINE_LEN / 2)

#define storage_types \
X(storage_error, "Errorlog.txt", FA_OPEN_APPEND | FA_WRITE) \
X(storage_filterFan, "FilterFan.txt", FA_OPEN_ALWAYS | FA_WRITE) \
X(storage_ledLight, "LedLight.txt", FA_OPEN_ALWAYS | FA_WRITE) \
X(storage_type_max, "", FA_READ)

#define X(type, filename, openFlags) type,
typedef enum storage_type { storage_types } storage_type_t;
#undef X

void storage_save(storage_type_t type, void *value);
bool storage_read(storage_type_t type, void *value);


#endif /* INC_STORAGE_H_ */
