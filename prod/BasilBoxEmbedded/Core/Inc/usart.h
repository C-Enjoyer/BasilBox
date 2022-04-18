/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define USART1_RX_SIZE	512
#define USART1_TX_SIZE	512
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void usart_init(void);
void usart_mainLoop(void);
void usart1_transmit(uint8_t* data, uint16_t num);
void usart1_init(void);
void usart1_deInit(void);
void usart1_reInit(void);
uint16_t usart_ringInc(uint16_t num, uint16_t max);
uint16_t usart_ringIncBy(uint16_t num, uint16_t inc, uint16_t max);
uint16_t usart_ringLen(uint16_t head, uint16_t tail, uint16_t max);
uint16_t usart_bufLen(uint16_t head, uint16_t tail, uint16_t max);
void usart_ringToBuf(uint8_t* buf, uint16_t len, uint8_t* ring, uint16_t start, uint16_t max);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

