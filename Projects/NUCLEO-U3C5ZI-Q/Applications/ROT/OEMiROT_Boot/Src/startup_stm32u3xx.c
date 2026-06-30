/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 * Copyright (c) 2024 STMicroelectronics. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.6.0 startup_ARMv81MML.c
 * Git SHA: b5f0603d6a584d1724d952fd8b0737458b90d62b
 */

#include "stm32u3xx.h"
/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void(*pFunc)(void);

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern void __PROGRAM_START(void) __NO_RETURN;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern void Error_Handler(void);
#endif

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Reset_Handler(void) __NO_RETURN;
#define TAMP_SR_NO_EVENT         (0U)
/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
#define DEFAULT_IRQ_HANDLER(handler_name)  \
  void handler_name(void); \
  __WEAK void handler_name(void) { \
    while(1); \
  }

/* Exceptions */
DEFAULT_IRQ_HANDLER(NMI_Handler)
DEFAULT_IRQ_HANDLER(HardFault_Handler)
DEFAULT_IRQ_HANDLER(MemManage_Handler)
DEFAULT_IRQ_HANDLER(BusFault_Handler)
DEFAULT_IRQ_HANDLER(UsageFault_Handler)
DEFAULT_IRQ_HANDLER(SecureFault_Handler)
DEFAULT_IRQ_HANDLER(SVC_Handler)
DEFAULT_IRQ_HANDLER(DebugMon_Handler)
DEFAULT_IRQ_HANDLER(PendSV_Handler)
DEFAULT_IRQ_HANDLER(SysTick_Handler)
DEFAULT_IRQ_HANDLER(WWDG_IRQHandler)
DEFAULT_IRQ_HANDLER(PVD_PVM_IRQHandler)
DEFAULT_IRQ_HANDLER(RTC_IRQHandler)
DEFAULT_IRQ_HANDLER(RTC_S_IRQHandler)
DEFAULT_IRQ_HANDLER(TAMP_IRQHandler)
DEFAULT_IRQ_HANDLER(RAMCFG_IRQHandler)
DEFAULT_IRQ_HANDLER(FLASH_IRQHandler)
DEFAULT_IRQ_HANDLER(FLASH_S_IRQHandler)
DEFAULT_IRQ_HANDLER(GTZC_IRQHandler)
DEFAULT_IRQ_HANDLER(RCC_IRQHandler)
DEFAULT_IRQ_HANDLER(RCC_S_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI0_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI1_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI2_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI3_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI4_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI5_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI6_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI7_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI8_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI9_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI10_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI11_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI12_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI13_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI14_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI15_IRQHandler)
DEFAULT_IRQ_HANDLER(IWDG_IRQHandler)
DEFAULT_IRQ_HANDLER(SAES_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel0_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel1_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel2_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel3_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel4_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel5_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel6_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel7_IRQHandler)
DEFAULT_IRQ_HANDLER(ADC1_IRQHandler)
DEFAULT_IRQ_HANDLER(DAC1_IRQHandler)
DEFAULT_IRQ_HANDLER(FDCAN1_IT0_IRQHandler)
DEFAULT_IRQ_HANDLER(FDCAN1_IT1_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_BRK_TERR_IERR_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_UP_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_TRG_COM_DIR_IDX_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_CC_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM2_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM3_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM4_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM6_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM7_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM12_IRQHandler)
DEFAULT_IRQ_HANDLER(I3C1_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I3C1_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C1_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C1_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C2_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C2_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI1_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI2_IRQHandler)
DEFAULT_IRQ_HANDLER(USART1_IRQHandler)
DEFAULT_IRQ_HANDLER(USART2_IRQHandler)
DEFAULT_IRQ_HANDLER(USART3_IRQHandler)
DEFAULT_IRQ_HANDLER(UART4_IRQHandler)
DEFAULT_IRQ_HANDLER(UART5_IRQHandler)
DEFAULT_IRQ_HANDLER(LPUART1_IRQHandler)
DEFAULT_IRQ_HANDLER(LPTIM1_IRQHandler)
DEFAULT_IRQ_HANDLER(LPTIM2_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM15_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM16_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM17_IRQHandler)
DEFAULT_IRQ_HANDLER(COMP_IRQHandler)
DEFAULT_IRQ_HANDLER(USB_FS_IRQHandler)
DEFAULT_IRQ_HANDLER(CRS_IRQHandler)
DEFAULT_IRQ_HANDLER(OCTOSPI1_IRQHandler)
DEFAULT_IRQ_HANDLER(HSP1_IRQHandler)
DEFAULT_IRQ_HANDLER(SDMMC1_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel8_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel9_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel10_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel11_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C3_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C3_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(SAI1_IRQHandler)
DEFAULT_IRQ_HANDLER(TSC_IRQHandler)
DEFAULT_IRQ_HANDLER(AES_IRQHandler)
DEFAULT_IRQ_HANDLER(RNG_IRQHandler)
DEFAULT_IRQ_HANDLER(FPU_IRQHandler)
DEFAULT_IRQ_HANDLER(HASH_IRQHandler)
DEFAULT_IRQ_HANDLER(PKA_IRQHandler)
DEFAULT_IRQ_HANDLER(LPTIM3_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI3_IRQHandler)
DEFAULT_IRQ_HANDLER(I3C2_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I3C2_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_BRK_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_UP_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_TRG_COM_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_CC_IRQHandler)
DEFAULT_IRQ_HANDLER(ICACHE_IRQHandler)
DEFAULT_IRQ_HANDLER(LPTIM4_IRQHandler)
DEFAULT_IRQ_HANDLER(ADF1_IRQHandler)
DEFAULT_IRQ_HANDLER(ADC2_IRQHandler)
DEFAULT_IRQ_HANDLER(FDCAN2_IT0_IRQHandler)
DEFAULT_IRQ_HANDLER(FDCAN2_IT1_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C4_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C4_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI4_IRQHandler)
DEFAULT_IRQ_HANDLER(PWR_IRQHandler)
DEFAULT_IRQ_HANDLER(PWR_S_IRQHandler)
/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const pFunc __VECTOR_TABLE[];
const pFunc __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE =
{
  (pFunc)(&__INITIAL_SP),           /*      Initial Stack Pointer */
  Reset_Handler,                    /*      Reset Handler */
  NMI_Handler,                      /* -14: NMI Handler */
  Error_Handler,                    /*      Hard Fault Handler*/
  Error_Handler,                    /*      MPU Fault Handler*/
  Error_Handler,                    /* -11: Bus Fault Handler */
  Error_Handler,                    /* -10: Usage Fault Handler */
  Error_Handler,                    /*  -9: Secure Fault Handler */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  SVC_Handler,                      /*  -5: SVCall Handler */
  DebugMon_Handler,                 /*  -4: Debug Monitor Handler */
  0,                                /*      Reserved */
  PendSV_Handler,                   /*  -2: PendSV Handler */
  SysTick_Handler,                  /*  -1: SysTick Handler */
  WWDG_IRQHandler,                  /*   0: Window WatchDog */
  PVD_PVM_IRQHandler,               /*   1: PVD/PVM through EXTI Line 16 detection Interrupt */
  RTC_IRQHandler,                   /*   2: RTC non-secure interrupt */
  RTC_S_IRQHandler,                 /*   3: RTC secure interrupt */
  TAMP_IRQHandler,                  /*   4: Tamper interrupt */
  RAMCFG_IRQHandler,                /*   5: RAMCFG global interrupt */
  FLASH_IRQHandler,                 /*   6: FLASH non-secure global interrupt */
  FLASH_S_IRQHandler,               /*   7: FLASH secure global interrupt */
  GTZC_IRQHandler,                  /*   8: Global TrustZone Controller interrupt */
  RCC_IRQHandler,                   /*   9: RCC non-secure global interrupts */
  RCC_S_IRQHandler,                 /*  10: RCC secure global interrupts */
  EXTI0_IRQHandler,                 /*  11: EXTI Line 0 interrupt */
  EXTI1_IRQHandler,                 /*  12: EXTI Line 1 interrupt */
  EXTI2_IRQHandler,                 /*  13: EXTI Line 2 interrupt */
  EXTI3_IRQHandler,                 /*  14: EXTI Line 3 interrupt */
  EXTI4_IRQHandler,                 /*  15: EXTI Line 4 interrupt */
  EXTI5_IRQHandler,                 /*  16: EXTI Line 5 interrupt */
  EXTI6_IRQHandler,                 /*  17: EXTI Line 6 interrupt */
  EXTI7_IRQHandler,                 /*  18: EXTI Line 7 interrupt */
  EXTI8_IRQHandler,                 /*  19: EXTI Line 8 interrupt */
  EXTI9_IRQHandler,                 /*  20: EXTI Line 9 interrupt */
  EXTI10_IRQHandler,                /*  21: EXTI Line 10 interrupt */
  EXTI11_IRQHandler,                /*  22: EXTI Line 11 interrupt */
  EXTI12_IRQHandler,                /*  23: EXTI Line 12 interrupt */
  EXTI13_IRQHandler,                /*  24: EXTI Line 13 interrupt */
  EXTI14_IRQHandler,                /*  25: EXTI Line 14 interrupt */
  EXTI15_IRQHandler,                /*  26: EXTI Line 15 interrupt */
  IWDG_IRQHandler,                  /*  27: IWDG global interrupt */
  SAES_IRQHandler,                  /*  28:Secure AES global interrupt */
  GPDMA1_Channel0_IRQHandler,       /*  29: GPDMA1 Channel 0 global interrupt */
  GPDMA1_Channel1_IRQHandler,       /*  30: GPDMA1 Channel 1 global interrupt */
  GPDMA1_Channel2_IRQHandler,       /*  31: GPDMA1 Channel 2 global interrupt */
  GPDMA1_Channel3_IRQHandler,       /*  32: GPDMA1 Channel 3 global interrupt */
  GPDMA1_Channel4_IRQHandler,       /*  33: GPDMA1 Channel 4 global interrupt */
  GPDMA1_Channel5_IRQHandler,       /*  34: GPDMA1 Channel 5 global interrupt */
  GPDMA1_Channel6_IRQHandler,       /*  35: GPDMA1 Channel 6 global interrupt */
  GPDMA1_Channel7_IRQHandler,       /*  36: GPDMA1 Channel 7 global interrupt */
  ADC1_IRQHandler,                  /*  37: ADC1 global interrupt  */
  DAC1_IRQHandler,                  /*  38: DAC1 global interrupt */
  FDCAN1_IT0_IRQHandler,            /*  39: FDCAN1 interrupt 0 */
  FDCAN1_IT1_IRQHandler,            /*  40: FDCAN1 interrupt 1 */
  TIM1_BRK_TERR_IERR_IRQHandler,    /*  41: TIM1 Break, Transition error and Index error interrupt */
  TIM1_UP_IRQHandler,               /*  42: TIM1 Update interrupt */
  TIM1_TRG_COM_DIR_IDX_IRQHandler,  /*  43: TIM1 Trigger, Commutation, Direction change and Index interrupt */
  TIM1_CC_IRQHandler,               /*  44: TIM1 Capture Compare interrupt */
  TIM2_IRQHandler,                  /*  45: TIM2 global interrupt */
  TIM3_IRQHandler,                  /*  46: TIM3 global interrupt */
  TIM4_IRQHandler,                  /*  47: TIM4 global interrupt */
  0,                                /*  48: Reserved */
  TIM6_IRQHandler,                  /*  49: TIM6 global interrupt */
  TIM7_IRQHandler,                  /*  50: TIM7 global interrupt */
  TIM12_IRQHandler,                 /*  51: TIM12 global interrupt */
  0,                                /*  52: Reserved */
  I3C1_EV_IRQHandler,               /*  53: I3C1 event interrupt */
  I3C1_ER_IRQHandler,               /*  54: I3C1 error interrupt */
  I2C1_EV_IRQHandler,               /*  55: I2C1 Event interrupt */
  I2C1_ER_IRQHandler,               /*  56: I2C1 Error interrupt */
  I2C2_EV_IRQHandler,               /*  57: I2C2 Event interrupt */
  I2C2_ER_IRQHandler,               /*  58: I2C2 Error interrupt */
  SPI1_IRQHandler,                  /*  59: SPI1 global interrupt */
  SPI2_IRQHandler,                  /*  60: SPI2 global interrupt */
  USART1_IRQHandler,                /*  61: USART1 global interrupt */
  USART2_IRQHandler,                /*  62: USART2 global interrupt */
  USART3_IRQHandler,                /*  63: USART3 global interrupt */
  UART4_IRQHandler,                 /*  64: UART4 global interrupt */
  UART5_IRQHandler,                 /*  65: UART5 global interrupt */
  LPUART1_IRQHandler,               /*  66: LPUART1 global interrupt */
  LPTIM1_IRQHandler,                /*  67: LPTIM1 global interrupt */
  LPTIM2_IRQHandler,                /*  68: LPTIM2 global interrupt */
  TIM15_IRQHandler,                 /*  69: TIM15 global interrupt */
  TIM16_IRQHandler,                 /*  70: TIM16 global interrupt */
  TIM17_IRQHandler,                 /*  71: TIM17 global interrupt */
  COMP_IRQHandler,                  /*  72: COMP1 and COMP2 through EXTI Lines interrupt */
  USB_FS_IRQHandler,                /*  73, USB FS global interrupt */
  CRS_IRQHandler,                   /*  74: CRS global interrupt */
  0,                                /*  75: Reserved */
  OCTOSPI1_IRQHandler,              /*  76: OctoSPI1 global interrupt */
  HSP1_IRQHandler,                  /*  77: HSP1 global interrupt */
  SDMMC1_IRQHandler,                /*  78: SDMMC1 global interrupt */
  0,                                /*  79: Reserved */
  GPDMA1_Channel8_IRQHandler,       /*  80: GPDMA1 Channel 8 global interrupt */
  GPDMA1_Channel9_IRQHandler,       /*  81: GPDMA1 Channel 9 global interrupt */
  GPDMA1_Channel10_IRQHandler,      /*  82: GPDMA1 Channel 10 global interrupt */
  GPDMA1_Channel11_IRQHandler,      /*  83: GPDMA1 Channel 11 global interrupt */
  0,                                /*  84: Reserved */
  0,                                /*  85: Reserved */
  0,                                /*  86: Reserved */
  0,                                /*  87: Reserved */
  I2C3_EV_IRQHandler,               /*  88: I2C3 event interrupt */
  I2C3_ER_IRQHandler,               /*  89: I2C3 error interrupt */
  SAI1_IRQHandler,                  /*  90: Serial Audio Interface 1 global interrupt */
  0,                                /*  91: Reserved */
  TSC_IRQHandler,                   /*  92: Touch Sense Controller global interrupt */
  AES_IRQHandler,                   /*  93: AES global interrupt */
  RNG_IRQHandler,                   /*  94: RNG global interrupt */
  FPU_IRQHandler,                   /*  95: FPU global interrupt */
  HASH_IRQHandler,                  /*  96: HASH global interrupt */
  PKA_IRQHandler,                   /*  97: PKA global interrupt */
  LPTIM3_IRQHandler,                /*  98: LPTIM3 global interrupt */
  SPI3_IRQHandler,                  /*  99: SPI3 global interrupt */
  I3C2_EV_IRQHandler,               /* 100: I3C2 Event interrupt */
  I3C2_ER_IRQHandler,               /* 101: I3C2 Error interrupt */
  TIM8_BRK_IRQHandler,              /* 102: TIM8 Break interrupt */
  TIM8_UP_IRQHandler,               /* 103: TIM8 Update interrupt */
  TIM8_TRG_COM_IRQHandler,          /* 104: TIM8 Trigger and Commutation interrupt */
  TIM8_CC_IRQHandler,               /* 105: TIM8 Capture Compare interrupt */
  0,                                /* 106: Reserved */
  ICACHE_IRQHandler,                /* 107: Instruction cache global interrupt*/
  0,                                /* 108: Reserved */
  0,                                /* 109: Reserved */
  LPTIM4_IRQHandler,                /* 110: LPTIM4 global interrupt */
  0,                                /* 111: Reserved */
  ADF1_IRQHandler,                  /* 112: ADF interrupt */
  ADC2_IRQHandler,                  /* 113: ADC2 global interrupt */
  FDCAN2_IT0_IRQHandler,            /* 114: FDCAN2 Interrupt 0 */
  FDCAN2_IT1_IRQHandler,            /* 115: FDCAN2 Interrupt 1 */
  I2C4_EV_IRQHandler,               /* 116: I2C4 event interrupt */
  I2C4_ER_IRQHandler,               /* 117: I2C4 error interrupt */
  0,                                /* 118: Reserved */
  SPI4_IRQHandler,                  /* 119: SPI4 global interrupt */  
  0,                                /* 120: Reserved */
  0,                                /* 121: Reserved */
  0,                                /* 122: Reserved */
  PWR_IRQHandler,                   /* 123: PWR non-secure interrupt */
  PWR_S_IRQHandler,                 /* 124: PWR secure interrupt */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

#ifdef OEMIROT_DEV_MODE
#ifdef __ICCARM__
__no_init volatile uint32_t TamperEventCleared;
#else
volatile uint32_t TamperEventCleared  __attribute__((section(".bss.NoInit")));
#endif /* __ICCARM__ */
#endif /* OEMIROT_DEV_MODE */
extern RTC_HandleTypeDef RTCHandle;
/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
  /* Configure DWT to enable cycles counter */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk | CoreDebug_DEMCR_MON_EN_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /** Register access to avoid stack usage */
  /* Enable RAMCFG Clock */
  SET_BIT(RCC->AHB1ENR1, RCC_AHB1ENR1_RAMCFGEN);
  (void)READ_BIT(RCC->AHB1ENR1, RCC_AHB1ENR1_RAMCFGEN); /* Dummy read to ensure clocking */

  /* Wait for SRAM2 availability */
  while (READ_BIT(RAMCFG_SRAM2->ISR, (RAMCFG_ISR_SRAMBUSY)) != 0U)
  {
    __NOP();
  }
#ifdef OEMIROT_DEV_MODE
  /* Reset the tamper event status */
  TamperEventCleared = 0;
#endif
  /* enable write access to backup domain */
  SET_BIT(RCC->AHB1ENR2, RCC_AHB1ENR2_PWREN);
  (void)READ_BIT(RCC->AHB1ENR2, RCC_AHB1ENR2_PWREN); /* Dummy read to wait for write effectiveness */

  SET_BIT(PWR->DBPR, PWR_DBPR_DBP);
  (void)READ_REG(PWR->DBPR); /* Dummy read to wait for write effectiveness */

  /* Enable clock for Tamper register access */
  SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_RTCAPBEN);
  (void)READ_REG(RCC->APB1ENR1); /* Dummy read to wait for write effectiveness */

  /* Release reset of back-up domain in case it is set, to avoid
  blocking the device (system reset does not release it) */
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_BDRST);
  (void)READ_REG(RCC->BDCR); /* Dummy read to wait for write effectiveness */

  /* Check if a tamper has occurred */
  if (TAMP->SR != TAMP_SR_NO_EVENT)
  {
    /* reset Tamper status */
    TAMP->SCR = 0xFFFFFFFFU;

    /* Clear the pending IRQ which has already been handled */
    NVIC_ClearPendingIRQ(TAMP_IRQn);
#ifdef OEMIROT_DEV_MODE
    /* Deactivation of tampers to reach a predictable log to help the developer */
    HAL_RTCEx_DeactivateTamper(&RTCHandle, RTC_TAMPER_ALL);

    /* memorize for log that event has been cleared */
    TamperEventCleared = 1U;
#else
    /* System Reset request */
    NVIC_SystemReset();
#endif /* OEMIROT_DEV_MODE */
  }
  else
  {
    /**  Enable TAMP IRQ , to catch tamper interrupt in TAMP_IRQHandler */
    /*  else a stack in SRAM2 is cleaned a HardFault can occur, at every pop of
    *  function */
#ifndef OEMIROT_DEV_MODE
    __enable_irq();
    NVIC_EnableIRQ(TAMP_IRQn);
#endif /* !OEMIROT_DEV_MODE */
  }

  /** After this point, the stack may be used */

  /* disable write access to backup domain */
  __DMB(); /* To wait for TAMP reg write complete before cutting access */
  CLEAR_BIT(PWR->DBPR, PWR_DBPR_DBP);

  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
