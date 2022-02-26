/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim11;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM11_Init(void);

/* USER CODE BEGIN Prototypes */
typedef struct tim_scal
{
	uint32_t cnt, max;
}tim_scal_t;

void tim_init(void);
void tim_scalStart(tim_scal_t* scal, uint32_t time);
void tim_scalStop(tim_scal_t* scal);
void tim_scalRestart(tim_scal_t* scal);
bool tim_scalCount(tim_scal_t* scal);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

