/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    I3C/I3C_Sensor_Private_Command_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32U3xx I3C HAL API as Controller
  *          to transmit and receive a data buffer
  *          with a communication process based on Interrupt transfer.
  *          The communication is done using STM32U3xx Nucleo and
  *          X-NUCLEO-IKS4A1.
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
#include "desc_target1.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define I3C_IDX_FRAME_1         0U  /* Index of Frame 1 */
#define I3C_IDX_FRAME_2         1U  /* Index of Frame 2 */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I3C_HandleTypeDef hi3c2;

/* USER CODE BEGIN PV */
/* Context buffer related to Frame context, contain different buffer value for a communication */
I3C_XferTypeDef aContextBuffers[2];

/* Number of Targets detected during DAA procedure */
__IO uint32_t uwTargetCount = 0;

/* Buffer used for transmission */
uint8_t aTxBuffer[0x0F];

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Buffer used by HAL to compute control data for the Private Communication */
uint32_t aControlBuffer[0xF];

/* Array contain targets descriptor */
TargetDesc_TypeDef *aTargetDesc[1] = \
                          {
                            &TargetDesc1       /* DEVICE_ID1 */
                          };

/* Array contain SET CCC associated data */
uint8_t aDISEC_data[1]   = {0x08};
uint8_t aSETDASA_LSM6DSV16X_data[1]   = {(TARGET1_DYN_ADDR << 1)};

/* Buffer used for LSM6DSV16X configuration, and reading data raw */
uint8_t aLSM6DSV16X_Config[] = {0x10, 0x06, 0x06}; /* Enable Accelerometer then Gyroscope */
uint8_t aLSM6DSV16X_Data[]   = {0x20}; /* First Temperature registers + 12 Read continuous register access */

/* Variable to display reading data */
uint32_t uwDisplayDelay   = 0U;
int16_t Temperature       = 0;
int16_t aGyroscope[3]     = {0};
int16_t aAccelerometer[3] = {0};

/* Descriptor for broadcast CCC */
I3C_CCCTypeDef aBroadcast_CCC[] =
{
    /*  Target Addr           CCC Value           CCC data + defbyte pointer  CCC size + defbyte         Direction        */
    {0,                Broadcast_DISEC,           {aDISEC_data,           1},              LL_I3C_DIRECTION_WRITE},
    {0,                Broadcast_RSTDAA,          {NULL,                  0},              LL_I3C_DIRECTION_WRITE},
};

/* Descriptor for direct CCC */
I3C_CCCTypeDef aDirect_CCC[] =
{
    /*  Target Addr           CCC Value           CCC data + defbyte pointer  CCC size + defbyte         Direction        */
    {LSM6DSV16X_STA,         Direct_SETDASA,            {aSETDASA_LSM6DSV16X_data, 1},              LL_I3C_DIRECTION_WRITE},
};

/* Descriptor for private data transmit */
I3C_PrivateTypeDef aPrivateDescriptorConfig[1] = \
                                          {
                                            {TARGET1_DYN_ADDR, {aLSM6DSV16X_Config, LSM6DSV16XCONFIGSIZE}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE},
                                          };

/* Descriptor for private data transmit */
I3C_PrivateTypeDef aPrivateDescriptor[2] = \
                                          {
                                            {TARGET1_DYN_ADDR, {aLSM6DSV16X_Data, LSM6DSV16XDATASIZE}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE},
                                            {TARGET1_DYN_ADDR, {NULL, 0}, {aRxBuffer, RXBUFFERSIZE}, HAL_I3C_DIRECTION_READ}
                                          };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_I3C2_Init(void);
/* USER CODE BEGIN PFP */
static void DisplayValue(uint8_t *pPrivateBuffer);
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
extern void initialise_monitor_handles(void);
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
  LL_I3C_CtrlBusConfTypeDef CtrlBusConf;
  I3C_CtrlTimingTypeDef CtrlTiming;

#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
  initialise_monitor_handles();
  printf("Semihosting Test...\n\r");
#endif
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
  MX_ICACHE_Init();
  MX_I3C2_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LD2 */
  BSP_LED_Init(LD2);

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);

  /*#-STEP1-#- Disable IBI interrupt, Reset a previous Dynamic address ###############*/
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size    = COUNTOF(aControlBuffer);
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer   = aTxBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size      = 1;

  /* Associate a new Dynamic address through Static address */
  aContextBuffers[I3C_IDX_FRAME_2].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[I3C_IDX_FRAME_2].CtrlBuf.Size    = COUNTOF(aControlBuffer);
  aContextBuffers[I3C_IDX_FRAME_2].TxBuf.pBuffer   = aTxBuffer;
  aContextBuffers[I3C_IDX_FRAME_2].TxBuf.Size      = 1;

  /*##- Add context buffer Set CCC frame in Frame context ############################*/
  if (HAL_I3C_AddDescToFrame(&hi3c2,
                             aBroadcast_CCC,
                             NULL,
                             &aContextBuffers[I3C_IDX_FRAME_1],
                             COUNTOF(aBroadcast_CCC),
                             I3C_BROADCAST_WITHOUT_DEFBYTE_RESTART) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  if (HAL_I3C_Ctrl_TransmitCCC_IT(&hi3c2, &aContextBuffers[I3C_IDX_FRAME_1]) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Wait for the end of the transfer #################################*/
  /*  Before starting a new communication transfer, you need to check the current
  state of the peripheral; if it's busy you need to wait for the end of current
  transfer before starting a new one.
  For simplicity reasons, this example is just waiting till the end of the
  transfer, but application may perform other tasks while transfer operation
  is ongoing. */
  while (HAL_I3C_GetState(&hi3c2) != HAL_I3C_STATE_READY)
  {
  }

  /*#-STEP2-#- Set Dynamic address using Static address #############################*/
  /*##- Add context buffer Set CCC frame in Frame context ###########################*/
  if (HAL_I3C_AddDescToFrame(&hi3c2,
                             aDirect_CCC,
                             NULL,
                             &aContextBuffers[I3C_IDX_FRAME_2],
                             COUNTOF(aDirect_CCC),
                             I3C_DIRECT_WITHOUT_DEFBYTE_STOP) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  if (HAL_I3C_Ctrl_TransmitCCC_IT(&hi3c2, &aContextBuffers[I3C_IDX_FRAME_2]) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Wait for the end of the transfer #################################*/
  /*  Before starting a new communication transfer, you need to check the current
  state of the peripheral; if it's busy you need to wait for the end of current
  transfer before starting a new one.
  For simplicity reasons, this example is just waiting till the end of the
  transfer, but application may perform other tasks while transfer operation
  is ongoing. */
  while (HAL_I3C_GetState(&hi3c2) != HAL_I3C_STATE_READY)
  {
  }

  /* After a dynamic address has been assigned, the sensor is recognized as an I3C device */
  /* Check if the LSM6DSV16X sensor is ready to communicate in I3C */
  if (HAL_I3C_Ctrl_IsDeviceI3C_Ready(&hi3c2, TARGET1_DYN_ADDR, 300, 1000) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /* Set bus speed to 12.5MHz as on IKS4A1 is the maximum allowed speed due to the level shifters */
  /* Calculate the new timing for Controller */
  CtrlTiming.clockSrcFreq = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I3C2);
  CtrlTiming.i3cPPFreq = 12500000;
  CtrlTiming.i2cODFreq = 0;
  CtrlTiming.dutyCycle = 50;
  CtrlTiming.busType = I3C_PURE_I3C_BUS;

  /* Calculate the new timing for Controller */
  I3C_CtrlTimingComputation(&CtrlTiming, &CtrlBusConf);

  /* Update Controller Bus characteristic */
  HAL_I3C_Ctrl_BusCharacteristicConfig(&hi3c2, &CtrlBusConf);

  /*#-STEP3-#- Configure the Gyroscope ###################################*/
  /*##- Prepare context buffers process ##################################*/
  /* Prepare Transmit context buffer with the different parameters */
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size    = 1;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer   = aTxBuffer;
  aContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size      = LSM6DSV16XCONFIGSIZE;

  /*##- Add context buffer transmit in Frame context #####################*/
  if (HAL_I3C_AddDescToFrame(&hi3c2,
                             NULL,
                             &aPrivateDescriptorConfig[I3C_IDX_FRAME_1],
                             &aContextBuffers[I3C_IDX_FRAME_1],
                             aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size,
                             I3C_PRIVATE_WITH_ARB_STOP) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /* Wait for User push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }

  /* Wait for User push-button release before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
  }

  /*##- Start the transmission process ###################################*/
  /* Transmit private data processus */
  if (HAL_I3C_Ctrl_Transmit_IT(&hi3c2, &aContextBuffers[I3C_IDX_FRAME_1]) != HAL_OK)
  {
    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }

  /*##- Wait for the end of the transfer #################################*/
  /*  Before starting a new communication transfer, you need to check the current
  state of the peripheral; if it's busy you need to wait for the end of current
  transfer before starting a new one.
  For simplicity reasons, this example is just waiting till the end of the
  transfer, but application may perform other tasks while transfer operation
  is ongoing. */
  while (HAL_I3C_GetState(&hi3c2) != HAL_I3C_STATE_READY)
  {
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /*#-STEP4-#- Retrieve data from the Gyroscope ##########################*/

    /*##- Prepare context buffers process ##################################*/
    /* Prepare multiple transfer context buffer with the different parameters */
    aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.pBuffer = aControlBuffer;
    aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size    = 2;
    aContextBuffers[I3C_IDX_FRAME_1].TxBuf.pBuffer   = aTxBuffer;
    aContextBuffers[I3C_IDX_FRAME_1].TxBuf.Size      = LSM6DSV16XDATASIZE;
    aContextBuffers[I3C_IDX_FRAME_1].RxBuf.pBuffer   = aRxBuffer;
    aContextBuffers[I3C_IDX_FRAME_1].RxBuf.Size      = RXBUFFERSIZE;

    /*##- Add context buffer transmit in Frame context #####################*/
    if (HAL_I3C_AddDescToFrame(&hi3c2,
                               NULL,
                               &aPrivateDescriptor[I3C_IDX_FRAME_1],
                               &aContextBuffers[I3C_IDX_FRAME_1],
                               aContextBuffers[I3C_IDX_FRAME_1].CtrlBuf.Size,
                               I3C_PRIVATE_WITH_ARB_RESTART) != HAL_OK)
    {
      /* Error_Handler() function is called when error occurs. */
      Error_Handler();
    }

    /*##- Start the multiple transfer process ###################################*/
    /* Transmit/receive private data processus */
    if (HAL_I3C_Ctrl_MultipleTransfer_IT(&hi3c2, &aContextBuffers[I3C_IDX_FRAME_1]) != HAL_OK)
    {
      /* Error_Handler() function is called when error occurs. */
      Error_Handler();
    }

    /*##- Wait for the end of the transfer #################################*/
    /*  Before starting a new communication transfer, you need to check the current
    state of the peripheral; if it's busy you need to wait for the end of current
    transfer before starting a new one.
    For simplicity reasons, this example is just waiting till the end of the
    transfer, but application may perform other tasks while transfer operation
    is ongoing. */
    while (HAL_I3C_GetState(&hi3c2) != HAL_I3C_STATE_READY)
    {
    }
    /*##- Monitor the different value retrieve during Private communication */
    /*  At the end, of transfer, the application have retrieve all the data of
    the frame in an unique buffer, which must be unfill to retrieve the associated
    value */
    /* Display through external Terminal IO the value received */
    DisplayValue(&aRxBuffer[0]);
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
  * @brief I3C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I3C2_Init(void)
{

  /* USER CODE BEGIN I3C2_Init 0 */

  /* USER CODE END I3C2_Init 0 */

  I3C_FifoConfTypeDef sFifoConfig = {0};
  I3C_CtrlConfTypeDef sCtrlConfig = {0};

  /* USER CODE BEGIN I3C2_Init 1 */

  /* USER CODE END I3C2_Init 1 */
  hi3c2.Instance = I3C2;
  hi3c2.Mode = HAL_I3C_MODE_CONTROLLER;
  hi3c2.Init.CtrlBusCharacteristic.SDAHoldTime = HAL_I3C_SDA_HOLD_TIME_0_5;
  hi3c2.Init.CtrlBusCharacteristic.WaitTime = HAL_I3C_OWN_ACTIVITY_STATE_0;
  hi3c2.Init.CtrlBusCharacteristic.SCLPPLowDuration = 0x2f;
  hi3c2.Init.CtrlBusCharacteristic.SCLI3CHighDuration = 0x2f;
  hi3c2.Init.CtrlBusCharacteristic.SCLODLowDuration = 0x2f;
  hi3c2.Init.CtrlBusCharacteristic.SCLI2CHighDuration = 0x00;
  hi3c2.Init.CtrlBusCharacteristic.BusFreeDuration = 0x13;
  hi3c2.Init.CtrlBusCharacteristic.BusIdleDuration = 0x5e;
  if (HAL_I3C_Init(&hi3c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure FIFO
  */
  sFifoConfig.RxFifoThreshold = HAL_I3C_RXFIFO_THRESHOLD_4_4;
  sFifoConfig.TxFifoThreshold = HAL_I3C_TXFIFO_THRESHOLD_1_4;
  sFifoConfig.ControlFifo = HAL_I3C_CONTROLFIFO_DISABLE;
  sFifoConfig.StatusFifo = HAL_I3C_STATUSFIFO_DISABLE;
  if (HAL_I3C_SetConfigFifo(&hi3c2, &sFifoConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure controller
  */
  sCtrlConfig.DynamicAddr = 0;
  sCtrlConfig.StallTime = 0x00;
  sCtrlConfig.HotJoinAllowed = DISABLE;
  sCtrlConfig.ACKStallState = DISABLE;
  sCtrlConfig.CCCStallState = DISABLE;
  sCtrlConfig.TxStallState = DISABLE;
  sCtrlConfig.RxStallState = DISABLE;
  sCtrlConfig.HighKeeperSDA = DISABLE;
  if (HAL_I3C_Ctrl_Config(&hi3c2, &sCtrlConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I3C2_Init 2 */

  /* USER CODE END I3C2_Init 2 */

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

  /** Enable instruction cache (default 2-ways set associative cache)
  */
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Controller multiple Direct CCC Command, I3C private or I2C transfer Complete callback.
  * @param  hi3c : [IN] Pointer to an I3C_HandleTypeDef structure that contains the configuration information
  *                     for the specified I3C.
  * @retval None
  */
void HAL_I3C_CtrlMultipleXferCpltCallback(I3C_HandleTypeDef *hi3c)
{
  /* Toggle LD2: Transfer in transmission process is correct */
  BSP_LED_Toggle(LD2);
}

/**
  * @brief  Error callback.
  * @param  hi3c : [IN] Pointer to an I3C_HandleTypeDef structure that contains the configuration information
  *                     for the specified I3C.
  * @retval None
  */
void HAL_I3C_ErrorCallback(I3C_HandleTypeDef *hi3c)
{
  /* Error_Handler() function is called when error occurs. */
  Error_Handler();
}

/**
  * @brief  Display the different value retrieve during Communication.
  * @param  pPrivateBuffer   Pointer to a receive Private buffer.
  * @retval None.
  */
static void DisplayValue(uint8_t *pPrivateBuffer)
{
#define TEMP_OFFSET 25 /* Temperature offset is 25 degree celsius, mean Temperature value 0 correspond to 25 Celsius degree */

  /* Display the value measured each DISPLAY_REFRESH_DELAY */
  if (HAL_GetTick() >= (uwDisplayDelay + DISPLAY_REFRESH_DELAY))
  {
    Temperature = TEMP_OFFSET + (((int16_t)(((int16_t)aRxBuffer[1] * 256) + (int16_t)aRxBuffer[0])) / 100);

    printf("\n\r----------------LSM6DSV16X Measurement----------------\n\r");
    printf("Temperature (Celsius degree): %d \n\r", Temperature);

    aGyroscope[0] = (int16_t)aRxBuffer[3];
    aGyroscope[0] = (aGyroscope[0] * 256) + (int16_t)aRxBuffer[2];
    aGyroscope[1] = (int16_t)aRxBuffer[5];
    aGyroscope[1] = (aGyroscope[1] * 256) + (int16_t)aRxBuffer[4];
    aGyroscope[2] = (int16_t)aRxBuffer[7];
    aGyroscope[2] = (aGyroscope[2] * 256) + (int16_t)aRxBuffer[6];

    printf("GYR_X: %d, GYR_Y: %d, GYR_Z: %d\r\n", (int)aGyroscope[0], (int)aGyroscope[1], (int)aGyroscope[2]);

    aAccelerometer[0] = (int16_t)aRxBuffer[9];
    aAccelerometer[0] = (aAccelerometer[0] * 256) + (int16_t)aRxBuffer[8];
    aAccelerometer[1] = (int16_t)aRxBuffer[11];
    aAccelerometer[1] = (aAccelerometer[1] * 256) + (int16_t)aRxBuffer[10];
    aAccelerometer[2] = (int16_t)aRxBuffer[13];
    aAccelerometer[2] = (aAccelerometer[2] * 256) + (int16_t)aRxBuffer[12];

    printf("ACC_X: %d, ACC_Y: %d, ACC_Z: %d\r\n", (int)aAccelerometer[0], (int)aAccelerometer[1], (int)aAccelerometer[2]);

    uwDisplayDelay = HAL_GetTick();
  }
   /* Add process delay to let visibility of the LED management */
   HAL_Delay(100);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* Error if LD2 is slowly blinking (1 sec. period) */
  while (1)
  {
    BSP_LED_Toggle(LD2);
    HAL_Delay(1000);
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
  /* Infinite Loop */
  while(1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
