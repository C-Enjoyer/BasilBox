/*
 * sd.c
 *
 *  Created on: 23.04.2022
 *      Author: FloMo
 */

#include "sd.h"
#include <string.h>
#include "error.h"

bool sd_isMounted = false;

void sd_init(void)
{
	sd_mount();
}

void sd_mount(void)
{
	if(sd_isMounted)
	{
		return;
	}

	sd_isMounted = (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) == FR_OK);

	if(!sd_isMounted)
	{
		error_handle(error_sd_cannot_mount, error_soft);
	}
}

void sd_unmount(void)
{
	if(!sd_isMounted)
	{
		return;
	}

	sd_isMounted = (f_mount(0, (TCHAR const*)SDPath, 0) != FR_OK);

	if(sd_isMounted)
	{
		error_handle(error_sd_cannot_unmount, error_soft);
	}
}

bool sd_fOpen(FIL *file, const TCHAR* path, BYTE mode)
{
	FRESULT res = FR_OK;

	for(uint8_t i=0;i<SD_OPEN_ATTEMPTS;i++)
	{
		res = f_open(file, path, mode);
		if(res == FR_OK)
		{
			return true;
		}
	}

	sd_fClose(file);
	error_handle(error_sd_cannot_open_file, error_soft);
	return false;
}

bool sd_fWrite(FIL* file, char *str)
{
	unsigned int sendlen = strlen(str);
	unsigned int writtenlentotal = 0;
	FRESULT res = FR_OK;

	for(uint8_t i=0;i<SD_WRITE_ATTEMPTS;i++)
	{
		unsigned int writtenlen = 0;
		res = f_write(file,str,sendlen,&writtenlen);
		writtenlentotal += writtenlen;
		if(res == FR_OK && sendlen == writtenlentotal)
		{
			return true;
		}
	}

	sd_fClose(file);
	error_handle(error_sd_cannot_write_file, error_soft);
	return false;
}

bool sd_fRead(FIL* file, char *str, uint32_t len, uint32_t* len_read)
{
	FRESULT res = FR_OK;
	UINT read = 0;

	for(uint8_t i=0;i<SD_READ_ATTEMPTS;i++)
	{
		res = f_read(file, str, len, &read);
		*len_read = read;
		if(res == FR_OK)
		{
			return true;
		}
	}

	sd_fClose(file);
	error_handle(error_sd_cannot_read_file, error_soft);
	return false;
}

bool sd_fLseek(FIL* file, FSIZE_t ofs)
{
	FRESULT res = FR_OK;

	for(uint8_t i=0;i<SD_LSEEK_ATTEMPTS;i++)
	{
		res = f_lseek(file, ofs);
		if(res == FR_OK)
		{
			return true;
		}
	}

	sd_fClose(file);
	error_handle(error_sd_cannot_lseek_file, error_soft);
	return false;
}

bool sd_fGets(FIL* file, TCHAR* buf, int len)
{
	f_gets(buf, len, file);
	return true;
}

bool sd_fTruncate(FIL* file)
{
	FRESULT res = FR_OK;

	for(uint8_t i=0;i<SD_TRUNCATE_ATTEMPTS;i++)
	{
		res = f_truncate(file);
		if(res == FR_OK)
		{
			return true;
		}
	}

	sd_fClose(file);
	error_handle(error_sd_cannot_truncate_file, error_soft);
	return false;
}

bool sd_fDelete(const TCHAR* path)
{
	FRESULT res = FR_OK;

	for (uint8_t i = 0; i < SD_DELETE_ATTEMPTS; i++)
	{
		res = f_unlink(path);
		if (res == FR_OK)
		{
			return true;
		}
	}

	error_handle(error_sd_cannot_delete_file, error_soft);
	return false;
}

bool sd_fExisting(const TCHAR* path)
{
	FRESULT fr;
	FILINFO fno;

	fr = f_stat(path, &fno);

	return (fr == FR_OK) ? (true) : (false);
}

bool sd_fClose(FIL *file)
{
	FRESULT res = FR_OK;

	for (uint8_t i = 0; i < SD_CLOSE_ATTEMPTS; i++)
	{
		res = f_close(file);
		if (res == FR_OK)
		{
			return true;
		}
	}

	error_handle(error_sd_cannot_close_file, error_soft);
	return false;
}

bool sd_fClear(FIL *file)
{
	if(!sd_fLseek(file, 0))
	{
		return false;
	}
	if(!sd_fTruncate(file))
	{
		return false;
	}
	if(!sd_fClose(file))
	{
		return false;
	}

	return true;
}

bool sd_fDirExisting(const TCHAR* path)
{
	DIR dp;

	FRESULT res = f_opendir(&dp, path);

	if (res == FR_OK)
	{
		return sd_fCloseDir(&dp);
	}

	return false;
}

bool sd_fOpenDir(DIR* dp, const TCHAR* path)
{
	FRESULT res = FR_OK;

	for (uint8_t i = 0; i < SD_OPENDIR_ATTEMPTS; i++)
	{
		res = f_opendir(dp, path);
		if (res == FR_OK)
		{
			return true;
		}
	}

	error_handle(error_sd_cannot_open_dir, error_soft);
	return false;
}

bool sd_fReadDir(DIR* dp, FILINFO* fno)
{
	FRESULT res = FR_OK;

	for (uint8_t i = 0; i < SD_READDIR_ATTEMPTS; i++)
	{
		res = f_readdir(dp, fno);
		if (res == FR_OK)
		{
			return true;
		}
	}

	error_handle(error_sd_cannot_read_dir, error_soft);
	return false;
}

bool sd_fCloseDir(DIR* dp)
{
	FRESULT res = FR_OK;

	for (uint8_t i = 0; i < SD_CLOSEDIR_ATTEMPTS; i++)
	{
		res = f_closedir(dp);
		if (res == FR_OK)
		{
			return true;
		}
	}

	error_handle(error_sd_cannot_close_dir, error_soft);
	return false;
}
