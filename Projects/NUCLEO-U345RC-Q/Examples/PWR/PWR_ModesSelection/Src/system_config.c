/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/system_config.c
  * @author  MCD Application Team
  * @brief   This file provides system configuration.
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
/* USER CODE BEGIN 1 */
/* Includes ------------------------------------------------------------------*/
#include "system_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USER_MSI_CALIBRATION  0U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;

/* Private function prototypes -----------------------------------------------*/
static System_StatusTypeDef SystemClock_12MHz(void);
static System_StatusTypeDef SystemClock_48MHz(void);
static System_StatusTypeDef SystemClock_96MHz(void);
static System_StatusTypeDef RTC_Enable(void);
static System_StatusTypeDef RTC_Disable(void);

extern UART_HandleTypeDef huart1;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Optimize system power consumption configuration.
  * @param  None.
  * @retval System Status.
  */
void system_config(System_LowPowerModeScenarioTypeDef scenario)
{
  /* *** Common pre-setup for all modes *** */

  /* Enforcing true STOP and STANDBY modes, by disabling their emulation. */
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();

  /* Disable unused VDDs */
  HAL_PWREx_DisableVddA();
  HAL_PWREx_DisableVddIO2();
  HAL_PWREx_DisableVddUSB();
  HAL_PWREx_DisableBatteryCharging();

  /* Disable Unused Internal clocks */
  HAL_RCCEx_DisableKernelClkInStop(RCC_KERNELCLK_MSIK);
  HAL_RCCEx_DisableKernelClkInStop(RCC_KERNELCLK_HSI);

  /* *** End of Common pre-setup *** */

  /* *** Specific setup for each mode *** */

  switch (scenario)
  {
    /* Sleep 12 MHz */
    case SYSTEM_LOWPOWER_S2:
    {
      CLEAR_REG(RCC->AHB1SLPENR1);
      CLEAR_REG(RCC->AHB1SLPENR2);
      CLEAR_REG(RCC->AHB2SLPENR1);
      CLEAR_REG(RCC->AHB2SLPENR2);
      CLEAR_REG(RCC->APB1SLPENR1);
      CLEAR_REG(RCC->APB1SLPENR2);
      CLEAR_REG(RCC->APB2SLPENR);
      CLEAR_REG(RCC->APB3SLPENR);
      break;
    }

    /* STOP1 - Full SRAM (SRAM 1 + 2 = 256kb) */
    case SYSTEM_LOWPOWER_S3:
    {
      CLEAR_REG(RCC->AHB1STPENR1);
      CLEAR_REG(RCC->AHB2STPENR1);
      CLEAR_REG(RCC->APB1STPENR1);
      CLEAR_REG(RCC->APB1STPENR2);
      CLEAR_REG(RCC->APB2STPENR);
      CLEAR_REG(RCC->APB3STPENR);
      break;
    }

    /* STOP1 - 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S4:
    {
      CLEAR_REG(RCC->AHB1STPENR1);
      CLEAR_REG(RCC->AHB2STPENR1);
      CLEAR_REG(RCC->APB1STPENR1);
      CLEAR_REG(RCC->APB1STPENR2);
      CLEAR_REG(RCC->APB2STPENR);
      CLEAR_REG(RCC->APB3STPENR);
      break;
    }

    /* STOP2 - Full SRAM (SRAM 1 + 2 = 256kb) */
    case SYSTEM_LOWPOWER_S5:
    {
      CLEAR_REG(RCC->AHB1STPENR1);
      CLEAR_REG(RCC->AHB2STPENR1);
      CLEAR_REG(RCC->APB1STPENR1);
      CLEAR_REG(RCC->APB1STPENR2);
      CLEAR_REG(RCC->APB2STPENR);
      CLEAR_REG(RCC->APB3STPENR);
      break;
    }

    /* STOP2 - 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S6:
    {
      CLEAR_REG(RCC->AHB1STPENR1);
      CLEAR_REG(RCC->AHB2STPENR1);
      CLEAR_REG(RCC->APB1STPENR1);
      CLEAR_REG(RCC->APB1STPENR2);
      CLEAR_REG(RCC->APB2STPENR);
      CLEAR_REG(RCC->APB3STPENR);
      break;
    }

    /* STOP3 - Full SRAM (SRAM 1 + 2 = 256kb) */
    case SYSTEM_LOWPOWER_S7:
    {
      CLEAR_REG(RCC->AHB1STPENR1);
      CLEAR_REG(RCC->AHB2STPENR1);
      CLEAR_REG(RCC->APB1STPENR1);
      CLEAR_REG(RCC->APB1STPENR2);
      CLEAR_REG(RCC->APB2STPENR);
      CLEAR_REG(RCC->APB3STPENR);
      break;
    }

    /* STOP3 - 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S8:
    {
      CLEAR_REG(RCC->AHB1STPENR1);
      CLEAR_REG(RCC->AHB2STPENR1);
      CLEAR_REG(RCC->APB1STPENR1);
      CLEAR_REG(RCC->APB1STPENR2);
      CLEAR_REG(RCC->APB2STPENR);
      CLEAR_REG(RCC->APB3STPENR);
      break;
    }

    default:
    {
      /* Do nothing */
      break;
    }
  }

  /* *** End of specific setup for each mode *** */
}

/**
  * @brief  Configure rtc state.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_rtc_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  switch (scenario)
  {
    /* Standby - 8kb SRAM retention + RTC + ULPEN=1 */
    case SYSTEM_LOWPOWER_S9:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      break;
    }

    /* Standby - No SRAM retention + RTC + ULPEN=1 */
    case SYSTEM_LOWPOWER_S10:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      break;
    }

    /* Shutdown w/ RTC */
    case SYSTEM_LOWPOWER_S12:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }

      break;
    }

    /* Disable RTC */
    default:
    {
      if (RTC_Disable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system clock frequency.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_clock_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* If it uses RTC, preset the RTCPBAEN bit */
  if((scenario == SYSTEM_LOWPOWER_S9) || (scenario == SYSTEM_LOWPOWER_S10) || (scenario == SYSTEM_LOWPOWER_S12))
  {
    SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_RTCAPBEN);
  }

  switch (scenario)
  {
    /* Run 96MHz */
    case SYSTEM_LOWPOWER_S0:
    {
      if (SystemClock_96MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }

    /* Run 48MHz */
    case SYSTEM_LOWPOWER_S1:
    {
      if (SystemClock_48MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }

    /* Sleep 12MHz */
    case SYSTEM_LOWPOWER_S2:
    {
      if (SystemClock_12MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }

    default:
    {
      if (SystemClock_12MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure SRAM retention.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_ram_retention_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Disable cache and clear flags for low power modes */
  if (scenario >= SYSTEM_LOWPOWER_S2)
  {
    HAL_ICACHE_Disable();
    HAL_ICACHE_DeInit();
    __HAL_ICACHE_CLEAR_FLAG(ICACHE_FLAG_BUSYEND | ICACHE_FLAG_ERROR);
  }

  /* Disable every retention by default before re-enabling mandatory ones */
  if ((scenario > SYSTEM_LOWPOWER_S3) && (scenario != SYSTEM_LOWPOWER_S5) && (scenario != SYSTEM_LOWPOWER_S7))
  {
  __HAL_RCC_SRAM1_CLK_DISABLE();
  HAL_PWREx_EnableRAMsPowerDown(PWR_SRAM1_POWERDOWN);
  }

  HAL_PWREx_DisableSRAM2ContentStandbyRetention(PWR_SRAM2_FULL_STANDBY_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM2_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_FDCAN_USB_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_ICACHE_STOP_RETENTION);
#if defined(PWR_PKA_STOP_RETENTION)
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_PKA_STOP_RETENTION);
#endif/*PWR_PKA_STOP_RETENTION*/
  HAL_PWREx_DisableSRAMFastWakeUp();

  /* Wait that default config is effective */
  __DSB();

  /* Set specific configurations */
  switch (scenario)
  {
    /* Stop 1 - 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S4:
    {
      HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_PAGE3_STOP_RETENTION);
      break;
    }

    /* Stop 2 - 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S6:
    {
      HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_PAGE3_STOP_RETENTION);
      break;
    }

    /* Stop 3 - 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S8:
    {
      HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_PAGE3_STOP_RETENTION);
      break;
    }

    /* Standby - 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S9:
    {
      HAL_PWREx_EnableSRAM2ContentStandbyRetention(PWR_SRAM2_PAGE3_STANDBY_RETENTION);
      break;
    }

    /* Standby - No retention */
    case SYSTEM_LOWPOWER_S10:
    {
      /* Do nothing */
      break;
    }

    /* Standby - No retention */
    case SYSTEM_LOWPOWER_S11:
    {
      /* Do nothing */
      break;
    }

    /* Shutdown - No retention */
    case SYSTEM_LOWPOWER_S12:
    {
      /* Do Nothing */
      break;
    }

    /* Shutdown - No retention */
    case SYSTEM_LOWPOWER_S13:
    {
      /* Do Nothing */
      break;
    }

    /* full SRAM (SRAM 1 + 2 = 256kb) */
    default:
    {
      HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
      HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_FULL_STOP_RETENTION);
      break;
    }
  }

  /* Wait that default config is effective */
  __DSB();

  return status;
}

/**
  * @brief  Configure system clock frequency to 12MHz (RC1 range 5).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_12MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_ALL;
  RCC_OscInitStruct.MSISSource          = RCC_MSI_RC1;
  RCC_OscInitStruct.MSISDiv             = RCC_MSI_DIV2;
  RCC_OscInitStruct.MSISState           = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKState           = RCC_MSI_OFF;
  RCC_OscInitStruct.HSIState            = RCC_HSI_OFF;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.LSIState            = RCC_LSI_OFF;
  if(READ_BIT(RCC->APB1ENR1, RCC_APB1ENR1_RTCAPBEN) == RCC_APB1ENR1_RTCAPBEN)
  {
    RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
  }
  else
  {
    RCC_OscInitStruct.LSEState            = RCC_LSE_OFF;
  }
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_OFF;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Disable booster */
  if (HAL_PWREx_DisableEpodBooster() != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1/2/3
  & clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                 RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0))
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Enable MSI Bias sampling mode */
  SET_BIT(RCC->ICSCR1, RCC_ICSCR1_MSIBIAS);

  return SYSTEM_OK;
}

/**
  * @brief  Configure system clock frequency to 48MHz (RC0 Range 1).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_48MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Set CLK Source and div */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_ALL;
  RCC_OscInitStruct.MSISSource          = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISState           = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKState           = RCC_MSI_OFF;
  RCC_OscInitStruct.HSIState            = RCC_HSI_OFF;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.LSEState            = RCC_LSE_OFF;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_OFF;
  RCC_OscInitStruct.MSISDiv             = RCC_MSI_DIV4; /* 24 MHz */

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Change Voltage scale */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure EPOD settings in RCC */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Enable booster */
  if (HAL_PWREx_EnableEpodBooster() != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Apply DIV2 to go back to 48 MHz */
  RCC_OscInitStruct.MSISDiv             = RCC_MSI_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1/2/3
  & clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                 RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2))
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  /* Change Voltage scale */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Enable MSI Bias sampling mode */
  SET_BIT(RCC->ICSCR1, RCC_ICSCR1_MSIBIAS);

  return SYSTEM_OK;
}

/**
  * @brief  Configure system clock frequency to 96MHz (RC0 range 0).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_96MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_ALL;
  RCC_OscInitStruct.MSISSource          = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISState           = RCC_MSI_ON;
  RCC_OscInitStruct.MSIKState           = RCC_MSI_OFF;
  RCC_OscInitStruct.HSIState            = RCC_HSI_OFF;
  RCC_OscInitStruct.HSEState            = RCC_HSE_OFF;
  RCC_OscInitStruct.LSEState            = RCC_LSE_OFF;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_OFF;
  RCC_OscInitStruct.MSISDiv             = RCC_MSI_DIV4; /* MSI = 24MHz */

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Change Voltage scale */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure EPOD settings in RCC */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Enable booster */
  if (HAL_PWREx_EnableEpodBooster() != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1/2/3
  & clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                 RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4))
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  /* Apply DIV1 to go back to 96 MHz */
  RCC_OscInitStruct.MSISDiv             = RCC_MSI_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Enable MSI Bias sampling mode */
  SET_BIT(RCC->ICSCR1, RCC_ICSCR1_MSIBIAS);

  return SYSTEM_OK;
}

/**
  * @brief  Configure the RTC peripheral and Initialize it.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Enable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Enable RTC Clock */
  if (READ_BIT(RCC->BDCR, RCC_BDCR_RTCEN) == 0U)
  {
    SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
  }

  /* Enable Backup domain access, PWR_DBPCR_DBP enable */
  HAL_PWR_EnableBkUpAccess();

  /* Enable LSE and RTC Clocks */
  __HAL_RCC_RTC_ENABLE();
  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);

  /* Disable LSI */
  __HAL_RCC_LSI_DISABLE();

  /* Set parameter to be configured */
  RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv   = 0x7FU;
  RTCHandle.Init.SynchPrediv    = 0xFFU;
  RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  RTCHandle.Init.OutPutPullUp   = RTC_OUTPUT_PULLUP_NONE;

  /* Initialize the RTC peripheral */
  if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Set Calendar Ultra-Low power mode */
  if (HAL_RTCEx_SetLowPowerCalib(&RTCHandle, RTC_LPCAL_SET) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  if (HAL_RTCEx_DeactivateCalibrationOutPut(&RTCHandle) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Disable Backup access */
  HAL_PWR_DisableBkUpAccess();

  return SYSTEM_OK;
}

/**
  * @brief  De-Initialize the RTC peripheral.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Disable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Check RTC state */
  if (HAL_RTC_GetState(&RTCHandle) != HAL_RTC_STATE_RESET)
  {
    /* De-Initialize the RTC peripheral */
    if (HAL_RTC_DeInit(&RTCHandle) != HAL_OK)
    {
      return SYSTEM_ERROR;
    }
  }

  /* Enable Backup domain access, PWR_DBPCR_DBP enable */
  HAL_PWR_EnableBkUpAccess();

  /* Disable LSI */
  __HAL_RCC_LSI_DISABLE();

  /* Enable LSI Clocks */
  __HAL_RCC_LSE_CONFIG(RCC_LSE_OFF);
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == 1U);

  /* Select LSI Clock for RTC */
  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);

  /* Disable RTC Clock */
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_RTCEN);

  /* Disable backup access */
  HAL_PWR_DisableBkUpAccess();

  return SYSTEM_OK;
}

/**
  * @brief  Configure system low power mode.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_lowpower_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock if mandatory */
  if (READ_BIT(RCC->AHB1ENR2, RCC_AHB1ENR2_PWREN) == 0U)
  {
    __HAL_RCC_PWR_CLK_ENABLE();
  }

  /* Enable Flash Low Power Read Mode */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

  /* Reduce Flash power consumption */
  if (HAL_FLASHEx_EnablePowerDown(FLASH_BANK_2) != HAL_OK)
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  if (HAL_FLASHEx_ConfigLowPowerRead(FLASH_LPM_ENABLE) != HAL_OK)
  {
    /* HAL RCC configuration error */
    return SYSTEM_ERROR;
  }

  if (READ_BIT(RCC->AHB1ENR2, RCC_AHB1ENR1_SRAM1EN) == 1U)
  {
    /* Disable SRAM1 clock */
    CLEAR_BIT(RCC->AHB1ENR2, RCC_AHB1ENR1_SRAM1EN);
  }

  /* Suspend Systick */
  HAL_SuspendTick();

  /* Empty buffers before reaching the final mode */
  __DSB();

  switch (scenario)
  {
    /* Sleep 12 MHz */
    case SYSTEM_LOWPOWER_S2:
    {
      /* Enter in Sleep Mode */
      HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Stop 1 mode with full SRAM (SRAM 1 + 2 = 256kb) */
    case SYSTEM_LOWPOWER_S3:
    {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP1, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Stop 1 mode with 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S4:
    {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP1, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Stop 2 mode with full SRAM (SRAM 1 + 2 = 256kb) */
    case SYSTEM_LOWPOWER_S5:
    {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP2, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Stop 2 mode with 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S6:
    {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP2, PWR_SLEEPENTRY_WFI);
      break;
    }

    /* Stop 3 mode with full SRAM (SRAM 1 + 2 = 256kb) */
    case SYSTEM_LOWPOWER_S7:
    {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP3, PWR_STOPENTRY_WFI);
      break;
    }

    /* Stop 3 mode with 8-Kbyte SRAM */
    case SYSTEM_LOWPOWER_S8:
    {
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP3, PWR_STOPENTRY_WFI);
      break;
    }

    /* Standby - 8kb SRAM retention + RTC + ULPEN=1 */
    case SYSTEM_LOWPOWER_S9:
    {
      HAL_PWR_EnterSTANDBYMode();
      break;
    }

    /* Standby - No SRAM retention + RTC + ULPEN=1 */
    case SYSTEM_LOWPOWER_S10:
    {
      HAL_PWR_EnterSTANDBYMode();
      break;
    }

    /* Standby - No SRAM retention + No RTC + ULPEN=1 */
    case SYSTEM_LOWPOWER_S11:
    {
      HAL_PWR_EnterSTANDBYMode();
      break;
    }

    /* Shutdown w/ RTC */
    case SYSTEM_LOWPOWER_S12:
    {
      HAL_PWREx_EnterSHUTDOWNMode();
      break;
    }

    /* Shutdown wo/ RTC */
    case SYSTEM_LOWPOWER_S13:
    {
      HAL_PWREx_EnterSHUTDOWNMode();
      break;
    }

    default:
    {
      /* Run mode */
      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system regulator.
  * @param  scenario : Specifies system low-power mode scenario from
  *                    System_LowPowerModeScenarioTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_regulator_config(System_LowPowerModeScenarioTypeDef scenario)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  switch (scenario)
  {
    /* Check if system regulator is in low power mode Standby */
    case SYSTEM_LOWPOWER_S9:
    {
      HAL_PWREx_EnableUltraLowPowerMode();
      break;
    }

    /* Check if system regulator is in low power mode Standby */
    case SYSTEM_LOWPOWER_S10:
    {
      HAL_PWREx_EnableUltraLowPowerMode();
      break;
    }

    /* Check if system regulator is in low power mode Standby */
    case SYSTEM_LOWPOWER_S11:
    {
      HAL_PWREx_EnableUltraLowPowerMode();
      break;
    }

    /* Main regulator is on */
    default:
    {
      HAL_PWREx_DisableUltraLowPowerMode();
      break;
    }
  }

  return status;
}
/* USER CODE END 1 */