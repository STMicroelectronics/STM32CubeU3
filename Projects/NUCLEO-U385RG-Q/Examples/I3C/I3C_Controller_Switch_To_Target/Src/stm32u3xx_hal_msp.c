
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    I3C/I3C_Controller_Switch_To_Target/Src/stm32u3xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief I3C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi3c: I3C handle pointer
* @retval None
*/
void HAL_I3C_MspInit(I3C_HandleTypeDef* hi3c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hi3c->Instance==I3C1)
  {
  /* USER CODE BEGIN I3C1_MspInit 0 */

  /* USER CODE END I3C1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I3C1;
    PeriphClkInit.I3c1ClockSelection = RCC_I3C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_I3C1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I3C1 GPIO Configuration
    PA6     ------> I3C1_SDA
    PA7     ------> I3C1_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_I3C1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* I3C1 interrupt Init */
    HAL_NVIC_SetPriority(I3C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I3C1_EV_IRQn);
    HAL_NVIC_SetPriority(I3C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I3C1_ER_IRQn);
  /* USER CODE BEGIN I3C1_MspInit 1 */
    
    /* Add a delay to let startup of High level on the Bus */
    HAL_Delay(1);

    /* Then set the pull at default value for I3C communication, mean no pull */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* USER CODE END I3C1_MspInit 1 */

  }

}

/**
* @brief I3C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi3c: I3C handle pointer
* @retval None
*/
void HAL_I3C_MspDeInit(I3C_HandleTypeDef* hi3c)
{
  if(hi3c->Instance==I3C1)
  {
  /* USER CODE BEGIN I3C1_MspDeInit 0 */

  /* USER CODE END I3C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I3C1_CLK_DISABLE();

    /**I3C1 GPIO Configuration
    PA6     ------> I3C1_SDA
    PA7     ------> I3C1_SCL
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6|GPIO_PIN_7);

    /* I3C1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(I3C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I3C1_ER_IRQn);
  /* USER CODE BEGIN I3C1_MspDeInit 1 */

  /* USER CODE END I3C1_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
