/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : hsp_engine.c
  * @brief           : This file implements the HSP_Engine
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 - 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "hsp_engine.h"
#include "app_hsp_bram_alloc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* HSP Middleware handle */
/* HSP core handle declaration */
hsp_engine_context_t hmw;

/* Private function prototypes -----------------------------------------------*/

/* Application functions prototypes *******/
extern void Error_Handler(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief Middleware HSP Initialization Function
 * @param None
 * @retval None
 */
void MX_HSP_Engine_Init(void)
{
  /* USER CODE BEGIN HSP_Engine_Init_PreTreatment */

  /* USER CODE END HSP_Engine_Init_PreTreatment */

  if (HSP_Engine_IF_Init(&hmw) != HSP_IF_OK)
  {
    Error_Handler();
  }

  if (HSP_CORE_Init(&hmw, hmw.hdriver) != HSP_CORE_OK)
  {
    Error_Handler();
  }

  MX_HSP_BRAM_Allocation(&hmw);

  /* USER CODE BEGIN HSP_Engine_Init_PostTreatment */

  /* USER CODE END HSP_Engine_Init_PostTreatment */

}

/**
 * @brief Middleware HSP Process Function
 * @param None
 * @retval None
 */
void MX_HSP_Engine_Process(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
}

/**
 * @brief Middleware HSP get context Function
 * @param None
 * @retval None
 */
hsp_engine_context_t* MX_HSP_Engine_GetContext(void)
{

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
  return &hmw;
}
