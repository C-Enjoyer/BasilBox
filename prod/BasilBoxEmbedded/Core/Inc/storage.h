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

#define STORAGE_MAX_LINE_LEN		100

#define storage_types \
X(storage_error, "Errorlog.txt") \
X(storage_filter_fan_state, "Filter.txt") \
X(storage_led_light_brightness, "Brightness.txt") \
X(storage_type_max, "")

#define X(type, filename) type,
typedef enum storage_type { storage_types } storage_type_t;
#undef X

void storage_save(storage_type_t type, void *value);


#endif /* INC_STORAGE_H_ */
