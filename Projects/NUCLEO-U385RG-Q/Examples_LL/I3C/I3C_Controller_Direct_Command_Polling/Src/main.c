/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    I3C/I3C_Controller_Direct_Command_Polling/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32U3xx I3C LL API as Controller
  *          to manage a Direct Command procedure between a Controller and a Target
  *          with a communication process based on Polling transfer on Controller Side.
  *          The communication is done using 2 Boards.
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
#include "desc_target2.h"
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
__IO uint8_t ubButtonPress = 0;

/* Number of Targets detected during DAA procedure */
__IO uint32_t uwTargetCount = 0;

/* Array contain targets descriptor */
TargetDesc_TypeDef *aTargetDesc[2] = \
                          {
                            &TargetDesc1,       /* DEVICE_ID1 */
                            &TargetDesc2        /* DEVICE_ID2 */
                          };

uint8_t aPayloadBuffer[64*COUNTOF(aTargetDesc)];

/* Array contain SET CCC associated data */
uint8_t aSETMWL_data[2] = {0x0, 0x4};
uint8_t aSETMRL_data[2] = {0x0, 0x4};

/* Descriptor for direct read CCC */
I3C_CCCTypeDef aGET_CCCList[] =
{
    /*   Target Addr              CCC Value    CCC data + defbyte pointer  CCC size + defbyte         Direction       */
    {TARGET1_DYN_ADDR,          Direct_GETPID,          {NULL,                  6},             LL_I3C_DIRECTION_READ},
    {TARGET1_DYN_ADDR,          Direct_GETBCR,          {NULL,                  1},             LL_I3C_DIRECTION_READ},
    {TARGET1_DYN_ADDR,          Direct_GETDCR,          {NULL,                  1},             LL_I3C_DIRECTION_READ},
    {TARGET1_DYN_ADDR,          Direct_GETMWL,          {NULL,                  2},             LL_I3C_DIRECTION_READ},
    {TARGET1_DYN_ADDR,          Direct_GETMRL,          {NULL,                  2},             LL_I3C_DIRECTION_READ},
    {TARGET1_DYN_ADDR,          Direct_GETSTATUS,       {NULL,                  2},             LL_I3C_DIRECTION_READ},
};

/* Descriptor for direct write CCC */
I3C_CCCTypeDef aSET_CCCList[] =
{
    /*   Target Addr              CCC Value    CCC data + defbyte pointer  CCC size + defbyte         Direction       */
    {TARGET1_DYN_ADDR,          Direct_SETMWL,          {aSETMWL_data,          2},             LL_I3C_DIRECTION_WRITE},
    {TARGET1_DYN_ADDR,          Direct_SETMRL,          {aSETMRL_data,          2},             LL_I3C_DIRECTION_WRITE},
};

/* Array contain Get CCC name in char, this array is use to print the value receive during Get CCC procedure */
char* aCommandCode[] = {
  "GETPID",
  "GETBCR",
  "GETDCR",
  "GETMWL",
  "GETMRL",
  "GETSTATUS"};

/* CCC transfer variables */
uint8_t ubCCCIdx = 0;
uint8_t ubFrameIdx = 0;
uint32_t uwCCCMessage[0xFF] = {0};
__IO uint32_t uwExitCondition = LL_I3C_GENERATE_RESTART;
__IO uint8_t ubNbCCC = 0;

/* Buffer used for reception */
uint8_t aRxBuffer[0x10];
__IO uint8_t ubNbRxData = 0;

/* Buffer used for transmission */
uint8_t aTxBuffer[0x10];
__IO uint8_t ubNbTxDataToTransfer = 0;
__IO uint8_t ubNbTxData = 0;

/* Completion status */
__IO uint8_t ubFrameComplete = 0;
__IO uint8_t ubReceptionComplete = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I3C1_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Blinking(uint32_t Period);
void WaitForUserButtonPress(void);
void FlushBuffer8(uint8_t* pBuffer1, uint16_t BufferSize);
void Handle_ENTDAA_Controller(void);
void Prepare_GET_CCC_Transfer(void);
void Prepare_SET_CCC_Transfer(void);
void Handle_CCC_Controller(void);
static void DisplayCCCValue(I3C_CCCTypeDef *pGetCCCList, char **pCCCCharList, uint8_t *pCCCBuffer, uint8_t nbCCC);
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
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
  initialise_monitor_handles();
  printf("Semihosting Test...\n\r");
#endif
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I3C1_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
  /* Set LD2 Off */
  LED_Off();

  /*##- Start Dynamic Address Assignment procedure ##################################*/

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Handle Dynamic Address Assignment */
  Handle_ENTDAA_Controller();

  /*##- Prepare all CCC command  ####################################################*/
  /*  Each command is separate by a repeated start,
  until the end of the corresponding array aGET_CCCList then aSET_CCCList where
  an auto stop is generated */

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Prepare CCC transfer from array aGET_CCCList then aSET_CCCList */
  Prepare_GET_CCC_Transfer();
  Prepare_SET_CCC_Transfer();

  /*##- Send the First part of CCC command ##########################################*/
  /*  Each command is separate by an repeated start,
  until the end of the corresponding array */

  /* Handle CCC transfer for first step of this example, mean GETCCC */
  Handle_CCC_Controller();

  /*##- Monitor the different value retrieve during CCC get #########################*/
  /*  At the end, of transfer, the application have retrieve all the data of
  the frame in an unique buffer, which must be unfill to retrieve the associated
  value for each get CCC command. */
  /* Display through external Terminal IO the Get CCC associated value received */
  DisplayCCCValue(aGET_CCCList, &aCommandCode[0], &aRxBuffer[0], COUNTOF(aGET_CCCList));

  /*##- Send the second part of them ################################################*/
  /*  Each command is separate by an repeated start,
      until the end of the corresponding array */

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();

  /* Handle CCC transfer for second step of this example, mean SETCCC */
  Handle_CCC_Controller();
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

  /* Enable Epod Booster */
  LL_AHB1_GRP2_EnableClock(LL_AHB1_GRP2_PERIPH_PWR);
  LL_RCC_SetEPODBoosterClkSource(LL_RCC_EPODBOOSTCLKSRCE_MSIS);
  LL_PWR_EnableEPODBooster();
  while(LL_PWR_IsActiveFlag_BOOST() != 1)
  {
  }

  /* Activate MSIS as source */
  LL_RCC_MSIS_SetClockSource(LL_RCC_MSIS_CLOCK_SOURCE_RC0);
  LL_RCC_MSIS_SetClockDivision(LL_RCC_MSIS_CLOCK_SOURCE_RC_DIV_1);
  LL_RCC_MSI_SetMSIxClockRange();
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSIS);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_MSIS)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_HCLK_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_HCLK_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_HCLK_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_HCLK_DIV_1);

  LL_Init1msTick(96000000);

  LL_SetSystemCoreClock(96000000);
}

/**
  * @brief I3C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I3C1_Init(void)
{

  /* USER CODE BEGIN I3C1_Init 0 */

  /* USER CODE END I3C1_Init 0 */

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetI3CClockSource(LL_RCC_I3C1_CLKSOURCE_PCLK1);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I3C1);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**I3C1 GPIO Configuration
  PA6   ------> I3C1_SDA
  PA7   ------> I3C1_SCL
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN I3C1_Init 1 */

  /* USER CODE END I3C1_Init 1 */

  /** I3C Initialization
  */
  LL_I3C_SetMode(I3C1, LL_I3C_MODE_CONTROLLER);
  LL_I3C_SetDataHoldTime(I3C1, LL_I3C_SDA_HOLD_TIME_1_5);
  LL_I3C_SetControllerActivityState(I3C1, LL_I3C_OWN_ACTIVITY_STATE_0);
  LL_I3C_ConfigClockWaveForm(I3C1, 0x00230303);
  LL_I3C_SetCtrlBusCharacteristic(I3C1, 0x1014005e);
  LL_I3C_DisableHJAck(I3C1);

  /** Configure FIFO
  */
  LL_I3C_SetRxFIFOThreshold(I3C1, LL_I3C_RXFIFO_THRESHOLD_1_4);
  LL_I3C_SetTxFIFOThreshold(I3C1, LL_I3C_TXFIFO_THRESHOLD_1_4);
  LL_I3C_DisableControlFIFO(I3C1);
  LL_I3C_DisableStatusFIFO(I3C1);

  /** Configure Controller
  */
  LL_I3C_SetOwnDynamicAddress(I3C1, 0);
  LL_I3C_EnableOwnDynAddress(I3C1);
  LL_I3C_SetStallTime(I3C1, 0x00);
  LL_I3C_DisableStallACK(I3C1);
  LL_I3C_DisableStallParityCCC(I3C1);
  LL_I3C_DisableStallParityData(I3C1);
  LL_I3C_DisableStallTbit(I3C1);
  LL_I3C_DisableHighKeeperSDA(I3C1);

  /** Enable the selected I3C peripheral
  */
  LL_I3C_Enable(I3C1);
  /* USER CODE BEGIN I3C1_Init 2 */

  /* USER CODE END I3C1_Init 2 */

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
  LL_ICACHE_SetMode(LL_ICACHE_1WAY);
  LL_ICACHE_Enable();
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
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);

  /**/
  LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTC, LL_EXTI_CONFIG_LINE13);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_MODE_INPUT);

  /**/
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(EXTI13_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Flush 8-bit buffer.
  * @param  pBuffer1: pointer to the buffer to be flushed.
  * @param  BufferSize: Size of Buffer.
  * @retval None
  */
void FlushBuffer8(uint8_t* pBuffer1, uint16_t BufferSize)
{
  uint8_t Index = 0;

  for (Index = 0; Index < BufferSize; Index++)
  {
    pBuffer1[Index] = 0;
  }
}

/**
  * @brief  I3C transmit Dynamic address to a Target.
  * @param  None
  * @retval None
  */
static void I3C_DynamicAddressTreatment(void)
{
  uint64_t target_payload = 0U;

  /* Retrieve Target Payload */
  /* Check on the Rx FIFO threshold to know the Dynamic Address Assignment treatment process : byte or word */
  if (LL_I3C_GetRxFIFOThreshold(I3C1) == LL_I3C_RXFIFO_THRESHOLD_1_4)
  {
    /* For loop to get target payload */
    for (uint32_t index = 0U; index < 8U; index++)
    {
      /* Retrieve payload byte by byte */
      target_payload |= (uint64_t)((uint64_t)LL_I3C_ReceiveData8(I3C1) << (index * 8U));
    }
  }
  else
  {
    /* Retrieve first 32 bits payload */
    target_payload = (uint64_t)LL_I3C_ReceiveData32(I3C1);

    /* Retrieve second 32 bits payload */
    target_payload |= (uint64_t)((uint64_t)LL_I3C_ReceiveData32(I3C1) << 32U);
  }

  /* Store Payload in aTargetDesc */
  aTargetDesc[uwTargetCount]->TARGET_BCR_DCR_PID = target_payload;

  /* Send associated dynamic address */
  /* Write device address in the TDR register */
  /* Increment Target counter */
  LL_I3C_TransmitData8(I3C1, aTargetDesc[uwTargetCount++]->DYNAMIC_ADDR);
}

/**
  * @brief  This Function handle Controller events to perform an Assign dynamic address process
  * @note  This function is composed in one step :
  *        -1- Initiate the Dynamic address assignment procedure.
  * @param  None
  * @retval None
  */
void Handle_ENTDAA_Controller(void)
{
  /* (1) Initiate a Dynamic Address Assignment to the Target connected on the bus ****/
  /* Controller Generate Start condition for a write request with a Broadcast ENTDAA:
  *  - to the Targets connected on the bus
  *  - with an auto stop condition generation when all Targets answer the ENTDAA sequence.
  */
  LL_I3C_ControllerHandleCCC(I3C1, Broadcast_ENTDAA, 0, LL_I3C_GENERATE_STOP);

  do
  {
    /* If the I3C HW request a TX data, retrieve the Target Payload then send an associated Dynamic Address */
    if (LL_I3C_IsActiveFlag_TXFNF(I3C1))
    {
      /* Get Target Payload, then assign an associated Dynamic Address */
      I3C_DynamicAddressTreatment();
    }
  }
  while ((READ_REG(I3C1->EVR) & (I3C_EVR_FCF | I3C_EVR_ERRF)) == 0U);

  /* Clear frame complete flag */
  if (LL_I3C_IsActiveFlag_FC(I3C1))
  {
    LL_I3C_ClearFlag_FC(I3C1);

    /* Toggle LD2 */
    LED_Toggle();
  }

  /* Check on error flag */
  if (LL_I3C_IsActiveFlag_ERR(I3C1))
  {
    /* Clear error flag */
    LL_I3C_ClearFlag_ERR(I3C1);

    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
}

/**
  * @brief  This Function handle Get Direct Common Command Code transfer to a target
  * @note  This function is composed in one step :
  *        -1- Compute the different parameter of aGET_CCCList array into CCC message.
  *            The CCC message take the structure of hardware Control Register
  *            to safe process time during the sending of Control data into the hardware.
  *            A repeated start is inserted between each CCC command.
  *            A stop is inserted at the end of the CCC array.
  * @param  None
  * @retval None
  */
void Prepare_GET_CCC_Transfer(void)
{
  uint32_t ControlMask = (I3C_CR_ADD | I3C_CR_DCNT | I3C_CR_RNW | I3C_CR_MTYPE | I3C_CR_MEND);


  for(ubCCCIdx = 0; ubCCCIdx < COUNTOF(aGET_CCCList); ubCCCIdx++)
  {
    /* (1) Prepare the CCC transfer to a Target connected on the bus ***********************/
    /* First part correspond to Direct Command Code value, size of the defining byte and
       repeated start for second part of CCC message.
    */
    MODIFY_REG(uwCCCMessage[ubFrameIdx],                                                                               \
               ControlMask,                                                                                            \
               ((aGET_CCCList[ubCCCIdx].CCC << I3C_CR_RNW_Pos)                                                       | \
               0                                                                                                     | \
               LL_I3C_CONTROLLER_MTYPE_CCC                                                                           | \
               LL_I3C_GENERATE_RESTART));

    /* Increment Frame index */
    ubFrameIdx++;

    /* Insert a stop condition at the end of the CCC array */
    if (ubCCCIdx == (COUNTOF(aGET_CCCList) - 1))
    {
      uwExitCondition = LL_I3C_GENERATE_STOP;
    }

    /* Second part correspond to Target address, size of the CCC associated data, direction and
       repeated start between each CCC message or stop the last CCC message.
    */
    MODIFY_REG(uwCCCMessage[ubFrameIdx],                                                                               \
               ControlMask,                                                                                            \
               ((aGET_CCCList[ubCCCIdx].TargetAddr << I3C_CR_ADD_Pos)                                                | \
               aGET_CCCList[ubCCCIdx].CCCBuf.Size                                                                    | \
               aGET_CCCList[ubCCCIdx].Direction                                                                      | \
               LL_I3C_CONTROLLER_MTYPE_DIRECT                                                                        | \
               uwExitCondition));

    /* Reset variable to Restart default value */
    uwExitCondition = LL_I3C_GENERATE_RESTART;

    /* Increment Frame index */
    ubFrameIdx++;
  }
}

/**
  * @brief  This Function handle Set Direct Common Command Code transfer to a target
  * @note  This function is composed in one step :
  *        -1- Compute the different parameter of aSET_CCCList array into CCC message.
  *            The CCC message take the structure of hardware Control Register
  *            to safe process time during the sending of Control data into the hardware.
  *            A repeated start is inserted between each CCC command.
  *            A stop is inserted at the end of the CCC array.
  * @param  None
  * @retval None
  */
void Prepare_SET_CCC_Transfer(void)
{
  uint32_t ControlMask = (I3C_CR_ADD | I3C_CR_DCNT | I3C_CR_RNW | I3C_CR_MTYPE | I3C_CR_MEND);

  for(ubCCCIdx = 0; ubCCCIdx < COUNTOF(aSET_CCCList); ubCCCIdx++)
  {
    /* (1) Prepare the CCC transfer to a Target connected on the bus ***********************/
    /* First part correspond to Direct Command Code value, size of the defining byte and
       repeated start for second part of CCC message.
    */
    MODIFY_REG(uwCCCMessage[ubFrameIdx],                                                                               \
               ControlMask,                                                                                            \
               ((aSET_CCCList[ubCCCIdx].CCC << I3C_CR_RNW_Pos)                                                       | \
               0                                                                                                     | \
               LL_I3C_CONTROLLER_MTYPE_CCC                                                                           | \
               LL_I3C_GENERATE_RESTART));

    /* Increment Frame index */
    ubFrameIdx++;

    /* Insert a stop condition at the end of the CCC array */
    if (ubCCCIdx == (COUNTOF(aSET_CCCList) - 1))
    {
      uwExitCondition = LL_I3C_GENERATE_STOP;
    }

    /* Second part correspond to Target address, size of the CCC associated data, direction and
       repeated start between each CCC message or stop the last CCC message.
    */
    MODIFY_REG(uwCCCMessage[ubFrameIdx],                                                                               \
               ControlMask,                                                                                            \
               ((aSET_CCCList[ubCCCIdx].TargetAddr << I3C_CR_ADD_Pos)                                                | \
               aSET_CCCList[ubCCCIdx].CCCBuf.Size                                                                    | \
               aSET_CCCList[ubCCCIdx].Direction                                                                      | \
               LL_I3C_CONTROLLER_MTYPE_DIRECT                                                                        | \
               uwExitCondition));

    /* Prepare Tx Buffer with the value of CCC associated data */
    for (uint8_t size = 0; size < aSET_CCCList[ubCCCIdx].CCCBuf.Size; size++)
    {
      aTxBuffer[ubNbTxDataToTransfer++] = aSET_CCCList[ubCCCIdx].CCCBuf.pBuffer[size];
    }

    /* Reset variable to Restart default value */
    uwExitCondition = LL_I3C_GENERATE_RESTART;

    /* Increment Frame index */
    ubFrameIdx++;
  }
}

/**
  * @brief  This Function handle Direct Common Command Code transfer to a target
  * @note  This function is composed in one step :
  *        -1- Initiate the start of the CCC transfer procedure.
  * @param  None
  * @retval None
  */
void Handle_CCC_Controller(void)
{
  /* Flush Buffer before start a CCC transfer */
  FlushBuffer8(aRxBuffer, COUNTOF(aRxBuffer));

  /* Reset Counter */
  ubNbRxData = 0;

  /* Start Transfer CCC */
  LL_I3C_RequestTransfer(I3C1);

  do
  {
    /* Write message into CR register */
    if (LL_I3C_IsActiveFlag_CFNF(I3C1))
    {
      WRITE_REG(I3C1->CR, uwCCCMessage[ubNbCCC++]);
    }

    /* Receive Common Command Code Associated data if any */
    if (LL_I3C_IsActiveFlag_RXFNE(I3C1))
    {
      aRxBuffer[ubNbRxData++] = LL_I3C_ReceiveData8(I3C1);
    }

    /* Transmit Common Command Code Associated data if any */
    if ((ubNbTxDataToTransfer > 0) && (LL_I3C_IsActiveFlag_TXFNF(I3C1)))
    {
      /* Write Tx buffer data to transmit register */
      LL_I3C_TransmitData8(I3C1, aTxBuffer[ubNbTxData++]);

      /* Decrement remaining bytes counter */
      ubNbTxDataToTransfer--;
    }
  }
  while ((READ_REG(I3C1->EVR) & (I3C_EVR_FCF | I3C_EVR_ERRF)) == 0U);

  /* Clear frame complete flag */
  if (LL_I3C_IsActiveFlag_FC(I3C1))
  {
    LL_I3C_ClearFlag_FC(I3C1);

    /* Toggle LD2 */
    LED_Toggle();
  }

  /* Check on error flag */
  if (LL_I3C_IsActiveFlag_ERR(I3C1))
  {
    /* Clear error flag */
    LL_I3C_ClearFlag_ERR(I3C1);

    /* Error_Handler() function is called when error occurs. */
    Error_Handler();
  }
}

/**
  * @brief  Display the different value retrieve during GET CCC.
  * @param  pGetCCCList  Pointer to a CCC List descriptor.
  * @param  pCCCCharList Pointer to a buffer contain CCC name in char.
  * @param  pCCCBuffer   Pointer to a receive CCC buffer.
  * @param  nbCCC        Number of Get CCC to display.
  * @retval None.
  */
static void DisplayCCCValue(I3C_CCCTypeDef *pGetCCCList, char **pCCCCharList, uint8_t *pCCCBuffer, uint8_t nbCCC)
{
  for (uint8_t j = 0; j < nbCCC; j++)
  {
    printf("%s: 0x", pCCCCharList[j]);
    for (uint8_t i = 0; i < (pGetCCCList[j].CCCBuf.Size); i++)
    {
      /* Fill local buffer with different CCC value */
      printf("%x", *pCCCBuffer++);
    }
    printf("\n\r");
  }
}

/**
  * @brief  Turn-on LD2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LD2 on */
  LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Turn-off LD2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LD2 off */
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Toggle LD2.
  * @param  None
  * @retval None
  */
void LED_Toggle(void)
{
  /* Toggle LD2 on */
  LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Set LD2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Turn LD2 on */
  LED_On();

  /* Toggle IO in an infinite loop */
  while (1)
  {
    LED_Toggle();
    LL_mDelay(Period);
  }
}

/******************************************************************************/
/*   IRQ HANDLER TREATMENT Functions                                          */
/******************************************************************************/
/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None
  * @retval None
  */
/*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LED_Toggle();
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Turn LD2 off */
  LED_Off();

  /* Reset Variable */
  ubButtonPress = 0;
}

/**
  * @brief  Function to manage User push-button
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  while (1)
  {
    /* Unexpected event : Set LD2 to Blinking mode to indicate error occurs */
    LED_Blinking(LED_BLINK_ERROR);
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
