/*
 * sd.h
 *
 *  Created on: 23.04.2022
 *      Author: FloMo
 */

#ifndef INC_SD_H_
#define INC_SD_H_

#include <stdio.h>
#include <stdbool.h>
#include "fatfs.h"

void sd_init(void);
void sd_mount(void);
void sd_unmount(void);
bool sd_test(void);

#endif /* INC_SD_H_ */
