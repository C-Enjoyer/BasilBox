/*
 * com.h
 *
 *  Created on: 25.03.2022
 *      Author: FloMo
 */

#ifndef INC_COM_H_
#define INC_COM_H_

#include <stdint.h>
#include <stdbool.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include "messages.pb.h"

void com_init(void);
void com_mainLoop(void);
uint16_t com_handleBuffer(uint8_t* buffer, uint16_t bufferSize, uint16_t head, uint16_t tail);
void com_sendMessage(BasilMessage_MessageType messageType, void* message);
void com_transmit(uint8_t* buffer, uint16_t num);
void com_transmitString(char* string);

#endif /* INC_COM_H_ */
