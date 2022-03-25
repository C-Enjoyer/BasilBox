/*
 * com.c
 *
 *  Created on: 25.03.2022
 *      Author: FloMo
 */

#include "com.h"
#include "usart.h"
#include "string.h"

uint16_t com_handleBuffer(uint8_t* buffer, uint16_t bufferSize, uint16_t head, uint16_t tail)
{
	char bIdiot[] = "You are a bloody idiot!\r\n";
	char fIdiot[] = "You are a fucking idiot!\r\n";
	char invalid[] = "Invalid character\r\n";

	switch(buffer[tail])
	{
		case 'b': com_transmitString(bIdiot);
		break;
		case 'f': com_transmitString(fIdiot);
		break;
		default: com_transmitString(invalid);
		break;
	}

	return usart_ringInc(tail, bufferSize);
}

void com_transmit(uint8_t* data, uint16_t num)
{
	usart1_transmit(data, num);
}

void com_transmitString(char* string)
{
	com_transmit((uint8_t*) string, strlen(string));
}
