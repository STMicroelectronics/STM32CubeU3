/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_TwoBoards_WakeUpFromStop2_IT_Init/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u3xx.h"
#include "stm32u3xx_ll_i2c.h"
#include "stm32u3xx_ll_icache.h"
#include "stm32u3xx_ll_pwr.h"
#include "stm32u3xx_ll_crs.h"
#include "stm32u3xx_ll_rcc.h"
#include "stm32u3xx_ll_bus.h"
#include "stm32u3xx_ll_system.h"
#include "stm32u3xx_ll_exti.h"
#include "stm32u3xx_ll_cortex.h"
#include "stm32u3xx_ll_utils.h"
#include "stm32u3xx_ll_dma.h"
#include "stm32u3xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/**
  * @brief Toggle periods for various blinking modes
  */

#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

/**
  * @brief Slave settings
  */
#define SLAVE_OWN_ADDRESS                       0x5A

/* Uncomment this line to use the board as slave, if not it is used as master */
//#define SLAVE_BOARD

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* IRQ Handler treatment functions */
#ifdef SLAVE_BOARD
void Slave_Ready_To_Transmit_Callback(void);
void Slave_Complete_Callback(void);
#else /* MASTER_BOARD */
void UserButton_Callback(void);
void Master_Reception_Callback(void);
void Master_Complete_Callback(void);
#endif /* SLAVE_BOARD */
void Error_Callback(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SLAVE_OWN_ADDRESS 0x5A
#define USER_BUTTON_Pin LL_GPIO_PIN_13
#define USER_BUTTON_GPIO_Port GPIOC
#define USER_BUTTON_EXTI_IRQn EXTI13_IRQn
#define LED2_Pin LL_GPIO_PIN_5
#define LED2_GPIO_Port GPIOA
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
