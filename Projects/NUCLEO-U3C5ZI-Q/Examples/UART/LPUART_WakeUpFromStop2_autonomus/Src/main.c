/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    UART/LPUART_WakeUpFromStop/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use UART through the STM32U3xx
  *          HAL API (LPUART instance) to wake up the MCU from STOP mode
  *          Two boards are used, one which enters STOP mode and the second
  *          one which sends the wake-up stimuli.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "stm32u3xx_nucleo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* to enable for the board entering STOP mode,
   to disable for the board sending wake-up stimuli */
//#define BOARD_IN_STOP_MODE
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef hlpuart1;
DMA_HandleTypeDef handle_GPDMA1_Channel1;
DMA_HandleTypeDef handle_GPDMA1_Channel0;

/* USER CODE BEGIN PV */
__IO ITStatus UartTxReady = RESET;
__IO ITStatus UartRxReady = RESET;
__IO uint32_t TransferStarted = 0U;
__IO uint32_t LedToggleDeadline = 0U;
/* Buffer used for transmission and as the reference frame for comparison. */
uint8_t aTxBuffer[] = "LPUART Stop2 autonomous";
/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN PFP */
#if defined(BOARD_IN_STOP_MODE)
void SystemClock_Config_fromSTOP(void);
static HAL_StatusTypeDef PrepareReceiverAutonomousEcho(void);
static HAL_StatusTypeDef ShutdownReceiverAutonomousEcho(void);
static void EnterStop2(void);
#endif
#if !defined(BOARD_IN_STOP_MODE)
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t buffer_length);
#endif
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
  HAL_RCCEx_StopWakeupSysclkConfig(RCC_STOP_WKUP_SYSCLK_MSIS);
  HAL_RCCEx_EnableKernelClkInStop(RCC_KERNELCLK_MSIK);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */
#if defined(BOARD_IN_STOP_MODE)
  HAL_GPIO_DeInit(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin);
#else
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = USER_BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);
  }
#endif
  BSP_LED_Init(LD1);

#if defined(BOARD_IN_STOP_MODE)
  HAL_DBGMCU_DisableDBGStopMode();
  BSP_LED_On(LD1);
  HAL_Delay(2000);
  BSP_LED_Off(LD1);
#else
  (void)memset(aRxBuffer, 0, sizeof(aRxBuffer));
  BSP_LED_On(LD1);
  LedToggleDeadline = HAL_GetTick() + TX_LED_TOGGLE_PERIOD_MS;

  while (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin) == GPIO_PIN_RESET)
  {
    if ((int32_t)(HAL_GetTick() - LedToggleDeadline) >= 0)
    {
      BSP_LED_Toggle(LD1);
      LedToggleDeadline = HAL_GetTick() + TX_LED_TOGGLE_PERIOD_MS;
    }
  }

  BSP_LED_Off(LD1);

  if (HAL_UART_Receive_DMA(&hlpuart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UART_Transmit_DMA(&hlpuart1, (uint8_t *)aTxBuffer, TXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }

  TransferStarted = 1U;
#endif /* BOARD_IN_STOP_MODE */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if defined(BOARD_IN_STOP_MODE)
    if (PrepareReceiverAutonomousEcho() != HAL_OK)
    {
      Error_Handler();
    }

    EnterStop2();

    while (UartTxReady != SET)
    {
    }

    if (ShutdownReceiverAutonomousEcho() != HAL_OK)
    {
      Error_Handler();
    }

    EnterStop2();
#else
    if ((TransferStarted != 0U) && (UartRxReady == SET))
    {
      UartTxReady = RESET;
      UartRxReady = RESET;
      TransferStarted = 0U;

      if (Buffercmp((uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != 0U)
      {
        Error_Handler();
      }

      BSP_LED_On(LD1);
      while (1)
      {
      }
    }
#endif /* BOARD_IN_STOP_MODE */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIKState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKSource = RCC_MSI_RC1;
  RCC_OscInitStruct.MSIKDiv = RCC_MSI_DIV4;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
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

/**
  * @brief GPDMA1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPDMA1_Init(void)
{

  /* USER CODE BEGIN GPDMA1_Init 0 */

  /* USER CODE END GPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  /* USER CODE BEGIN GPDMA1_Init 2 */

  /* USER CODE END GPDMA1_Init 2 */

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

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */
  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */
  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */
  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : USER_BUTTON_Pin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
#if defined(BOARD_IN_STOP_MODE)
/**
  * @brief  Prepare receiver autonomous RX plus echo in Stop2.
  * @retval HAL status
  */
static HAL_StatusTypeDef PrepareReceiverAutonomousEcho(void)
{
  UART_AutonomousModeConfTypeDef autonomous_mode = {0};

  UartTxReady = RESET;
  UartRxReady = RESET;
  (void)memset(aRxBuffer, 0, sizeof(aRxBuffer));

  (void)HAL_UART_Abort(&hlpuart1);
  __HAL_UART_CLEAR_FLAG(&hlpuart1, UART_CLEAR_PEF | UART_CLEAR_FEF | UART_CLEAR_NEF |
                                    UART_CLEAR_OREF | UART_CLEAR_IDLEF | UART_CLEAR_TCF);
  __HAL_UART_SEND_REQ(&hlpuart1, UART_RXDATA_FLUSH_REQUEST);

  if (HAL_UARTEx_ClearConfigAutonomousMode(&hlpuart1) != HAL_OK)
  {
    return HAL_ERROR;
  }

  autonomous_mode.AutonomousModeState = UART_AUTONOMOUS_MODE_ENABLE;
  autonomous_mode.TriggerSelection = LPUART_GPDMA1_CH0_TCF_TRG;
  autonomous_mode.TriggerPolarity = UART_TRIG_POLARITY_RISING;
  autonomous_mode.DataSize = RXBUFFERSIZE;
  autonomous_mode.IdleFrame = UART_IDLE_FRAME_DISABLE;

  if (HAL_UARTEx_SetConfigAutonomousMode(&hlpuart1, &autonomous_mode) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UART_Transmit_DMA(&hlpuart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UART_Receive_DMA(&hlpuart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_EnableStopMode(&hlpuart1) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Shut down the receiver autonomous echo path after the frame is echoed.
  * @retval HAL status
  */
static HAL_StatusTypeDef ShutdownReceiverAutonomousEcho(void)
{
  UartTxReady = RESET;
  UartRxReady = RESET;

  if (HAL_UARTEx_DisableStopMode(&hlpuart1) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_ClearConfigAutonomousMode(&hlpuart1) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_UART_DeInit(&hlpuart1) != HAL_OK)
  {
    return HAL_ERROR;
  }

  __HAL_RCC_GPDMA1_CLK_DISABLE();

  return HAL_OK;
}

/**
  * @brief  System Clock Configuration restored after STOP mode
  * @param  None
  * @retval None
  */
  void SystemClock_Config_fromSTOP(void)
{
  SystemClock_Config();
  HAL_RCCEx_StopWakeupSysclkConfig(RCC_STOP_WKUP_SYSCLK_MSIS);
  HAL_RCCEx_EnableKernelClkInStop(RCC_KERNELCLK_MSIK);
}

/**
  * @brief  Enter Stop2 mode and restore the system clock after wake-up.
  * @retval None
  */
  static void EnterStop2(void)
{
  HAL_SuspendTick();
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP2, PWR_STOPENTRY_WFI);
  HAL_ResumeTick();
    SystemClock_Config_fromSTOP();
}
#endif

#if !defined(BOARD_IN_STOP_MODE)
/**
  * @brief  Compare two data buffers.
  * @param  pBuffer1 first buffer
  * @param  pBuffer2 second buffer
  * @param  buffer_length number of bytes to compare
  * @retval 0 when both buffers are identical, non-zero otherwise
  */
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t buffer_length)
{
  while (buffer_length-- != 0U)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1U;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0U;
}
#endif

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  if (UartHandle->Instance == LPUART1)
  {
    Error_Handler();
  }
}

/**
  * @brief UART RX Complete callback
  * @param huart: uart handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == LPUART1)
  {
    UartRxReady = SET;
  }
}

/**
  * @brief UART TX Complete callback
  * @param huart: uart handle
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == LPUART1)
  {
    UartTxReady = SET;
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
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
    /* In case of error, LD1 transmits a sequence of three dots, three dashes, three dots */
    BSP_LED_Off(LD1);
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
