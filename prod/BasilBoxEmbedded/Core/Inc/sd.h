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

#define SD_OPEN_ATTEMPTS		(5)
#define SD_WRITE_ATTEMPTS	(5)
#define SD_READ_ATTEMPTS		(5)
#define SD_LSEEK_ATTEMPTS	(5)
#define SD_TRUNCATE_ATTEMPTS	(5)
#define SD_DELETE_ATTEMPTS	(5)
#define SD_CLOSE_ATTEMPTS	(5)
#define SD_OPENDIR_ATTEMPTS	(5)
#define SD_READDIR_ATTEMPTS	(5)
#define SD_CLOSEDIR_ATTEMPTS	(5)

void sd_init(void);
void sd_mount(void);
void sd_unmount(void);
bool sd_fOpen(FIL *file, const TCHAR* path, BYTE mode);
bool sd_fWrite(FIL* file, char *str);
bool sd_fRead(FIL* file, char *str, uint32_t len, uint32_t* len_read);
bool sd_fLseek(FIL* file, FSIZE_t ofs);
bool sd_fGets(FIL* file, TCHAR* buf, int len);
bool sd_fTruncate(FIL* file);
bool sd_fDelete(const TCHAR* path);
bool sd_fExisting(const TCHAR* path);
bool sd_fClose(FIL *file);
bool sd_fClear(FIL *file);
bool sd_fDirExisting(const TCHAR* path);
bool sd_fOpenDir(DIR* dp, const TCHAR* path);
bool sd_fReadDir(DIR* dp, FILINFO* fno);
bool sd_fCloseDir(DIR* dp);

#endif /* INC_SD_H_ */
