/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram_TrustZone/Secure/Src/main.c
  * @author  MCD Application Team
  * @brief   Main secure program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
/* Non-secure Vector table to jump to (internal Flash Bank2 here)             */
/* Caution: address must correspond to non-secure internal Flash where is     */
/*          mapped in the non-secure vector table                             */
#define VTOR_TABLE_NS_START_ADDR    0x08100000UL

#define FLASH_USER_SEC_START_ADDR   (FLASH_BASE + (FLASH_PAGE_SIZE * 4))                        /* Start @ of user Flash area in Bank1 */
#define FLASH_USER_SEC_END_ADDR     (FLASH_BASE + FLASH_BANK_SIZE - FLASH_PAGE_SIZE*2 - 1)      /* End @ of user Flash area (before NSC area) */

#define FLASH_USER_NS_START_ADDR    ((FLASH_BASE_NS + FLASH_BANK_SIZE) + (FLASH_PAGE_SIZE * 4)) /* Start @ of user Flash area in Bank2 */
#define FLASH_USER_NS_END_ADDR      (FLASH_BASE_NS + FLASH_SIZE - 1)                            /* End @ of user Flash area in Bank2   */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern funcptr_NS pSecureErrorCallback;
/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t MemoryProgramStatus = 0;
uint32_t DoubleWord[2] = {0x12345678,
                          0x12345678};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void NonSecure_Init(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_GTZC_S_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */
static uint32_t GetPage(uint32_t Address);
static uint32_t GetPage_NS(uint32_t Addr);
static uint32_t GetBank(uint32_t Address);
static uint32_t GetBank_NS(uint32_t Addr);
static uint32_t Check_Program(uint32_t StartAddress, uint32_t EndAddress, uint32_t *Data);
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
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* Initialize LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  /* GTZC initialisation */
  MX_GTZC_S_Init();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* Disable instruction cache prior to internal cacheable memory update   */  
  if (HAL_ICACHE_Disable() != HAL_OK)
  {
    Error_Handler();
  }

  /* Switch to voltage scaling range 1 to allow FLASH operations */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

 /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Erase the user Flash area
    (area defined by FLASH_USER_SEC_START_ADDR and FLASH_USER_SEC_END_ADDR) ***********/

  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_SEC_START_ADDR);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_SEC_END_ADDR) - FirstPage + 1;

  /* Get the bank */
  BankNumber = GetBank(FLASH_USER_SEC_START_ADDR);

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks         = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    Error_Handler();
  }

  /* Program the user Flash area word by word
    (area defined by FLASH_USER_SEC_START_ADDR and FLASH_USER_SEC_END_ADDR) ***********/

  Address = FLASH_USER_SEC_START_ADDR;

  while (Address < FLASH_USER_SEC_END_ADDR)
  {
     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, ((uint32_t)DoubleWord)) == HAL_OK)
    {
      Address = Address + 8;  /* increment to the next Flash word */
    }
    else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
      Error_Handler();
    }
  }

  /* Enable instruction cache with no security attribute at GTZC level  */
  /* to let the instruction cache being updated by the non-secure       */
  /* application to insure memory programming into cacheable memory     */
  /* with cache disabled                                                */
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }

  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of doublewords not programmed correctly ******/
  MemoryProgramStatus = Check_Program(FLASH_USER_SEC_START_ADDR, FLASH_USER_SEC_END_ADDR, DoubleWord);
  if( MemoryProgramStatus != 0)
  {
    Error_Handler();
  }

  /* In this part of example, flash Bank2 non-secure will be erased        */
  /* from Bank1 secure. Once this operation is finished, page double-word  */
  /* programming operation will be performed in the non-secure part of     */
  /* the Flash memory. The written data is then read back and checked      */
  
  /* Disable instruction cache prior to internal cacheable memory update   */  
  if (HAL_ICACHE_Disable() != HAL_OK)
  {
    Error_Handler();
  }

   /* Get the 1st page to erase */
  FirstPage   = GetPage_NS(FLASH_USER_NS_START_ADDR);

   /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage_NS(FLASH_USER_NS_END_ADDR) - FirstPage + 1;

  /* Get the bank */
  BankNumber = GetBank_NS(FLASH_USER_NS_START_ADDR);

  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES_NS;
  EraseInitStruct.Banks     = BankNumber;
  EraseInitStruct.Page    = FirstPage;
  EraseInitStruct.NbPages   = NbOfPages;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    /* Error occurred while page erase */
    Error_Handler();
  }

  Address = FLASH_USER_NS_START_ADDR;

  while (Address < FLASH_USER_NS_END_ADDR)
  {
     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD_NS, Address, ((uint32_t)DoubleWord)) == HAL_OK)
    {
      Address = Address + 8;  /* increment to the next Flash word */
    }
    else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
      Error_Handler();
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Enable instruction cache with no security attribute at GTZC level  */
  /* to let the instruction cache being updated by the non-secure       */
  /* application to insure memory programming into cacheable memory     */
  /* with cache disabled                                                */
  MX_ICACHE_Init();

  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  MemoryProgramStatus = Check_Program(FLASH_USER_NS_START_ADDR, FLASH_USER_NS_END_ADDR, DoubleWord);
  /* Check if there is an issue to program data */
  if (MemoryProgramStatus != 0)
  {
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Secure SysTick should rather be suspended before calling non-secure  */
  /* in order to avoid wake-up from sleep mode entered by non-secure      */
  /* The Secure SysTick shall be resumed on non-secure callable functions */
  HAL_SuspendTick();

  /*************** Setup and jump to non-secure *******************************/

  NonSecure_Init();

  /* Non-secure software does not return, this code is not executed */

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
  * @brief  Non-secure call function
  *         This function is responsible for Non-secure initialization and switch
  *         to non-secure state
  * @retval None
  */
static void NonSecure_Init(void)
{
  funcptr_NS NonSecure_ResetHandler;

  SCB_NS->VTOR = VTOR_TABLE_NS_START_ADDR;

  /* Set non-secure main stack (MSP_NS) */
  __TZ_set_MSP_NS((*(uint32_t *)VTOR_TABLE_NS_START_ADDR));

  /* Get non-secure reset handler */
  NonSecure_ResetHandler = (funcptr_NS)(*((uint32_t *)((VTOR_TABLE_NS_START_ADDR) + 4U)));

  /* Start non-secure state software application */
  NonSecure_ResetHandler();

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
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_3);

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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_ConfigAttributes(RCC_SYSCLK, RCC_SEC_NPRIV);
}

/**
  * @brief GTZC_S Initialization Function
  * @param None
  * @retval None
  */
static void MX_GTZC_S_Init(void)
{

  /* USER CODE BEGIN GTZC_S_Init 0 */

  /* USER CODE END GTZC_S_Init 0 */

  MPCBB_ConfigTypeDef MPCBB_Area_Desc = {0};

  /* USER CODE BEGIN GTZC_S_Init 1 */

  /* USER CODE END GTZC_S_Init 1 */
  MPCBB_Area_Desc.SecureRWIllegalMode = GTZC_MPCBB_SRWILADIS_ENABLE;
  MPCBB_Area_Desc.InvertSecureState = GTZC_MPCBB_INVSECSTATE_NOT_INVERTED;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[0] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[1] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[2] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[3] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[4] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[5] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[6] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[7] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[8] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[9] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[10] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[11] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[0] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[1] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[2] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[3] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[4] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[5] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[6] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[7] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[8] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[9] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[10] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[11] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_LockConfig_array[0] =   0x00000000;
  if (HAL_GTZC_MPCBB_ConfigMem(SRAM1_BASE, &MPCBB_Area_Desc) != HAL_OK)
  {
    Error_Handler();
  }
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[0] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[1] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[2] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_SecConfig_array[3] =   0xFFFFFFFF;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[0] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[1] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[2] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_PrivConfig_array[3] =   0x00000000;
  MPCBB_Area_Desc.AttributeConfig.MPCBB_LockConfig_array[0] =   0x00000000;
  if (HAL_GTZC_MPCBB_ConfigMem(SRAM2_BASE, &MPCBB_Area_Desc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GTZC_S_Init 2 */

  /* Clear all illegal access flags in GTZC TZIC */
  if(HAL_GTZC_TZIC_ClearFlag(GTZC_PERIPH_ALL) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Enable all illegal access interrupts in GTZC TZIC */
  if(HAL_GTZC_TZIC_EnableIT(GTZC_PERIPH_ALL) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  /* USER CODE END GTZC_S_Init 2 */

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

  /*IO attributes management functions */
  HAL_GPIO_ConfigPinAttributes(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_NSEC);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  GTZC TZIC interrupt callback.
  * @param  PeriphId Peripheral identifier triggering the illegal access.
  *         This parameter can be a value of @ref GTZC_TZSC_TZIC_PeriphId
  * @retval None.
  */
void HAL_GTZC_TZIC_Callback(uint32_t PeriphId)
{
  funcptr_NS callback_NS; /* non-secure callback function pointer */

  /* Prevent unused argument(s) compilation warning */
  UNUSED(PeriphId);

  if(pSecureErrorCallback != (funcptr_NS)NULL)
  {
   /* return function pointer with cleared LSB */
   callback_NS = (funcptr_NS)cmse_nsfptr_create(pSecureErrorCallback);

   callback_NS();
  }
  else
  {
    /* Something went wrong in test case */
    while(1);
  }
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Address)
{
  uint32_t page = 0;

  if((Address >= FLASH_BASE) && (Address < FLASH_BASE + FLASH_BANK_SIZE))
  {
    page = (Address & ~FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else if ((Address >= FLASH_BASE + FLASH_BANK_SIZE) && (Address < FLASH_BASE + FLASH_SIZE))
  {
    page = ((Address & ~FLASH_BASE) - FLASH_BANK_SIZE) / FLASH_PAGE_SIZE;
  }
  else
  {
    page = 0xFFFFFFFF; /* Address out of range */
  }

  return page;
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage_NS(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE_NS + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE_NS) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE_NS + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;

  if((Addr >= FLASH_BASE) && (Addr < FLASH_BASE + FLASH_BANK_SIZE))
  {
    bank = FLASH_BANK_1;
  }
  else if ((Addr >= FLASH_BASE + FLASH_BANK_SIZE) && (Addr < FLASH_BASE + FLASH_SIZE))
  {
    bank = FLASH_BANK_2;
  }
  else
  {
    bank = 0xFFFFFFFF; /* Address out of range */
  }

  return bank;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank_NS(uint32_t Addr)
{
  uint32_t bank = 0;

  if((Addr >= FLASH_BASE_NS) && (Addr < FLASH_BASE_NS + FLASH_BANK_SIZE))
  {
    bank = FLASH_BANK_1;
  }
  else if ((Addr >= FLASH_BASE_NS + FLASH_BANK_SIZE) && (Addr < FLASH_BASE_NS + FLASH_SIZE))
  {
    bank = FLASH_BANK_2;
  }
  else
  {
    bank = 0xFFFFFFFF; /* Address out of range */
  }

  return bank;
}

/**
  * @brief  Check program operation.
  * param StartAddress Area start address
  * param EndAddress Area end address
  * param Data Expected data
  * @retval FailCounter
  */
static uint32_t Check_Program(uint32_t StartAddress, uint32_t EndAddress, uint32_t *Data)
{
  uint32_t Address;
  uint32_t index, FailCounter = 0;
  uint32_t data32;
  uint32_t size_word = 2;

  /* check the user Flash area word by word */
  Address = StartAddress;

  while(Address < EndAddress)
  {
    for(index = 0; index < size_word; index++)
    {
      data32 = *(uint32_t*)Address;
      if(data32 != Data[index])
      {
        FailCounter++;
        return FailCounter;
      }
      Address += 4;
    }
  }
  return FailCounter;
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
    /* Make LED_RED blink (100ms on, 2s off) to indicate an error occurred */
    BSP_LED_On(LED_RED);
    HAL_Delay(100);
    BSP_LED_Off(LED_RED);
    HAL_Delay(2000);
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
