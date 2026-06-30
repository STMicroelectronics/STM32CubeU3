/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    RCC/RCC_MSIPLLMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the RCC HAL API to configure the
  *          system clock (SYSCLK) and modify the clock settings on run time.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MSI_NORMAL_MODE  0U
#define MSIRC0_PLLMODE   1U
#define MSIRC1_PLLMODE   2U
#define MSI_NB_MODE      3U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint32_t mode_selection;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void MSI_96MHz_Config(void);
static void MSIRC0_PLL_LSEConfig(void);
static void MSIRC1_PLL_LSEConfig(void);
static void SystemClockSwitch_HSI(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LD2 */
  BSP_LED_Init(LD2);

  /* Initialize User push-button, will be used to trigger an interrupt each time it's pressed.
     In the ISR the PLL source will be changed from different clocks */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Output SYSCLK on MCO1 pin(PA.08) */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

  /* Initialize MSIS clock frequency variable selection */
  mode_selection = MSI_NORMAL_MODE;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* Toggle LD2 */
    if (mode_selection == MSI_NORMAL_MODE)
    {
      /* If MSIS is running at 96 MHz without MSIS PLL enable, LD2 is emitting a 
      double flash every half-second */
      BSP_LED_On(LD2);
      HAL_Delay(100);
      BSP_LED_Off(LD2);
      HAL_Delay(100);
      BSP_LED_On(LD2);
      HAL_Delay(100);
      BSP_LED_Off(LD2);
      HAL_Delay(500);
    }
    else if (mode_selection == MSIRC0_PLLMODE)
    {
      /* MSI PLL mode where MSI input is RC0 and it is locked to LSE, LD2
      toggles fast */
      BSP_LED_Toggle(LD2);
      HAL_Delay(100);
    }

    else if (mode_selection == MSIRC1_PLLMODE)
    {
      /* MSI PLL mode where MSI input is RC1 and it is locked to LSE, LD2
      toggles slow */
      BSP_LED_Toggle(LD2);
      HAL_Delay(1000);
    }
    else
    {
      /* We should not reach here */
      Error_Handler();
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
  */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable Epod Booster
  */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_PWREx_EnableEpodBooster() != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Set Flash latency before increasing MSIS
  */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);

  /** Enable MSI Oscillator
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS | RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  RCC_OscInitStruct.MSIKDiv = RCC_MSI_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/* USER CODE BEGIN 4 */
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    mode_selection++;

    if (mode_selection == MSI_NB_MODE)
    {
      mode_selection = MSI_NORMAL_MODE;
    }
  }

  if (mode_selection == MSI_NORMAL_MODE)
  {
      MSI_96MHz_Config();
  }
  else if (mode_selection == MSIRC0_PLLMODE)
  {
      MSIRC0_PLL_LSEConfig();
  }
  else if (mode_selection == MSIRC1_PLLMODE)
  {
      MSIRC1_PLL_LSEConfig();
  }
  else
  {
    /* We should not reach here */
    Error_Handler();
  }
}

/**
  * @brief  Configure MSI to reach 96MHz
  * @param  None
  * @retval None
  */
static void MSI_96MHz_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_MSIRCxPLLTypeDef msipll_set = {0};

  /* Disable previously configured MSI PLL modes */
  msipll_set.State = RCC_MSIRCx_PLL_OFF;
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC0, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC1, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_PWREx_EnableEpodBooster();

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set Flash latency before modifying MSIS frequency */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);

  /* Reconfigure MSIS */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  Configure MSI frequency as 48MHz using RC0 and MSI locked to LSE
  * @param  None
  * @retval None
  */
static void MSIRC0_PLL_LSEConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_MSIRCxPLLTypeDef msipll_set = {0};

  /* Switch SYCLK to HSI */
  SystemClockSwitch_HSI();

  /* Disable previously configured MSI PLL modes */
  msipll_set.State = RCC_MSIRCx_PLL_OFF;
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC0, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC1, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_PWREx_EnableEpodBooster();

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set Flash latency before modifying MSIS frequency */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1);

  /* Configure MSIS frequency as 48MHz and MSIS locked to LSE */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV2;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure MSI PLL mode */
  msipll_set.State = RCC_MSIRCx_PLL_ON;
  msipll_set.InputSrce = RCC_MSIRCx_PLL_INPUT_LSE;
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC0, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Configure MSI frequency as 24MHz using RC1 and MSI locked to LSE
  * @param  None
  * @retval None
  */
static void MSIRC1_PLL_LSEConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_MSIRCxPLLTypeDef msipll_set = {0};

  /* Switch SYCLK to HSI */
  SystemClockSwitch_HSI();

  /* Disable previously configured MSI PLL modes */
  msipll_set.State = RCC_MSIRCx_PLL_OFF;
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC0, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC1, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_PWREx_DisableEpodBooster();

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set Flash latency before modifying MSIS frequency */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1);

  /* Reconfigure MSIS nd configure LSE for MSI PLL source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC1;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure MSI PLL mode */
  msipll_set.State = RCC_MSIRCx_PLL_ON;
  msipll_set.InputSrce = RCC_MSIRCx_PLL_INPUT_LSE;
  if (HAL_RCCEx_MSIRCxPLLModeConfig(RCC_MSI_RC1, &msipll_set) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Switch system clock to HSI
  * @param  None
  * @retval None
  */
static void SystemClockSwitch_HSI(void)
{
  RCC_OscInitTypeDef  RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Disable EPOD booster */
  if (HAL_PWREx_DisableEpodBooster() != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure the main internal regulator output voltage scale to 2 */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initializes the CPU, AHB and APB buses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */

  /* Turn LD2 on */
  BSP_LED_On(LD2);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
