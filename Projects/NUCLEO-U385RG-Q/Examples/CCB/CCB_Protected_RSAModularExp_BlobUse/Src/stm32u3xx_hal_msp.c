/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32u3xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
* @brief CCB MSP Initialization
* This function configures the hardware resources used in this example
* @param hccb: CCB handle pointer
* @retval None
*/
void HAL_CCB_MspInit(CCB_HandleTypeDef* hccb)
{
  if(hccb->Instance==CCB)
  {
  /* USER CODE BEGIN CCB_MspInit 0 */

  /* USER CODE END CCB_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CCB_CLK_ENABLE();
    __HAL_RCC_PKA_CLK_ENABLE();
    __HAL_RCC_SAES_CLK_ENABLE();
    __HAL_RCC_RNG_CLK_ENABLE();
  /* USER CODE BEGIN CCB_MspInit 1 */

  /* USER CODE END CCB_MspInit 1 */

  }

}

/**
* @brief CCB MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hccb: CCB handle pointer
* @retval None
*/
void HAL_CCB_MspDeInit(CCB_HandleTypeDef* hccb)
{
  if(hccb->Instance==CCB)
  {
  /* USER CODE BEGIN CCB_MspDeInit 0 */

  /* USER CODE END CCB_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CCB_CLK_DISABLE();
    __HAL_RCC_PKA_CLK_DISABLE();
    __HAL_RCC_SAES_CLK_DISABLE();
    __HAL_RCC_RNG_CLK_DISABLE();
  /* USER CODE BEGIN CCB_MspDeInit 1 */

  /* USER CODE END CCB_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
