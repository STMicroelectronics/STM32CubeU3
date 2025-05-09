/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FDCAN/FDCAN_Adaptive_Bitrate_Receiver/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to configure the FDCAN to adapt to
  *          different CAN bit rates using restricted mode.
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
#define TX_TIMEOUT               (1000U)   /* Transmission timeout in ms                              */
#define RX_TIMEOUT               (100U)    /* Reception timeout in ms                                 */
#define TX_ID                    (0x321)   /* TX CAN message identifier                               */
#define RX_ID                    (0x444)   /* RX CAN message identifier                               */
#define NB_BITRATES              (5U)      /* Number of bitrate to test                               */
#define NB_MSG_TO_RECEIVE        (1U)      /* Number of message to receive                            */
#define DEFAULT_BITRATE_KBPS     (1000U)   /* Default 1Mbits/s nominal bitrate defined in STM32CubeMX */

/* Hardware related, can't be changed */
#define NB_RX_FIFO               (3U)      /* Number of RX FIFO Elements available                    */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define COUNTOF(BUFFER) (sizeof((BUFFER)) / sizeof(*(BUFFER)))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

FDCAN_HandleTypeDef hfdcan1;

/* USER CODE BEGIN PV */
FDCAN_RxHeaderTypeDef rxHeader;
uint8_t               rxData[8U];
static const uint8_t  txDataAck[]                = {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};
static const uint8_t  expectedRxData[]           = {0x10, 0x32, 0x54, 0x76, 0x98, 0x00, 0x11, 0x22};
static const uint16_t bitrates_kbps[NB_BITRATES] = {100U, 125U, 250U, 500U, 1000U}; /* Bitrates to test, in kbits/s */
__IO uint8_t          bitrate_found              = 0U; /* Flag used in IT context to signal bitrate match */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
static uint32_t BufferCmp8b(const uint8_t *pBuffer1, const uint8_t *pBuffer2, uint16_t BufferLength);

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
  uint8_t  index_bitrate;
  uint32_t tickstart;


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
  /* Configure LD2 */
  BSP_LED_Init(LD2);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FDCAN1_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /*##-1 Adapt to bus bitrate ###############################################*/

  /* Loop through different bit rates */
  for (index_bitrate = 0; index_bitrate < NB_BITRATES; index_bitrate++)
  {
    /**
      *  Configure bit timing for current bit rate.
      *  As baudrate is by default configured at 1Mbits/s in STM32CubeMX, changing the nominal prescaler
      *  is a quick way to change the baudrate
      */
    hfdcan1.Init.NominalPrescaler = (DEFAULT_BITRATE_KBPS / bitrates_kbps[index_bitrate]);

    /* Apply the modified NominalPrescaler */
    if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
    {
      Error_Handler();
    }

    /* Configure standard ID reception filter to Rx FIFO 0. Only accept ID = RX_ID */
    FDCAN_FilterTypeDef        sFilterConfig;
    sFilterConfig.IdType       = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex  = 0U;
    sFilterConfig.FilterType   = FDCAN_FILTER_DUAL;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1    = RX_ID;
    sFilterConfig.FilterID2    = RX_ID; /* For acceptance, MessageID and FilterID1 must match exactly */
    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
    {
      Error_Handler();
    }

    /**
      *  Configure global filter:
      *    - Reject all remote frames with STD and EXT ID
      *    - Reject non matching frames with STD ID and EXT ID
      */
    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,
                                     FDCAN_REJECT, FDCAN_REJECT,
                                     FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK)
    {
      Error_Handler();
    }

    /* Activate Rx FIFO 0 new message notification */
    if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0U) != HAL_OK)
    {
      Error_Handler();
    }

    /* Start FDCAN controller */
    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
    {
      Error_Handler();
    }

    /* Get tick */
    tickstart = HAL_GetTick();

    /**
      * Test if the IT context has received the expected message.
      * Wait for RX_TIMEOUT ms before trying with another bitrate
      */
    while ((bitrate_found == 0U) && ((HAL_GetTick() - tickstart) <= RX_TIMEOUT))
    {
      /* Wait for incoming message */
    }

    if (bitrate_found == 1U)
    {
      /* Suitable bitrate was found. Message was well received and data is as expected. */
      /* Stop searching for another bitrate. Break the for loop. */
      break;
    }
  }

  /* Check if no suitable bitrate were found */
  if (index_bitrate >= NB_BITRATES)
  {
    /* No suitable bitrate was found. Please check for connection issue. */
    Error_Handler();
  }

  /*##-2 Exit restricted mode ###############################################*/

  HAL_FDCAN_ExitRestrictedOperationMode(&hfdcan1);

  /*##-3 Send acknowledge message to the transmitter board ##################*/

  /* Add message to Tx FIFO */
  FDCAN_TxHeaderTypeDef           txHeaderAck;
  txHeaderAck.Identifier          = TX_ID;
  txHeaderAck.IdType              = FDCAN_STANDARD_ID;
  txHeaderAck.TxFrameType         = FDCAN_DATA_FRAME;
  txHeaderAck.DataLength          = FDCAN_DLC_BYTES_8;
  txHeaderAck.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  txHeaderAck.BitRateSwitch       = FDCAN_BRS_OFF;
  txHeaderAck.FDFormat            = FDCAN_CLASSIC_CAN;
  txHeaderAck.TxEventFifoControl  = FDCAN_STORE_TX_EVENTS;
  txHeaderAck.MessageMarker       = 0U;
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeaderAck, txDataAck) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait transmission complete */
  while (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1) != NB_RX_FIFO)
  {
    /* Timeout handling */
    if ((HAL_GetTick() - tickstart) > TX_TIMEOUT)
    {
      Error_Handler();
    }
  }

  /* Execution success. Turn ON LD2 */
  BSP_LED_On(LD2);

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
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = ENABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 1;
  hfdcan1.Init.NominalSyncJumpWidth = 24;
  hfdcan1.Init.NominalTimeSeg1 = 71;
  hfdcan1.Init.NominalTimeSeg2 = 24;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.StdFiltersNbr = 1;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

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
  * @brief  Compares two buffers.
  * @param  pBuffer1 buffer to be compared.
  * @param  pBuffer2 buffer to be compared.
  * @param  BufferLength: buffer's length.
  * @retval 0: pBuffer1 is identical to pBuffer2
  * @retval 1: pBuffer1 differs from pBuffer2
  */
static uint32_t BufferCmp8b(const uint8_t *pBuffer1, const uint8_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
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

/**
  * @brief  Rx FIFO 0 callback.
  * @param  hfdcan pointer to an FDCAN_HandleTypeDef structure that contains
  *         the configuration information for the specified FDCAN.
  * @param  RxFifo0ITs indicates which Rx FIFO 0 interrupts are signaled.
  *         This parameter can be any combination of @arg FDCAN_Rx_Fifo0_Interrupts.
  * @retval None
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  /* If a new message is in FIFO0 */
  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0U)
  {
    /* Retrieve Rx message from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK)
    {
      Error_Handler();
    }

    /* Compare received RX message to expected data. Ignore if not matching. */
    if ((rxHeader.Identifier == RX_ID) &&
        (rxHeader.IdType     == FDCAN_STANDARD_ID) &&
        (rxHeader.DataLength == FDCAN_DLC_BYTES_8) &&
        (BufferCmp8b(expectedRxData, rxData, COUNTOF(rxData)) == 0U))
    {
      /* Suitable bitrate was found. Message was well received and data is as expected. */
      /* Stop searching for another bitrate.*/
      bitrate_found = 1U;
    }
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
    /* Toggle LD2 on */
    BSP_LED_Toggle(LD2);

    /* 1s delay */
    HAL_Delay(1000U);
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
