/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef hlpuart1;
DMA_NodeTypeDef Node_GPDMA1_Channel1;
DMA_QListTypeDef List_GPDMA1_Channel1;
DMA_HandleTypeDef handle_GPDMA1_Channel1;

/* USER CODE BEGIN PV */
/**
  * @brief Text strings printed on PC Com port for user information
  */
uint8_t aTextInfoStart[] = "\r\nUSART Example : Enter characters to fill reception buffers.\r\n";

uint8_t aRXBufferUser[RX_BUFFER_SIZE];

/**
  * @brief Data buffers used to manage received data in interrupt routine
  */
uint8_t aRXBufferA[RX_BUFFER_SIZE];
uint8_t aRXBufferB[RX_BUFFER_SIZE];

__IO uint32_t     uwNbReceivedChars;
uint8_t *pBufferReadyForUser;
uint8_t *pBufferReadyForReception;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
void PrintInfo(UART_HandleTypeDef *huart, uint8_t *String, uint16_t Size);
void StartReception(void);
void UserDataTreatment(UART_HandleTypeDef *huart, uint8_t* pData, uint16_t Size);

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
  /* STM32U3xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
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
  MX_GPDMA1_Init();
  MX_LPUART1_UART_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* Initiate Continuous reception */
  StartReception();

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

  /** Initializes the CPU, AHB and APB buses clocks
  */
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Send Txt information message on UART Tx line (to PC Com port).
  * @param  huart UART handle.
  * @param  String String to be sent to user display
  * @param  Size   Size of string
  * @retval None
  */
void PrintInfo(UART_HandleTypeDef *huart, uint8_t *String, uint16_t Size)
{
  if (HAL_OK != HAL_UART_Transmit(huart, String, Size, 100))
  {
    Error_Handler();
  }
}

/**
  * @brief  This function prints user info on PC com port and initiates RX transfer
  * @retval None
  */
void StartReception(void)
{
  /* Initializes Buffer swap mechanism (used in User callback) :
     - 2 physical buffers aRXBufferA and aRXBufferB (RX_BUFFER_SIZE length)
  */
  pBufferReadyForReception = aRXBufferA;
  pBufferReadyForUser      = aRXBufferB;
  uwNbReceivedChars        = 0;

  /* Print user info on PC com port */
  PrintInfo(&hlpuart1, aTextInfoStart, COUNTOF(aTextInfoStart));

  /* Initializes Rx sequence using Reception To Idle event API.
     As DMA channel associated to UART Rx is configured as Circular,
     reception is endless.
     If reception has to be stopped, call to HAL_UART_AbortReceive() could be used.

     Use of HAL_UARTEx_ReceiveToIdle_DMA service, will generate calls to
     user defined HAL_UARTEx_RxEventCallback callback for each occurrence of
     following events :
     - DMA RX Half Transfer event (HT)
     - DMA RX Transfer Complete event (TC)
     - IDLE event on UART Rx line (indicating a pause is UART reception flow)
  */
  if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(&hlpuart1, aRXBufferUser, RX_BUFFER_SIZE))
  {
    Error_Handler();
  }
}

/**
  * @brief  This function handles buffer containing received data on PC com port
  * @note   In this example, received data are sent back on UART Tx (loopback)
  *         Any other processing such as copying received data in a larger buffer to make it
  *         available for application, could be implemented here.
  * @note   This routine is executed in Interrupt context.
  * @param  huart UART handle.
  * @param  pData Pointer on received data buffer to be processed
  * @retval Size  Nb of received characters available in buffer
  */
void UserDataTreatment(UART_HandleTypeDef *huart, uint8_t* pData, uint16_t Size)
{
  /*
   * This function might be called in any of the following interrupt contexts :
   *  - DMA TC and HT events
   *  - UART IDLE line event
   *
   * pData and Size defines the buffer where received data have been copied, in order to be processed.
   * During this processing of already received data, reception is still ongoing.
   *
   */
  uint8_t* pBuff = pData;
  uint8_t  i;

  /* Implementation of loopback is on purpose implemented in direct register access,
     in order to be able to echo received characters as fast as they are received.
     Wait for TC flag to be raised at end of transmit is then removed, only TXE is checked */
  for (i = 0; i < Size; i++)
  {
    while (!(__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE))) {}
    huart->Instance->TDR = *pBuff;
    pBuff++;
  }

}

/**
  * @brief  User implementation of the Reception Event Callback
  *         (Rx event notification called after use of advanced reception service).
  * @param  huart UART handle
  * @param  Size  Number of data available in application reception buffer (indicates a position in
  *               reception buffer until which, data are available)
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  static uint8_t old_pos = 0;
  uint8_t *ptemp;
  uint8_t i;

  /* Check if number of received data in recpetion buffer has changed */
  if (Size != old_pos)
  {
    /* Check if position of index in reception buffer has simply be increased
       of if end of buffer has been reached */
    if (Size > old_pos)
    {
      /* Current position is higher than previous one */
      uwNbReceivedChars = Size - old_pos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0; i < uwNbReceivedChars; i++)
      {
        pBufferReadyForUser[i] = aRXBufferUser[old_pos + i];
      }
    }
    else
    {
      /* Current position is lower than previous one : end of buffer has been reached */
      /* First copy data from current position till end of buffer */
      uwNbReceivedChars = RX_BUFFER_SIZE - old_pos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0; i < uwNbReceivedChars; i++)
      {
        pBufferReadyForUser[i] = aRXBufferUser[old_pos + i];
      }
      /* Check and continue with beginning of buffer */
      if (Size > 0)
      {
        for (i = 0; i < Size; i++)
        {
          pBufferReadyForUser[uwNbReceivedChars + i] = aRXBufferUser[i];
        }
        uwNbReceivedChars += Size;
      }
    }
    /* Process received data that has been extracted from Rx User buffer */
    UserDataTreatment(huart, pBufferReadyForUser, uwNbReceivedChars);

    /* Swap buffers for next bytes to be processed */
    ptemp = pBufferReadyForUser;
    pBufferReadyForUser = pBufferReadyForReception;
    pBufferReadyForReception = ptemp;
  }
  /* Update old_pos as new reference of position in User Rx buffer that
     indicates position to which data have been processed */
  old_pos = Size;

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
    /* Toggle LD2 for error */
    BSP_LED_Toggle(LD2);
    HAL_Delay(500);
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

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
