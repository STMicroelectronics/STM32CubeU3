/**
  ******************************************************************************
  * @file    system_stm32u3xx_s.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M33 Device Peripheral Access Layer System Source File
  *
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
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32u3xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *   After each device reset the MSI (12 MHz) is used as system clock source.
  *   Then SystemInit() function is called, in "startup_stm32u3xx.s" file, to
  *   configure the system clock before to branch to main program.
  *
  *   This file configures the system clock as follows:
  *=============================================================================
  *-----------------------------------------------------------------------------
  *        System Clock source                     | MSI
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                              | 12000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                                | 12000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                           | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB3 Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *=============================================================================
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup STM32U3xx_system
  * @{
  */

/** @addtogroup STM32U3xx_System_Private_Includes
  * @{
  */
#include "stm32u3xx.h"

/**
  * @}
  */

/** @addtogroup STM32U3xx_System_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @addtogroup STM32U3xx_System_Private_Defines
  * @{
  */
#if !defined  (HSE_VALUE)
  #define HSE_VALUE             32000000U /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE             16000000U /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#if !defined  (MSIRC0_VALUE)
  #define MSIRC0_VALUE          96000000U /*!< Value of the Internal MSI RC0 oscillator in Hz*/
#endif /* MSI_VALUE */

#if !defined  (MSIRC1_VALUE)
  #define MSIRC1_VALUE          24000000U /*!< Value of the Internal MSI RC1 oscillator in Hz*/
#endif /* MSI_VALUE */

/* Note: Following vector table addresses must be defined in line with linker
         configuration. */
/*!< Uncomment the following line if you need to relocate the vector table
     anywhere in Flash or Sram, else the vector table is kept at the automatic
     remap of boot address selected */
/* #define USER_VECT_TAB_ADDRESS */

#if defined(USER_VECT_TAB_ADDRESS)
/*!< Uncomment the following line if you need to relocate your vector Table
     in Sram else user remap will be done in Flash. */
/* #define VECT_TAB_SRAM */
#if defined(VECT_TAB_SRAM)
#define VECT_TAB_BASE_ADDRESS   SRAM1_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#else
#define VECT_TAB_BASE_ADDRESS   FLASH_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#endif /* VECT_TAB_SRAM */
#endif /* USER_VECT_TAB_ADDRESS */

/* Definitions for RCC PWR GTZC TZSC & TZIC ALL register values */
/* RCC ALL register values */
#define RCC_AHB1ENR1_ALL       (0x81031109UL)
#define RCC_AHB1ENR2_ALL       (0x00000004UL)
#define RCC_AHB2ENR1_ALL       (0x483F0CDFUL)
#define RCC_AHB2ENR2_ALL       (0x00000010UL)
#define RCC_APB1ENR1_ALL       (0x71FC4137UL)
#define RCC_APB1ENR2_ALL       (0x00000220UL)
#define RCC_APB2ENR_ALL        (0x09275800UL)
#define RCC_APB3ENR_ALL        (0x0000B8C2UL)
#define RCC_SECCFGR_ALL        (0x00001CFFUL)

/* GTZC ALL register values */
#define TZIC1_IER1_ALL         (0x01E6FDF7UL)
#define TZIC1_IER2_ALL         (0x0001FEFBUL)
#define TZIC1_IER3_ALL         (0x01D3F958UL)
#define TZIC1_IER4_ALL         (0x0F00C1FFUL)

#define TZSC1_SECCFGR1_ALL     (0x01E6FDF7UL)
#define TZSC1_SECCFGR2_ALL     (0x0001FEFBUL)
#define TZSC1_SECCFGR3_ALL     (0x01D3F958UL)

/* PWR ALL register values */
#define PWR_SECCFGR_ALL        (0x0000F3FFUL)

/* GPIO ALL register values */
#define GPIOA_SECCFGR_ALL      (0x0000FFFFUL)
#define GPIOB_SECCFGR_ALL      (0x0000FFFFUL)
#define GPIOC_SECCFGR_ALL      (0x0000FFFFUL)
#define GPIOD_SECCFGR_ALL      (0x0000FFFFUL)
#define GPIOE_SECCFGR_ALL      (0x0000FFFFUL)
#define GPIOG_SECCFGR_ALL      (0x0000FFFCUL)
#define GPIOH_SECCFGR_ALL      (0x0000000BUL)

/* SYSCFG ALL register values */
#define SYSCFG_SECCFGR_ALL     (0x0000000B)

/* DMA ALL register values */
#define GPDMA1_SECCFGR_ALL     (0x00000FFFUL)

/* EXTI ALL register values */
#define EXTI_SECCFGR1_ALL      (0x0007FFFFFUL)

/* RTC ALL register values */
#define RTC_SECCFGR_ALL        (0x00008000UL)

/* TAMP ALL register values */
#define TAMP_SECCFGR_ALL       (0x80FF80FFUL)

/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32U3xx_System_Private_Macros
  * @{
  */
void SecureSystemInit(void);
static void flash_init_cfg(void);
/**
  * @}
  */

/** @addtogroup STM32U3xx_System_Private_Variables
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
uint32_t SystemCoreClock = (MSIRC1_VALUE >> 1u);
const uint8_t AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint8_t APBPrescTable[8] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};
/**
  * @}
  */

/** @addtogroup STM32U3xx_System_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32U3xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 20U)|(3UL << 22U));  /* set CP10 and CP11 Full Access */
#endif

  /* Configure the Vector Table location -------------------------------------*/
#if defined(USER_VECT_TAB_ADDRESS)
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation */
#endif /* USER_VECT_TAB_ADDRESS */

  SecureSystemInit();                       /* Set resources as secure */
  flash_init_cfg();                         /* init the flash to full secure */
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is MSI, SystemCoreClock will be computed based on
  *             MSIRC0_VALUE(*) or MSIRC1_VALUE(*), depending on MSI source selected.
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(**)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(***)
  *
  *         (*) MSIRC0_VALUE and MSIRC1_VALUE are constant defined in stm32u3xx_hal_conf.h
  *             file (default values are respectively 96 MHz and 24 MHz)  but the real
  *             value may vary depending on the variations in voltage and temperature.
  *
  *         (**) HSI_VALUE is a constant defined in stm32u3xx_hal.h file (default value
  *              16 MHz) but the real value may vary depending on the variations
  *              in voltage and temperature.
  *
  *         (***) HSE_VALUE is a constant defined in stm32u3xx_hal.h file (default value
  *              16 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp, msirange;

  /* Get SYSCLK source -------------------------------------------------------*/
  switch (RCC->CFGR1 & RCC_CFGR1_SWS)
  {
  case 0x00:  /* MSIS used as system clock source */
    /* Read RCC ICSR1 register */
    tmp = RCC->ICSCR1;
    /* Check which MSIS Range is selected */
    if ((tmp & RCC_ICSCR1_MSIRGSEL) != 0x00u)
    {
      /* Check which MSIRCx is selected as MSIS source */
      if ((tmp & RCC_ICSCR1_MSISSEL) != 0x00u)
      {
        /* MSI RC1 is selected */
        SystemCoreClock = MSIRC1_VALUE;
      }
      else
      {
        /* MSI RC0 is selected */
        SystemCoreClock = MSIRC0_VALUE;
      }

      /* Get MSIS range */
      msirange = (tmp & RCC_ICSCR1_MSISDIV) >> RCC_ICSCR1_MSISDIV_Pos;
    }
    else
    {
      /* MSI RC1 is selected */
      SystemCoreClock = MSIRC1_VALUE;

      /* Get MSIS range */
      msirange = (RCC->CSR & (RCC_CSR_MSISDIVS_1 | RCC_CSR_MSISDIVS_0)) >> RCC_CSR_MSISDIVS_Pos;
    }

    /*MSIS frequency in HZ*/
    SystemCoreClock >>= msirange;
    break;

  case RCC_CFGR1_SWS_0:  /* HSI used as system clock source */
    SystemCoreClock = HSI_VALUE;
    break;

  case RCC_CFGR1_SWS_1:  /* HSE used as system clock source */
    SystemCoreClock = HSE_VALUE;
    break;

  default:
    SystemCoreClock = 0xFFFFFFFFU;
    break;
  }

  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR2 & RCC_CFGR2_HPRE) >> RCC_CFGR2_HPRE_Pos)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

/**
  * @brief  Configure all securable resources as secure
  * @param  None
  * @retval None
  */
void SecureSystemInit(void)
{

  /* Clock enabling */
  RCC->AHB1ENR1 |= RCC_AHB1ENR1_ALL;
  RCC->AHB1ENR2 |= RCC_AHB1ENR2_ALL;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_ALL;
  RCC->AHB2ENR2 |= RCC_AHB2ENR2_ALL;
  RCC->APB1ENR1 |= RCC_APB1ENR1_ALL;
  RCC->APB1ENR2 |= RCC_APB1ENR2_ALL;
  RCC->APB2ENR  |= RCC_APB2ENR_ALL;
  RCC->APB3ENR  |= RCC_APB3ENR_ALL;

  /* Illegal access interrupts configuration */
  GTZC_TZIC1->IER1 |= TZIC1_IER1_ALL;
  GTZC_TZIC1->IER2 |= TZIC1_IER2_ALL;
  GTZC_TZIC1->IER3 |= TZIC1_IER3_ALL;
  GTZC_TZIC1->IER4 |= TZIC1_IER4_ALL;

  /* Securable peripherals security and privilege management */
  GTZC_TZSC1->SECCFGR1  |= TZSC1_SECCFGR1_ALL;
  GTZC_TZSC1->SECCFGR2  |= TZSC1_SECCFGR2_ALL;
  GTZC_TZSC1->SECCFGR3  |= TZSC1_SECCFGR3_ALL;

  /* TrustZone-aware peripherals security and privilege management */

  /* PWR */
  PWR->SECCFGR  |= PWR_SECCFGR_ALL;

  /* RCC */
  RCC->SECCFGR  |= RCC_SECCFGR_ALL;

  /* GPIO */
  GPIOA->SECCFGR |= GPIOA_SECCFGR_ALL;
  GPIOB->SECCFGR |= GPIOB_SECCFGR_ALL;
  GPIOC->SECCFGR |= GPIOC_SECCFGR_ALL;
  GPIOD->SECCFGR |= GPIOD_SECCFGR_ALL;
  GPIOE->SECCFGR |= GPIOE_SECCFGR_ALL;
#ifdef GPIOG
  GPIOG->SECCFGR |= GPIOG_SECCFGR_ALL;
#endif
  GPIOH->SECCFGR |= GPIOH_SECCFGR_ALL;

  /* SYSCFG */
  SYSCFG->SECCFGR |= SYSCFG_SECCFGR_ALL;

  /* DMA */
  GPDMA1->SECCFGR  |= GPDMA1_SECCFGR_ALL;

  /* EXTI */
  EXTI->SECCFGR1  |= EXTI_SECCFGR1_ALL;

  /* RTC and TAMP */
  /* Registers accesses enabling */
  PWR->DBPR     |= PWR_DBPR_DBP;

  /* RTC */
  RTC->SECCFGR  |= RTC_SECCFGR_ALL;

  /* TAMP */
  TAMP->SECCFGR  |= TAMP_SECCFGR_ALL;

  /* Registers accesses disabling */
  PWR->DBPR     &= ~PWR_DBPR_DBP;

   /* Clock disabling */
   RCC->AHB1ENR1 &= ~(RCC_AHB1ENR1_ALL & ~(RCC_AHB1ENR1_SRAM1EN | RCC_AHB1ENR1_GTZC1EN | RCC_AHB1ENR1_FLASHEN));
   RCC->AHB1ENR2 &= ~(RCC_AHB1ENR2_ALL);
   RCC->AHB2ENR1 &= ~(RCC_AHB2ENR1_ALL & ~(RCC_AHB2ENR1_SRAM2EN));
   RCC->AHB2ENR2 &= ~(RCC_AHB2ENR2_ALL);
   RCC->APB1ENR1 &= ~(RCC_APB1ENR1_ALL);
   RCC->APB1ENR2 &= ~(RCC_APB1ENR2_ALL);
   RCC->APB2ENR  &= ~(RCC_APB2ENR_ALL);
   RCC->APB3ENR  &= ~(RCC_APB3ENR_ALL);
}

/*----------------------------------------------------------------------------
  * @brief  Initialize the flash to full secure.
  * @param  None
  * @retval None
 *----------------------------------------------------------------------------*/
static void flash_init_cfg(void)
{
  uint32_t i = 0U;
  __IO uint32_t *reg;
  const uint32_t flash_bb_all_sec = 0xFFFFFFFF;

  /* Configure Bank1 flash on Secure */
  reg = &(FLASH->SECBB1R1);

  for (i = 0U; i < FLASH_BLOCKBASED_NB_REG; i++)
  {
    *(reg + i) = flash_bb_all_sec;
  }

  /* Configure Bank2 flash on Secure */
  reg = &(FLASH->SECBB2R1);

  for (i = 0U; i < FLASH_BLOCKBASED_NB_REG; i++)
  {
    *(reg + i) = flash_bb_all_sec;
  }
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

