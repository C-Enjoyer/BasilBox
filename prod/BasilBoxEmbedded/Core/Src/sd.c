/*
 * sd.c
 *
 *  Created on: 23.04.2022
 *      Author: FloMo
 */

#include "sd.h"
#include <string.h>

#define _SD_TESTS 10000

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
}

void sd_unmount(void)
{
	if(!sd_isMounted)
	{
		return;
	}

	sd_isMounted = (f_mount(0, (TCHAR const*)SDPath, 0) != FR_OK);
}

bool sd_test(void)
{
	FIL file;
	FRESULT volatile fr = FR_OK;
	char filename[] = "test.txt";
	char teststr[] = "Tester";
	UINT teststr_len = strlen(teststr);
	char teststr_return[teststr_len + 1];
	UINT writtenlen = 0;

	teststr_return[0] = '\0';

	if(!sd_isMounted) //mount sd
	{
		fr = f_mount(&SDFatFS, (TCHAR const*) SDPath, 0);
	}

	fr = f_open(&file, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE); //open file
	if(fr != FR_OK) return false;

	fr = f_write(&file,teststr,teststr_len,&writtenlen); // write string to file
	if(fr != FR_OK) return false;

	fr = f_lseek(&file,0); // go back to the begin of the file
	if(fr != FR_OK) return false;

	f_gets(teststr_return, teststr_len + 1, &file); // read string from file

	if(strcmp(teststr, teststr_return) != 0) //compare strings
	{
		return false;
	}

	fr = f_close(&file); // close file
	if(fr != FR_OK) return false;

	fr = f_unlink(filename); //delete file
	if(fr != FR_OK) return false;

	return true;
}

