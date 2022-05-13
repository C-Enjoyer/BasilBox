/*
 * com.c
 *
 *  Created on: 25.03.2022
 *      Author: FloMo
 */

#include "com.h"
#include "usart.h"
#include "rtc.h"
#include <string.h>

#define _COM_BASIL_ID					42069420
#define _COM_MESSAGE_SIZE_OFFSET		sizeof(uint32_t)
#define _COM_MAX_IN_MESSAGE_SIZE		(BasilMessage_size)
#define _COM_MAX_OUT_MESSAGE_SIZE	(_COM_MAX_IN_MESSAGE_SIZE + _COM_MESSAGE_SIZE_OFFSET)

void _com_sendPing(void);
void _com_decodeMessage(void);
bool _com_decodeMessageReady(void);
void _com_handleDecodeMessage(void);
void _com_handleMessage(BasilMessage_MessageType messageType, void* message);
void _com_createOutStream(void);
void _com_setTs(uint32_t sec, uint32_t min, uint32_t hour, uint32_t day, uint32_t month, uint32_t year);

uint8_t com_messageOutBuffer[_COM_MAX_OUT_MESSAGE_SIZE] = {0};
uint8_t com_messageInBuffer[_COM_MAX_IN_MESSAGE_SIZE] = {0};
pb_ostream_t com_outStream;
pb_istream_t com_inStream;

uint32_t com_nextMessageLength = 0;
uint32_t com_decodeMessageLength = 0;
bool com_shouldDecodeMessage = false;

void com_init(void)
{

}

void com_mainLoop(void)
{
	if(com_shouldDecodeMessage)
	{
		_com_handleDecodeMessage();
		com_shouldDecodeMessage = false;
	}
}

uint16_t com_handleBuffer(uint8_t* buffer, uint16_t bufferSize, uint16_t head, uint16_t tail)
{
	uint16_t len = usart_ringLen(head, tail, bufferSize);

	if(com_nextMessageLength == 0)
	{
		if(len < _COM_MESSAGE_SIZE_OFFSET)
		{
			return tail;
		}
		usart_ringToBuf((uint8_t*) &com_nextMessageLength, _COM_MESSAGE_SIZE_OFFSET, buffer, tail, bufferSize);

		if(com_nextMessageLength > _COM_MAX_IN_MESSAGE_SIZE)
		{
			Error_Handler(); //TODO: handle Error
		}

		return usart_ringIncBy(tail, _COM_MESSAGE_SIZE_OFFSET, bufferSize);
	}

	if(len >= com_nextMessageLength && _com_decodeMessageReady())
	{
		usart_ringToBuf(com_messageInBuffer, com_nextMessageLength, buffer, tail, bufferSize);
		com_decodeMessageLength = com_nextMessageLength;
		com_nextMessageLength = 0;
		_com_decodeMessage();

		return usart_ringIncBy(tail, com_decodeMessageLength, bufferSize);
	}

	return tail;
}

void _com_handleDecodeMessage(void)
{
	BasilMessage basilMessage = BasilMessage_init_zero;

	com_inStream = pb_istream_from_buffer(com_messageInBuffer, com_decodeMessageLength);
	bool decodeRes = pb_decode(&com_inStream, BasilMessage_fields, &basilMessage);

	if (!decodeRes)
	{
		Error_Handler(); //TODO: handle Error
	}

	_com_handleMessage(basilMessage.which_message, &basilMessage.message);
}

void _com_handleMessage(BasilMessage_MessageType messageType, void* message)
{
	switch(messageType)
	{
		case BasilMessage_MessageType_infoMessageType:
		{
			InfoMessage infoMessage = *(InfoMessage*) message;
			if(infoMessage.has_tsSec && infoMessage.has_tsMin && infoMessage.has_tsHour && infoMessage.has_tsDay && infoMessage.has_tsMonth && infoMessage.has_tsYear)
			{
				_com_setTs(infoMessage.tsSec, infoMessage.tsMin, infoMessage.tsHour, infoMessage.tsDay, infoMessage.tsMonth, infoMessage.tsYear);
			}
			if(infoMessage.has_pingId && infoMessage.pingId == _COM_BASIL_ID)
			{
				_com_sendPing();
			}
		}break;
		default: Error_Handler(); //TODO: handle Error
		break;
	}
}

void com_transmit(uint8_t* data, uint16_t num)
{
	usart1_transmit(data, num);
}

void com_transmitString(char* string)
{
	com_transmit((uint8_t*) string, strlen(string));
}

void _com_sendPing(void)
{
	InfoMessage message = InfoMessage_init_zero;
	message.pingId = _COM_BASIL_ID;
	message.has_pingId = true;

	com_sendMessage(BasilMessage_MessageType_infoMessageType, &message);
}

void _com_decodeMessage(void)
{
	com_shouldDecodeMessage = true;
}

bool _com_decodeMessageReady(void)
{
	return !com_shouldDecodeMessage;
}

void _com_createOutStream(void)
{
	com_outStream = pb_ostream_from_buffer(&com_messageOutBuffer[_COM_MESSAGE_SIZE_OFFSET], _COM_MAX_OUT_MESSAGE_SIZE - _COM_MESSAGE_SIZE_OFFSET);
}

void com_sendMessage(BasilMessage_MessageType messageType, void* message)
{
	BasilMessage basilMessage = BasilMessage_init_zero;
	bool encodeRes = false;
	uint32_t messageLength = 0;

	switch(messageType)
	{
		case BasilMessage_MessageType_infoMessageType:
		{
			basilMessage.message.infoMessage = *(InfoMessage*) message;
		}break;
		default: Error_Handler(); //TODO: handle Error
		break;
	}

	basilMessage.which_message = messageType;
	_com_createOutStream();
	encodeRes = pb_encode(&com_outStream, BasilMessage_fields, &basilMessage);

	if(!encodeRes)
	{
		Error_Handler(); //TODO: handle Error
	}

	messageLength = com_outStream.bytes_written;

	if(messageLength == 0 || messageLength > BasilMessage_size)
	{
		Error_Handler(); //TODO: handle Error
	}

	memcpy(com_messageOutBuffer, &messageLength, _COM_MESSAGE_SIZE_OFFSET);
	com_transmit(com_messageOutBuffer, messageLength + _COM_MESSAGE_SIZE_OFFSET);
}

void _com_setTs(uint32_t sec, uint32_t min, uint32_t hour, uint32_t day, uint32_t month, uint32_t year)
{
	if(sec >= 60 || min >= 60 || hour >= 24 || day > 31 || month > 12 || year >= 100)
	{
		return;
	}

	rtc_ts_t ts = {
		.time = {
			.sec = sec,
			.min = min,
			.hour = hour
		},
		.date = {
			.day = day,
			.month = month,
			.year = year
		}
	};

	rtc_setTs(ts);
}
