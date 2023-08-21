/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "math.h"
#include "key.h"
#include "led.h"
#include "pid.h"
#include "bee.h"
#include "adc.h"
#include "motor.h"
#include "pstwo.h"
#include "wt901c.h"
#include "ws2812b.h"
#include "adc_voltage.h"
#include "tofsense.h"
#include "linktrack.h"
#include "linktrack_aoa.h"
#include "nlink_linktrack_nodeframe2.h"
#include "nlink_linktrack_nodeframe0.h"
#include "nlink_linktrack_aoa_nodeframe0.h"
#include "nlink_tofsense_frame0.h"
#include "nlink_tofsensem_frame0.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define BEE_Pin GPIO_PIN_2
#define BEE_GPIO_Port GPIOE
#define KEY4_Pin GPIO_PIN_3
#define KEY4_GPIO_Port GPIOE
#define KEY4_EXTI_IRQn EXTI3_IRQn
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOE
#define KEY3_EXTI_IRQn EXTI4_IRQn
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOE
#define KEY2_EXTI_IRQn EXTI9_5_IRQn
#define KEY1_Pin GPIO_PIN_6
#define KEY1_GPIO_Port GPIOE
#define KEY1_EXTI_IRQn EXTI9_5_IRQn
#define PS2_ATT_Pin GPIO_PIN_12
#define PS2_ATT_GPIO_Port GPIOB
#define PS2_CLK_Pin GPIO_PIN_13
#define PS2_CLK_GPIO_Port GPIOB
#define PS2_DAT_Pin GPIO_PIN_14
#define PS2_DAT_GPIO_Port GPIOB
#define PS2_COM_Pin GPIO_PIN_15
#define PS2_COM_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_8
#define LED4_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_9
#define LED3_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_10
#define LED2_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_11
#define LED1_GPIO_Port GPIOD
#define Motor1__Direction_Pin GPIO_PIN_7
#define Motor1__Direction_GPIO_Port GPIOD
#define Motor2__Direction_Pin GPIO_PIN_3
#define Motor2__Direction_GPIO_Port GPIOB
#define Motor3__Direction_Pin GPIO_PIN_4
#define Motor3__Direction_GPIO_Port GPIOB
#define Motor4__Direction_Pin GPIO_PIN_5
#define Motor4__Direction_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
