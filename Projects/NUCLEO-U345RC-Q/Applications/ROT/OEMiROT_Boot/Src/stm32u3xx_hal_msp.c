/**
  ******************************************************************************
  * @file    stm32u3xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"

/** @addtogroup STM32U3xx_HAL_Examples
  * @{
  */

/** @defgroup RTC_ActiveTamper
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constant ----------------------------------------------------------*/
/** @defgroup HAL_MSP_Private_Constant
  * @{
  */
const  RCC_OscInitTypeDef RCC_OscInitStruct_RTC =
{
  .OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE,
  .HSEState = 0,
#if defined(RTC_CLOCK_SOURCE_LSE)
  .LSEState = RCC_LSE_ON,
  .LSIState = RCC_LSI_OFF,
#elif defined (RTC_CLOCK_SOURCE_LSI)
  .LSEState = RCC_LSE_OFF,
  .LSIState = RCC_LSI_ON,
#else
#error
#endif /* defined (RTC_CLOCK_SOURCE_LSI) */
  .HSIState = 0,
  .HSICalibrationValue = 0,
  .LSIDiv = 0,
  .HSI48State = 0,
  .MSIKState = 0,
};

const RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct_RTC =
{
  .PeriphClockSelection = RCC_PERIPHCLK_RTC,
  .Usart1ClockSelection = 0,
#if defined(USART2)
  .Usart2ClockSelection = 0,
#endif /* defined(USART2) */
  .Usart3ClockSelection = 0,
  .Uart4ClockSelection = 0,
  .Uart5ClockSelection = 0,
  .Lpuart1ClockSelection = 0,
  .I2c1ClockSelection = 0,
#if defined(I2C2)
  .I2c2ClockSelection = 0,
#endif
  .I2c3ClockSelection = 0,
  .Lptim1ClockSelection = 0,
  .Lptim2ClockSelection = 0,
  .Lptim34ClockSelection = 0,
#if defined(FDCAN1)
  .FdcanClockSelection = 0,
#endif
#if defined(ADF1)
  .Adf1ClockSelection = 0,
#endif
#if defined(SAI1)
  .Sai1ClockSelection = 0,
#endif
#if defined (SAI2)
  .Sai2ClockSelection = 0,
#endif /* defined (SAI2) */
  .RngClockSelection = 0,
  .IclkClockSelection = 0,
  .AdcDacClockSelection = 0,
  .Spi1ClockSelection = 0,
  .Spi2ClockSelection = 0,
#if defined(SPI3)
  .Spi3ClockSelection = 0,
#endif /* SPI3 */
#if defined(RTC_CLOCK_SOURCE_LSE)
  .RTCClockSelection = RCC_RTCCLKSOURCE_LSE,
#elif defined (RTC_CLOCK_SOURCE_LSI)
  .RTCClockSelection = RCC_RTCCLKSOURCE_LSI,
#else
#error
#endif /* RTC_CLOCK_SOURCE_LSE */
};
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
#if defined(RTC_CLOCK_SOURCE_LSE)
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
#endif

  HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1);
  HAL_PWREx_EnableEpodBooster();

  /* Configure the main internal regulator output voltage*/
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /* Reset the whole backup domain, RTC included */
  if (HAL_RCC_OscConfig((RCC_OscInitTypeDef *)&RCC_OscInitStruct_RTC) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure RTC clock only if the RTC clock is not set */
  if ((RCC->BDCR & RCC_BDCR_RTCSEL_Msk) == RCC_RTCCLKSOURCE_DISABLE)
  {
    if (HAL_RCCEx_PeriphCLKConfig((RCC_PeriphCLKInitTypeDef *)&PeriphClkInitStruct_RTC) != HAL_OK)
    {
      Error_Handler();
    }
    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();
  }

  HAL_NVIC_SetPriority(TAMP_IRQn, 0x4, 0);
  HAL_NVIC_EnableIRQ(TAMP_IRQn);
}

/**
  * @brief RTC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  __HAL_RCC_RTC_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
