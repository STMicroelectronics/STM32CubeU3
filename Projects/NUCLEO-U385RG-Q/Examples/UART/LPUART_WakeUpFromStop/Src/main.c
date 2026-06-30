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
#include "stm32u3xx_nucleo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* to enable for the board entering STOP mode,
   to disable for the board sending wake-up stimuli */
#define BOARD_IN_STOP_MODE
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */
__IO uint32_t UserReceptionComplete = 0;  /* set to 1 after User Button interrupt  */
#if !defined(BOARD_IN_STOP_MODE)
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */
#endif /* BOARD_IN_STOP_MODE */

/* Buffer used for transmission */
uint8_t aTxBuffer[] = "RXNE wake-up" ;

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
#if defined(BOARD_IN_STOP_MODE)
void SystemClock_Config_fromSTOP(void);
#else
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
#endif /* defined(BOARD_IN_STOP_MODE) */


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
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

    /* Configure LD2*/
  BSP_LED_Init(LD2);

#if !defined(BOARD_IN_STOP_MODE)

  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Wait for User push-button press before starting the Communication.
     In the meantime, LD2 is blinking */
  while (UserButtonStatus == 0)
  {
    /* Toggle LD2*/
    BSP_LED_Toggle(LD2);
    HAL_Delay(100);
  }

  BSP_LED_Off(LD2);

  /* The board sends the message and expects to receive it back */

  /*##-1- Start the transmission process #####################################*/
  if (HAL_UART_Transmit_IT(&hlpuart1, (uint8_t *)aTxBuffer, TXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-2- Put UART peripheral in reception process ###########################*/
  if (HAL_UART_Receive_IT(&hlpuart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait for reception complete callback */
  while (UserReceptionComplete == 0)
  {
  }

  /* Compare the expected and received buffers */
  if (Buffercmp((uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, RXBUFFERSIZE))
  {
    Error_Handler();
  }

  /* Turn on LD2 if test is ok */
  BSP_LED_On(LD2);

#else
  /* LD2 is on till stop mode */
  BSP_LED_On(LD2);

  /* wait for two seconds before test start */
  HAL_Delay(2000);

  /* Disable Debug during Stop mode */
  HAL_DBGMCU_DisableDBGStopMode();

  /* After STOP, use MSIS as temporary SYSCLK (matches RCC_STOP_WKUP_SYSCLK_MSIS) */
  HAL_RCCEx_StopWakeupSysclkConfig(RCC_STOP_WKUP_SYSCLK_MSIS);

  /* Make sure MSIK kernel clock is available in STOP (for LPUART) */
  HAL_RCCEx_EnableKernelClkInStop(RCC_KERNELCLK_MSIK);

  /* Enable MCU wake-up by LPUART */
  HAL_UARTEx_EnableStopMode(&hlpuart1);

  /*##-1- Put UART peripheral in reception process ###########################*/
  if (HAL_UART_Receive_IT(&hlpuart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {

      Error_Handler();
  }

  /* about to enter STOP mode: switch off LD2 */
  BSP_LED_Off(LD2);

  /* enter STOP mode */
  /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
  /* Otherwise the Systick interrupt will wake up the device within 1ms     */
  /* (HAL time base).                                                       */
  HAL_SuspendTick();
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP2, PWR_STOPENTRY_WFI);
  /* Resume Tick interrupt if disabled prior to STOP mode entry */
  HAL_ResumeTick();

  /* The board receives the message from the other board. Message reception wakes up the board. */
  /* Switch On LD2 */
  BSP_LED_On(LD2);

  /* At that point, MCU has been awoken: LD2 has been turned back on ; Wake Up based on RXNE flag successful */
  SystemClock_Config_fromSTOP();

  /* Disable MCU wake-up by LPUART */
  HAL_UARTEx_DisableStopMode(&hlpuart1);

  /* Wait for reception complete callback */
  while (UserReceptionComplete == 0)
  {
  }

  /*##-2- Start the transmission process #####################################*/
  /* Send back received buffer */
  if (HAL_UART_Transmit_IT(&hlpuart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {

    Error_Handler();

  }

#endif /* !BOARD_IN_STOP_MODE */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
#ifdef BOARD_IN_STOP_MODE
  LPUART1_EXTI_ENABLE_IT();
#endif /* BOARD_IN_STOP_MODE */
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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI13_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

#if defined(BOARD_IN_STOP_MODE)
/**
  * @brief  System Clock Configuration restored after STOP mode
  * @param  None
  * @retval None
  */
void SystemClock_Config_fromSTOP(void)
{

  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t pFLatency = 0;

  /* Get the Oscillators configuration from the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* Wake up on HSI, re-enable PLL with HSI as source */
  /* Oscillator configuration unchanged */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* Get the clock prescalers configuration from the internal RCC registers */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif /* BOARD_IN_STOP_MODE */

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  Error_Handler();
}

#if !defined(BOARD_IN_STOP_MODE)
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUTTON_USER_PIN)
  {
    UserButtonStatus = 1;
  }
}
#endif /* !BOARD_IN_STOP_MODE */

/**
  * @brief UART RX Complete callback
  * @param huart: uart handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  UserReceptionComplete = 1;
}

#if !defined(BOARD_IN_STOP_MODE)
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}
#endif /* !defined(BOARD_IN_STOP_MODE) */
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
    /* In case of error, LD2 turns OFF */
  BSP_LED_Off(LD2);
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
