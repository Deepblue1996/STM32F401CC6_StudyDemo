/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define uchar unsigned char
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define p0_Pin GPIO_PIN_4
#define p0_GPIO_Port GPIOA
#define p1_Pin GPIO_PIN_5
#define p1_GPIO_Port GPIOA
#define p2_Pin GPIO_PIN_6
#define p2_GPIO_Port GPIOA
#define p3_Pin GPIO_PIN_1
#define p3_GPIO_Port GPIOB
#define p4_Pin GPIO_PIN_2
#define p4_GPIO_Port GPIOB
#define p5_Pin GPIO_PIN_10
#define p5_GPIO_Port GPIOB
#define rw_Pin GPIO_PIN_12
#define rw_GPIO_Port GPIOB
#define rs_Pin GPIO_PIN_13
#define rs_GPIO_Port GPIOB
#define p7_Pin GPIO_PIN_14
#define p7_GPIO_Port GPIOB
#define p6_Pin GPIO_PIN_15
#define p6_GPIO_Port GPIOB
#define U1_TX_Pin GPIO_PIN_9
#define U1_TX_GPIO_Port GPIOA
#define U1_RX_Pin GPIO_PIN_10
#define U1_RX_GPIO_Port GPIOA
#define e_Pin GPIO_PIN_5
#define e_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
