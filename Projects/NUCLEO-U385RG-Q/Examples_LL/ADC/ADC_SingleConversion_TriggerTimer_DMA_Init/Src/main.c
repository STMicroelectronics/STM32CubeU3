/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_SingleConversion_TriggerTimer_DMA_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a ADC peripheral to perform
  *          a single ADC conversion of a channel, at each trigger event
  *          from timer.
  *          Conversion data is transferred by DMA into a table in RAM memory,
  *          indefinitely (circular mode).
  *          This example is based on the STM32U3xx ADC LL API;
  *          Peripheral initialization done using LL unitary services functions.
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

/* Definitions of ADC hardware constraints delays */
/* Note: Only ADC IP HW delays are defined in ADC LL driver driver,           */
/*       not timeout values:                                                  */
/*       Timeout values for ADC operations are dependent to device clock      */
/*       configuration (system clock versus ADC clock),                       */
/*       and therefore must be defined in user application.                   */
/*       Refer to @ref ADC_LL_EC_HW_DELAYS for description of ADC timeout     */
/*       values definition.                                                   */


  /* Timeout values for ADC operations. */
  /* (calibration, enable settling time, disable settling time, ...)          */
  /* Values defined to be higher than worst cases: low clock frequency,       */
  /* maximum prescalers.                                                      */
  /* Note: ADC channel configuration ready (ADC_CHANNEL_CONF_RDY_TIMEOUT_MS)  */
  /*       is added in CubeMx code section.                                   */
  /* Unit: ms                                                                 */
  #define ADC_CALIBRATION_TIMEOUT_MS       (   1UL)
  #define ADC_ENABLE_TIMEOUT_MS            (   1UL)
  #define ADC_DISABLE_TIMEOUT_MS           (   1UL)
  #define ADC_STOP_CONVERSION_TIMEOUT_MS   (   1UL)
  #define ADC_CONVERSION_TIMEOUT_MS        (4000UL)

  /* Delay between ADC end of calibration and ADC enable.                     */
  /* Delay estimation in CPU cycles: Case of ADC enable done                  */
  /* immediately after ADC calibration, ADC clock setting slow                */
  /* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
  /* (CPU clock / ADC clock) is above 32.                                     */
  #define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       (3300UL)

/* Definitions of data related to this example */
  /* Definition of ADCx conversions data table size */
  #define ADC_CONVERTED_DATA_BUFFER_SIZE   (  64UL)

  /* Init variable out of expected ADC conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)

  /* Parameters of time base (used as ADC conversion trigger) */
  /* Time base frequency (unit: Hz). With a timer 16 bits and time base       */
  /* freq max 32kHz, range is [min=1Hz, max=32kHz].                           */
  #define TIMER_FREQUENCY_HZ               (1000UL)
  /* Time base range frequency maximum (unit: Hz).*/
  /* With a timer 16 bits, minimum frequency will be 1/32000 times this value.*/
  #define TIMER_FREQUENCY_RANGE_MAX_HZ    (32000UL)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
LL_DMA_LinkNodeTypeDef Node_GPDMA1_Channel1;
/* Variables for ADC conversion data */
__IO uint16_t uhADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADC group regular conversion data (array of data) */

/* Variables for ADC conversion data computation to physical values */
uint16_t uhADCxConvertedData_Voltage_mVolt[ADC_CONVERTED_DATA_BUFFER_SIZE];  /* Value of voltage calculated from ADC conversion data (unit: mV) (array of data) */

/* Variable to report status of DMA transfer of ADC group regular conversions */
/*  0: DMA transfer is not completed                                          */
/*  1: DMA transfer is completed                                              */
/*  2: DMA transfer has not yet been started yet (initial state)              */
__IO uint8_t ubDmaTransferStatus = 2U; /* Variable set into DMA interruption callback */

LL_DMA_LinkNodeTypeDef pNode1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */

void ADC_Activate(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

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
  uint32_t tmp_index;

  /* Initialize ADC group regular data buffer values */
  for (tmp_index = 0; tmp_index < ADC_CONVERTED_DATA_BUFFER_SIZE; tmp_index++)
  {
    uhADCxConvertedData[tmp_index] = VAR_CONVERTED_DATA_INIT_VALUE;
  }

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
    LL_AHB1_GRP2_EnableClock(LL_AHB1_GRP2_PERIPH_PWR);
  /* Enable the independent analog and I/Os supply */
   LL_PWR_EnableVDDA();
   LL_PWR_EnableVDDIO2();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* Activate ADC */
  /* Perform ADC activation procedure to make it ready to convert. */
  ADC_Activate();

  /* Start ADC group regular conversion */
  /* Note: ADC conversion will effectively start at timer trigger event */
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of this feature is conditioned to    */
  /*       ADC state:                                                         */
  /*       ADC must be enabled without conversion on going on group regular,  */
  /*       without ADC disable command on going.                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if ((LL_ADC_IsEnabled(ADC1) == 1)               &&
      (LL_ADC_IsDisableOngoing(ADC1) == 0)        &&
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    LL_ADC_REG_StartConversion(ADC1);
  }
  else
  {
    /* Error: ADC conversion start could not be performed */
    Error_Handler();
  }

  /* Start time base */
  LL_TIM_EnableCounter(TIM2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* ADC group regular status: toggle LED at buffer conversion completed */
    if(ubDmaTransferStatus == 1)
    {
      LED_On();
    }
    else
    {
      LED_Off();
    }

    /* Note: ADC group regular conversions data are stored into array         */
    /*       "uhADCxConvertedData"                                            */
    /*       (for debug: see variable content into watch window).             */

    /* Note: ADC conversion data are computed to physical values              */
    /*       into array "uhADCxConvertedData_Voltage_mVolt" using             */
    /*       ADC LL driver helper macro "__LL_ADC_CALC_DATA_TO_VOLTAGE()"     */
    /*       (for debug: see variable content with debugger)                  */
    /*       in IRQ handlers callback functions.                              */

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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetADCDACClockSource(LL_RCC_ADCDAC_CLKSOURCE_HCLK);

  /* Peripheral clock enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**ADC1 GPIO Configuration
  PA1   ------> ADC1_IN4
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ADC1 interrupt Init */
  NVIC_SetPriority(ADC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(ADC1_IRQn);

  /* USER CODE BEGIN ADC1_Init 1 */
   LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPDMA1);
  /* DMA node configuration declaration */
  LL_DMA_InitNodeTypeDef NodeConfig = {0};
  LL_DMA_InitLinkedListTypeDef DMA_InitLinkedListStruct = {0};
 
  /* Set node type */

  /* Set node configuration */

  /* GPDMA1_REQUEST_ADC1 Init */
  NodeConfig.DestAllocatedPort = LL_DMA_DEST_ALLOCATED_PORT0;
  NodeConfig.DestHWordExchange = LL_DMA_DEST_HALFWORD_PRESERVE;
  NodeConfig.DestByteExchange = LL_DMA_DEST_BYTE_PRESERVE;
  NodeConfig.DestBurstLength = 1U;
  NodeConfig.DestIncMode = LL_DMA_DEST_INCREMENT;
  NodeConfig.DestDataWidth = LL_DMA_DEST_DATAWIDTH_HALFWORD;
  NodeConfig.SrcAllocatedPort = LL_DMA_SRC_ALLOCATED_PORT0;
  NodeConfig.SrcByteExchange = LL_DMA_SRC_BYTE_PRESERVE;
  NodeConfig.DataAlignment = LL_DMA_DATA_ALIGN_ZEROPADD;
  NodeConfig.SrcBurstLength = 1U;
  NodeConfig.SrcIncMode = LL_DMA_SRC_FIXED;
  NodeConfig.SrcDataWidth = LL_DMA_SRC_DATAWIDTH_WORD;
  NodeConfig.TransferEventMode = LL_DMA_TCEM_BLK_TRANSFER;
  NodeConfig.TriggerPolarity = LL_DMA_TRIG_POLARITY_MASKED;
  NodeConfig.BlkHWRequest = LL_DMA_HWREQUEST_SINGLEBURST;
  NodeConfig.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  NodeConfig.Request = LL_GPDMA1_REQUEST_ADC1;
  NodeConfig.UpdateRegisters = (LL_DMA_UPDATE_CTR1 | LL_DMA_UPDATE_CTR2 | LL_DMA_UPDATE_CBR1 | LL_DMA_UPDATE_CSAR | LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CTR3 | LL_DMA_UPDATE_CBR2 | LL_DMA_UPDATE_CLLR);
  NodeConfig.NodeType = LL_DMA_GPDMA_LINEAR_NODE;
  NodeConfig.Mode = LL_DMA_NORMAL;
  NodeConfig.SrcAddress = (uint32_t)LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA);
  NodeConfig.DestAddress =  (uint32_t)&uhADCxConvertedData;
  NodeConfig.BlkDataLength = ADC_CONVERTED_DATA_BUFFER_SIZE * 4U;
  
  LL_DMA_CreateLinkNode(&NodeConfig, &Node_GPDMA1_Channel1);
  LL_DMA_ConnectLinkNode(&Node_GPDMA1_Channel1, LL_DMA_CLLR_OFFSET5, &Node_GPDMA1_Channel1, LL_DMA_CLLR_OFFSET5);
  LL_DMA_ConfigTransfer( GPDMA1,LL_DMA_CHANNEL_1, LL_DMA_DEST_INCREMENT | LL_DMA_DEST_DATAWIDTH_HALFWORD | LL_DMA_SRC_DATAWIDTH_HALFWORD);
  //
  /* Next function call is commented because it will not compile as is. The Node structure address has to be cast to an unsigned int (uint32_t)pNode_DMAxCHy */
  /*

  */
  
  DMA_InitLinkedListStruct.Priority = LL_DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMA_InitLinkedListStruct.LinkStepMode = LL_DMA_LSM_FULL_EXECUTION;
  DMA_InitLinkedListStruct.LinkAllocatedPort = LL_DMA_LINK_ALLOCATED_PORT0;
  DMA_InitLinkedListStruct.TransferEventMode = LL_DMA_TCEM_BLK_TRANSFER;
  
  LL_DMA_List_Init(GPDMA1, LL_DMA_CHANNEL_1, &DMA_InitLinkedListStruct);
  LL_DMA_SetLinkedListBaseAddr(GPDMA1, LL_DMA_CHANNEL_1, (uint32_t)&Node_GPDMA1_Channel1);

  LL_DMA_ConfigLinkUpdate(GPDMA1, LL_DMA_CHANNEL_1, LL_DMA_UPDATE_CTR1 | LL_DMA_UPDATE_CTR2 |LL_DMA_UPDATE_CBR1 | LL_DMA_UPDATE_CSAR | LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CLLR, (uint32_t)&Node_GPDMA1_Channel1);
  
  LL_DMA_SetLinkedListBaseAddr(GPDMA1, LL_DMA_CHANNEL_1, (uint32_t)&Node_GPDMA1_Channel1);
  
  NVIC_SetPriority(GPDMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
  /* ADC1 interrupt Init */
  NVIC_SetPriority(ADC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(ADC1_IRQn);
  LL_DMA_SetBlkDataLength(GPDMA1,
                          LL_DMA_CHANNEL_1,
                          ADC_CONVERTED_DATA_BUFFER_SIZE);
  /* USER CODE BEGIN ADC1_Init 1 */
  /* Set DMA transfer addresses of source and destination */
  LL_DMA_ConfigAddresses(GPDMA1,
                         LL_DMA_CHANNEL_1,
                         LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                                                  (uint32_t)&uhADCxConvertedData);
  /* Enable DMA transfer interruption: transfer complete */
  LL_DMA_EnableIT_TC(GPDMA1,
                     LL_DMA_CHANNEL_1);

  /* Enable DMA transfer interruption: half transfer */
  LL_DMA_EnableIT_HT(GPDMA1,
                     LL_DMA_CHANNEL_1);

  /* Enable DMA transfer interruption: transfer error */
  LL_DMA_EnableIT_DTE(GPDMA1,
                     LL_DMA_CHANNEL_1);

  /* Enable the DMA transfer */
  LL_DMA_EnableChannel(GPDMA1,
                       LL_DMA_CHANNEL_1);
  
  LL_DMA_SetLinkedListAddrOffset(GPDMA1, LL_DMA_CHANNEL_1, LL_DMA_CLLR_OFFSET5);
  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  ADC_InitStruct.LeftBitShift = LL_ADC_LEFT_BIT_SHIFT_NONE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  LL_ADC_SetGainCompensation(ADC1, 0);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_EXT_TIM2_TRGO;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
  ADC_REG_InitStruct.DataTransferMode = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
  LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);

  /* Disable ADC deep power down (enabled by default after reset state) */
  LL_ADC_DisableDeepPowerDown(ADC1);
  /* Enable ADC internal voltage regulator */
  LL_ADC_EnableInternalRegulator(ADC1);
  /* Delay for ADC internal voltage regulator stabilization. */
  /* Compute number of CPU cycles to wait for, from delay in us. */
  /* Note: Variable divided by 2 to compensate partially */
  /* CPU processing cycles (depends on compilation optimization). */
  /* Note: If system core clock frequency is below 200kHz, wait time */
  /* is only a few CPU processing cycles. */
  __IO uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_4);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_246CYCLES_5);
  LL_ADC_SetChannelPreselection(ADC1, LL_ADC_CHANNEL_4);
  /* USER CODE BEGIN ADC1_Init 2 */

  /* Configuration of ADC interruptions */
  /* Enable interruption ADC group regular overrun */
  LL_ADC_EnableIT_OVR(ADC1);

  /* USER CODE END ADC1_Init 2 */

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
  uint32_t tim_prescaler;
  uint32_t tim_period;
  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* Set timer prescaler value (timer frequency) */
  /* Note: Value TIMER_FREQUENCY_RANGE_MAX_HZ with factor 2 to have a minimum
           timer resolution */
  tim_prescaler = __LL_TIM_CALC_PSC(SystemCoreClock, TIMER_FREQUENCY_RANGE_MAX_HZ * 2);

  /* Set timer period value (time base frequency) */
  tim_period = __LL_TIM_CALC_ARR(SystemCoreClock, tim_prescaler, TIMER_FREQUENCY_HZ);

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = tim_prescaler;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = tim_period;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);

  /**/
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: ADC1).
  * @param  None
  * @retval None
  */
void ADC_Activate(void)
{
  __IO uint32_t wait_loop_index = 0U;
  #if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0U; /* Variable used for timeout management */
  #endif /* USE_TIMEOUT */

  /*## Operation on ADC hierarchical scope: ADC instance #####################*/

  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 series, setting of these features is conditioned to  */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Disable ADC deep power down (enabled by default after reset state) */
    LL_ADC_DisableDeepPowerDown(ADC1);

    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(ADC1);

    /* Delay for ADC internal voltage regulator stabilization.                */
    /* Compute number of CPU cycles to wait for, from delay in us.            */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    /* Note: If system core clock frequency is below 200kHz, wait time        */
    /*       is only a few CPU processing cycles.                             */
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1);

    /* Poll for ADC effectively calibrated */
    #if (USE_TIMEOUT == 1)
    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
    #endif /* USE_TIMEOUT */

    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
    {
    #if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if(Timeout-- == 0)
        {
          /* Error: Time-out */
          Error_Handler();
        }
      }
    #endif /* USE_TIMEOUT */
    }

    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    /* Enable ADC */
    LL_ADC_Enable(ADC1);

    /* Poll for ADC ready to convert */
    #if (USE_TIMEOUT == 1)
    Timeout = ADC_ENABLE_TIMEOUT_MS;
    #endif /* USE_TIMEOUT */

    while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
    {
    #if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if(Timeout-- == 0)
        {
          /* Error: Time-out */
          Error_Handler();
        }
      }
    #endif /* USE_TIMEOUT */
    }

    /* Note: ADC flag ADRDY is not cleared here to be able to check ADC       */
    /*       status afterwards.                                               */
    /*       This flag should be cleared at ADC Deactivation, before a new    */
    /*       ADC activation, using function "LL_ADC_ClearFlag_ADRDY()".       */
  }

  /*## Operation on ADC hierarchical scope: ADC group regular ################*/
  /* Note: No operation on ADC group regular performed here.                  */
  /*       ADC group regular conversions to be performed after this function  */
  /*       using function:                                                    */
  /*       "LL_ADC_REG_StartConversion();"                                    */

  /*## Operation on ADC hierarchical scope: ADC group injected ###############*/
  /* Note: Feature not available on this STM32 series */

}

/**
  * @brief  Turn-on LD2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED on */
  LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Turn-off LD2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED off */
  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);
}

/**
  * @brief  Toggle LD2.
  * @param  None
  * @retval None
  */
void LED_Toggle(void)
{
  /* Turn LED off */
  LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/

/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
void AdcDmaTransferComplete_Callback()
{
  uint32_t tmp_index;

  /* Computation of ADC conversions raw data to physical values
     using LL ADC driver helper macro. */
  for (tmp_index = 0; tmp_index < (ADC_CONVERTED_DATA_BUFFER_SIZE); tmp_index++)
  {
    uhADCxConvertedData_Voltage_mVolt[tmp_index] = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxConvertedData[tmp_index], LL_ADC_RESOLUTION_12B);
  }

  /* Update status variable of DMA transfer */
  ubDmaTransferStatus = 1;
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
void AdcDmaTransferError_Callback()
{
  if(ubDmaTransferStatus == 1)
  {
    /* Update status variable of DMA transfer */
    ubDmaTransferStatus = 0;
  }

  /* Error detected during DMA transfer */
  Error_Handler();
}

/**
  * @brief  ADC group regular overrun interruption callback
  * @note   This function is executed when ADC group regular
  *         overrun error occurs.
  * @retval None
  */
void AdcGrpRegularOverrunError_Callback(void)
{
  /* Note: Disable ADC interruption that caused this error before entering in
           infinite loop below. */

  /* In case of error due to overrun: Disable ADC group regular overrun interruption */
  LL_ADC_DisableIT_OVR(ADC1);

  /* Error reporting */
  Error_Handler();
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

  /* Turn on LED and remain in infinite loop */
  while (1)
  {
    LED_On();
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
