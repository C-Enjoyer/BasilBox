/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "com.h"

bool usart_initialized = false;

uint8_t usart1_rxDmaBuffer[USART1_RX_SIZE] = {0};
uint8_t usart1_txBuffer[USART1_TX_SIZE] = {0};
uint16_t usart1_rxTail = 0;
uint16_t usart1_rxHead = 0;
uint16_t usart1_txTail = 0;
uint16_t usart1_txHead = 0;
uint16_t usart1_txAmount = 0;
bool usart1_txReady = true;

void _usart1_rxCheck(void);
void _usart1_txCheck(void);
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
	if(!usart_initialized)
	{
		usart_initialized = true;
#ifndef DEBUG
		HAL_Delay(7000);
#endif
	}
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, USART_TX_Pin|USART_RX_Pin);

  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void usart_init(void)
{
	usart1_init();
}

void usart_mainLoop(void)
{
	_usart1_rxCheck();
	_usart1_txCheck();
}

void _usart1_rxCheck(void)
{
	uint16_t head = USART1_RX_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
    if (head != usart1_rxTail && head != usart1_rxHead)
    {
    	usart1_rxHead = head;
    	usart1_rxTail = com_handleBuffer(usart1_rxDmaBuffer, USART1_RX_SIZE, usart1_rxHead, usart1_rxTail);
    }
}

void _usart1_txCheck(void)
{
	HAL_UART_StateTypeDef uartState = HAL_UART_GetState(&huart1);
	if(usart1_txReady && (uartState == HAL_UART_STATE_READY || uartState == HAL_UART_STATE_BUSY_RX) && (usart1_txHead != usart1_txTail))
	{
		usart1_txReady = false;
		usart1_txAmount = usart_bufLen(usart1_txHead, usart1_txTail, USART1_TX_SIZE);

		HAL_UART_Transmit_IT(&huart1, &usart1_txBuffer[usart1_txTail], usart1_txAmount);
	}
}

void usart1_transmit(uint8_t* data, uint16_t num)
{
	for(uint16_t i = 0; i < num ; i++)
	{
		usart1_txBuffer[usart1_txHead] = data[i];

		uint16_t nextHead = usart_ringInc(usart1_txHead, USART1_TX_SIZE);

		if(nextHead == usart1_txTail) // head hitting tail
		{
			return;
		}

		usart1_txHead = nextHead;
	}
}

void usart1_init(void)
{
	HAL_UART_Receive_DMA(&huart1, usart1_rxDmaBuffer,  USART1_RX_SIZE);
}

void usart1_deInit(void)
{
	HAL_UART_DeInit(&huart1);
}

void usart1_reInit(void)
{
	usart1_deInit();
	MX_USART1_UART_Init();
	usart1_init();
	usart1_rxTail = USART1_RX_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
}

uint16_t usart_ringInc(uint16_t num, uint16_t max)
{
	return (num < max - 1) ? (num + 1) : (0);
}

uint16_t usart_ringIncBy(uint16_t num, uint16_t inc, uint16_t max)
{
	return (max - inc > num) ? (num + inc) : (inc - (max - num));
}

uint16_t usart_ringLen(uint16_t head, uint16_t tail, uint16_t max)
{
    return (head >= tail) ? (head - tail) : (max - (tail - head));
}

uint16_t usart_bufLen(uint16_t head, uint16_t tail, uint16_t max)
{
	return (head >= tail) ? (head - tail) : (max - tail);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
		usart1_txTail = usart_ringIncBy(usart1_txTail, usart1_txAmount, USART1_TX_SIZE);
		usart1_txReady = true;
	}
}

/* USER CODE END 1 */
