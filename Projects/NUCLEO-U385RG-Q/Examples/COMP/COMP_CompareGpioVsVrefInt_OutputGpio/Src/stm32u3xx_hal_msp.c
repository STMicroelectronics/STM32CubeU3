
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples/COMP/COMP_CompareGpioVsVrefInt_OutputGpio/Src/stm32u3xx_hal_msp.c
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
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  /* USER CODE END MspInit 1 */
}

/**
* @brief COMP MSP Initialization
* This function configures the hardware resources used in this example
* @param hcomp: COMP handle pointer
* @retval None
*/
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcomp->Instance==COMP1)
  {
  /* USER CODE BEGIN COMP1_MspInit 0 */

  /* USER CODE END COMP1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_COMP_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**COMP1 GPIO Configuration
    PA2     ------> COMP1_INP
    PB0     ------> COMP1_OUT
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF12_COMP1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN COMP1_MspInit 1 */

  /* USER CODE END COMP1_MspInit 1 */

  }

}

/**
* @brief COMP MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcomp: COMP handle pointer
* @retval None
*/
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
  if(hcomp->Instance==COMP1)
  {
  /* USER CODE BEGIN COMP1_MspDeInit 0 */

  /* USER CODE END COMP1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_COMP_CLK_DISABLE();

    /**COMP1 GPIO Configuration
    PA2     ------> COMP1_INP
    PB0     ------> COMP1_OUT
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

  /* USER CODE BEGIN COMP1_MspDeInit 1 */

  /* USER CODE END COMP1_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
