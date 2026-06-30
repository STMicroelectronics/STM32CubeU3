/**
  ******************************************************************************
  * @file    boot_hal_flowcontrol.h
  * @author  MCD Application Team
  * @brief   Header for flow control in boot_hal.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOOT_HAL_FLOWCONTROL_H
#define BOOT_HAL_FLOWCONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "region_defs.h"
#include "boot_hal_cfg.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/**
  * @brief  Flow Control : Initial value
  */
#define FLOW_STAGE_CFG              0xfffffe00U
#define FLOW_STAGE_CHK              0xffffe01eU

/**
  * @brief  Flow Control : Initial value
  */
#define FLOW_CTRL_INIT_VALUE        0x00005776U        /*!< Init value definition */

/**
  * @brief  Flow Control : Steps definition
  */
#ifdef OEMIROT_BOOT_MPU_PROTECTION
#define FLOW_STEP_MPU_S_I_EN_R0     0x00006787U        /*!< Step MPU Secure Region 0 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R1     0x00007999U        /*!< Step MPU Secure Region 1 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R2     0x0000aab5U        /*!< Step MPU Secure Region 2 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R3     0x0000b4abU        /*!< Step MPU Secure Region 3 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R4     0x0000cccdU        /*!< Step MPU Secure Region 4 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R5     0x0000d2d3U        /*!< Step MPU Secure Region 5 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R6     0x00043d7aU        /*!< Step MPU Secure Region 6 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R7     0x0001cf0eU        /*!< Step MPU Secure Region 7 Init enable value */
#define FLOW_STEP_MPU_S_I_EN_R8     0x00173940U        /*!< Step MPU Secure Region 8 Init enable value */
#if !defined(MCUBOOT_OVERWRITE_ONLY)
#define FLOW_STEP_MPU_S_I_EN_R9     0x00009bbaU        /*!< Step MPU Secure Region 9 Init enable value */
#else
#define FLOW_STEP_MPU_S_I_EN_R9     0x00000000U        /*!< Step MPU Secure Region 9 Init enable value */
#endif
#define FLOW_STEP_MPU_S_I_EN        0x0000e3dcU        /*!< Step MPU Secure Init enable value */
#else /* OEMIROT_BOOT_MPU_PROTECTION */
#define FLOW_STEP_MPU_S_I_EN_R0     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R1     0x00000000U        /*!< No effect on control flow */

#define FLOW_STEP_MPU_S_I_EN_R2     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R3     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R4     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R5     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R6     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R7     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN_R8     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_EN        0x00000000U        /*!< No effect on control flow */
#endif /* OEMIROT_BOOT_MPU_PROTECTION */

#define FLOW_STEP_SAU_I_EN_R1       0x00017ba4U        /*!< Step SAU Region 1 Init enable value */
#define FLOW_STEP_SAU_I_EN_R2       0x00018799U        /*!< Step SAU Region 2 Init enable value */
#define FLOW_STEP_SAU_I_EN_R3       0x00019987U        /*!< Step SAU Region 3 Init enable value */
#define FLOW_STEP_SAU_I_EN          0x0001b696U        /*!< Step SAU enable Init value */

#if  (OEMIROT_TAMPER_ENABLE == ALL_TAMPER)
#define FLOW_STEP_TAMP_ACT_EN       0x000673eaU        /*!< Step Tamper active enable value */
#else
#define FLOW_STEP_TAMP_ACT_EN       0x00000000U        /*!< No effect on control flow */
#endif /* (OEMIROT_TAMPER_ENABLE == ALL_TAMPER) */
#if (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
#define FLOW_STEP_TAMP_INT_EN       0x00067f4dU        /*!< Step Tamper internal enable value */
#define FLOW_STEP_TAMP_SEC_EN       0x0006ad9fU        /*!< Step Tamper secure enable value */
#define FLOW_STEP_TAMP_PRIV_EN      0x0006b381U        /*!< Step Tamper privilege enable value */
#define FLOW_STEP_TAMP_CFG_EN       0x0006bf26U        /*!< Step Tamper configurable enable value */
#else
#define FLOW_STEP_TAMP_INT_EN       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_SEC_EN       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_PRIV_EN      0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_CFG_EN       0x00000000U        /*!< No effect on control flow */
#endif /* OEMIROT_TAMPER_ENABLE != NO_TAMPER) */

#ifdef OEMIROT_FLASH_PRIVONLY_ENABLE
#define FLOW_STEP_FLASH_P_EN        0x0001cef0U        /*!< Step FLASH Priv enable value */
#else
#define FLOW_STEP_FLASH_P_EN        0x00000000U        /*!< No effect on control flow */
#endif /* OEMIROT_FLASH_PRIVONLY_ENABLE */


#ifdef OEMIROT_BOOT_MPU_PROTECTION
#define FLOW_STEP_MPU_S_I_CH_R0     0x0001d0eeU        /*!< Step MPU Secure Region 0 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R1     0x0001e1e1U        /*!< Step MPU Secure Region 1 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R2     0x0001ffffU        /*!< Step MPU Secure Region 2 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R3     0x00023bd7U        /*!< Step MPU Secure Region 3 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R4     0x00025dafU        /*!< Step MPU Secure Region 4 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R5     0x00029cf6U        /*!< Step MPU Secure Region 5 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R6     0x0003da37U        /*!< Step MPU Secure Region 6 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R7     0x0001fe01U        /*!< Step MPU Secure Region 7 Init check value */
#define FLOW_STEP_MPU_S_I_CH_R8     0x0017412eU        /*!< Step MPU Secure Region 8 Init check value */
#if !defined(MCUBOOT_OVERWRITE_ONLY)
#define FLOW_STEP_MPU_S_I_CH_R9     0x00007867U        /*!< Step MPU Secure Region 9 Init enable value */
#else
#define FLOW_STEP_MPU_S_I_CH_R9     0x00000000U        /*!< Step MPU Secure Region 9 Init enable value */
#endif
#define FLOW_STEP_MPU_S_I_CH        0x0002e8fbU        /*!< Step MPU Secure Init check value */
#else /* OEMIROT_BOOT_MPU_PROTECTION */
#define FLOW_STEP_MPU_S_I_CH_R0     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R1     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R2     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R3     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R4     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R5     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R6     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R7     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH_R8     0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_S_I_CH        0x00000000U        /*!< No effect on control flow */
#endif /* OEMIROT_BOOT_MPU_PROTECTION */

#define FLOW_STEP_SAU_I_CH_R1       0x0003ae3aU        /*!< Step SAU Region 1 Init check value */
#define FLOW_STEP_SAU_I_CH_R2       0x0003b1daU        /*!< Step SAU Region 2 Init check value */
#define FLOW_STEP_SAU_I_CH_R3       0x0003bdb1U        /*!< Step SAU Region 3 Init check value */
#define FLOW_STEP_SAU_I_CH          0x0003c9bcU        /*!< Step SAU Init check value */

#if  (OEMIROT_TAMPER_ENABLE == ALL_TAMPER)
#define FLOW_STEP_TAMP_ACT_CH       0x0006ca19U        /*!< Step Tamper active check value */
#else
#define FLOW_STEP_TAMP_ACT_CH       0x00000000U        /*!< No effect on control flow */
#endif /*(OEMIROT_TAMPER_ENABLE == ALL_TAMPER) */
#if  (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
#define FLOW_STEP_TAMP_INT_CH       0x0006d407U        /*!< Step Tamper internal check value */
#define FLOW_STEP_TAMP_SEC_CH       0x0006d95eU        /*!< Step Tamper secure check value */
#define FLOW_STEP_TAMP_PRIV_CH      0x00072a07U        /*!< Step Tamper privilege check value */
#define FLOW_STEP_TAMP_CFG_CH       0x00073419U        /*!< Step Tamper configurable check value */
#else
#define FLOW_STEP_TAMP_INT_CH       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_SEC_CH       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_PRIV_CH      0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_TAMP_CFG_CH       0x00000000U        /*!< No effect on control flow */
#endif /* OEMIROT_TAMPER_ENABLE != NO_TAMPER) */

#ifdef OEMIROT_FLASH_PRIVONLY_ENABLE
#define FLOW_STEP_FLASH_P_CH        0x0003d7a2U        /*!< Step FLASH Priv check value */
#else
#define FLOW_STEP_FLASH_P_CH        0x00000000U        /*!< No effect on control flow */
#endif /* OEMIROT_FLASH_PRIVONLY_ENABLE */

#ifdef OEMIROT_BOOT_MPU_PROTECTION
#define FLOW_STEP_MPU_A_EN_R1       0x0003dbc9U        /*!< Step MPU Region 0 Appli enable value */
#define FLOW_STEP_MPU_A_CH_R1       0x000474dfU        /*!< Step MPU Region 0 Appli check value */
#define FLOW_STEP_MPU_A_EN_R2       0x000b2437U        /*!< Step MPU Region 2 Appli enable value */
#define FLOW_STEP_MPU_A_CH_R2       0x000b448eU        /*!< Step MPU Region 2 Appli check value */
#if  MCUBOOT_S_DATA_IMAGE_NUMBER == 1
#define FLOW_STEP_MPU_A_EN_R3       0x000b285cU        /*!< Step MPU Region 3 Appli enable value */
#define FLOW_STEP_MPU_A_CH_R3       0x000c0cb9U        /*!< Step MPU Region 3 Appli check value */
#else
#define FLOW_STEP_MPU_A_EN_R3       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_A_CH_R3       0x00000000U        /*!< No effect on control flow */
#endif
#else
#define FLOW_STEP_MPU_A_EN_R1       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_A_CH_R1       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_A_EN_R2       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_A_CH_R2       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_A_EN_R3       0x00000000U        /*!< No effect on control flow */
#define FLOW_STEP_MPU_A_CH_R3       0x00000000U        /*!< No effect on control flow */
#endif /* OEMIROT_BOOT_MPU_PROTECTION */

/**
  * @brief  SFU_BOOT Flow Control : Control values runtime protections
  */
/* Flow control Stage 1 */
#define FLOW_CTRL_MPU_S_I_EN_R0     (FLOW_CTRL_INIT_VALUE ^      FLOW_STEP_MPU_S_I_EN_R0)
#define FLOW_CTRL_MPU_S_I_EN_R1     (FLOW_CTRL_MPU_S_I_EN_R0 ^   FLOW_STEP_MPU_S_I_EN_R1)
#define FLOW_CTRL_MPU_S_I_EN_R2     (FLOW_CTRL_MPU_S_I_EN_R1 ^   FLOW_STEP_MPU_S_I_EN_R2)
#define FLOW_CTRL_MPU_S_I_EN_R3     (FLOW_CTRL_MPU_S_I_EN_R2 ^   FLOW_STEP_MPU_S_I_EN_R3)
#define FLOW_CTRL_MPU_S_I_EN_R4     (FLOW_CTRL_MPU_S_I_EN_R3 ^   FLOW_STEP_MPU_S_I_EN_R4)
#define FLOW_CTRL_MPU_S_I_EN_R5     (FLOW_CTRL_MPU_S_I_EN_R4 ^   FLOW_STEP_MPU_S_I_EN_R5)
#define FLOW_CTRL_MPU_S_I_EN_R6     (FLOW_CTRL_MPU_S_I_EN_R5 ^   FLOW_STEP_MPU_S_I_EN_R6)
#define FLOW_CTRL_MPU_S_I_EN_R7     (FLOW_CTRL_MPU_S_I_EN_R6 ^   FLOW_STEP_MPU_S_I_EN_R7)
#define FLOW_CTRL_MPU_S_I_EN_R8     (FLOW_CTRL_MPU_S_I_EN_R7 ^   FLOW_STEP_MPU_S_I_EN_R8)
#define FLOW_CTRL_MPU_S_I_EN_R9     (FLOW_CTRL_MPU_S_I_EN_R8 ^   FLOW_STEP_MPU_S_I_EN_R9)
#define FLOW_CTRL_MPU_S_I_EN        (FLOW_CTRL_MPU_S_I_EN_R9 ^   FLOW_STEP_MPU_S_I_EN)

#define FLOW_CTRL_SAU_I_EN_R1       (FLOW_CTRL_MPU_S_I_EN  ^     FLOW_STEP_SAU_I_EN_R1)
#define FLOW_CTRL_SAU_I_EN_R2       (FLOW_CTRL_SAU_I_EN_R1 ^     FLOW_STEP_SAU_I_EN_R2)
#define FLOW_CTRL_SAU_I_EN_R3       (FLOW_CTRL_SAU_I_EN_R2 ^     FLOW_STEP_SAU_I_EN_R3)
#define FLOW_CTRL_SAU_I_EN          (FLOW_CTRL_SAU_I_EN_R3 ^     FLOW_STEP_SAU_I_EN)

#define FLOW_CTRL_TAMP_ACT_EN       (FLOW_CTRL_SAU_I_EN ^        FLOW_STEP_TAMP_ACT_EN)
#define FLOW_CTRL_TAMP_INT_EN       (FLOW_CTRL_TAMP_ACT_EN ^     FLOW_STEP_TAMP_INT_EN)
#define FLOW_CTRL_TAMP_SEC_EN       (FLOW_CTRL_TAMP_INT_EN ^     FLOW_STEP_TAMP_SEC_EN)
#define FLOW_CTRL_TAMP_PRIV_EN      (FLOW_CTRL_TAMP_SEC_EN ^     FLOW_STEP_TAMP_PRIV_EN)
#define FLOW_CTRL_TAMP_CFG_EN       (FLOW_CTRL_TAMP_PRIV_EN ^    FLOW_STEP_TAMP_CFG_EN)

#define FLOW_CTRL_FLASH_P_EN        (FLOW_CTRL_TAMP_CFG_EN ^     FLOW_STEP_FLASH_P_EN)

#define FLOW_CTRL_STAGE_1           FLOW_CTRL_FLASH_P_EN

/* Flow control Stage 2 */
#define FLOW_CTRL_MPU_S_I_CH_R0     (FLOW_CTRL_STAGE_1 ^         FLOW_STEP_MPU_S_I_CH_R0)
#define FLOW_CTRL_MPU_S_I_CH_R1     (FLOW_CTRL_MPU_S_I_CH_R0 ^   FLOW_STEP_MPU_S_I_CH_R1)
#define FLOW_CTRL_MPU_S_I_CH_R2     (FLOW_CTRL_MPU_S_I_CH_R1 ^   FLOW_STEP_MPU_S_I_CH_R2)
#define FLOW_CTRL_MPU_S_I_CH_R3     (FLOW_CTRL_MPU_S_I_CH_R2 ^   FLOW_STEP_MPU_S_I_CH_R3)
#define FLOW_CTRL_MPU_S_I_CH_R4     (FLOW_CTRL_MPU_S_I_CH_R3 ^   FLOW_STEP_MPU_S_I_CH_R4)
#define FLOW_CTRL_MPU_S_I_CH_R5     (FLOW_CTRL_MPU_S_I_CH_R4 ^   FLOW_STEP_MPU_S_I_CH_R5)
#define FLOW_CTRL_MPU_S_I_CH_R6     (FLOW_CTRL_MPU_S_I_CH_R5 ^   FLOW_STEP_MPU_S_I_CH_R6)
#define FLOW_CTRL_MPU_S_I_CH_R7     (FLOW_CTRL_MPU_S_I_CH_R6 ^   FLOW_STEP_MPU_S_I_CH_R7)
#define FLOW_CTRL_MPU_S_I_CH_R8     (FLOW_CTRL_MPU_S_I_CH_R7 ^   FLOW_STEP_MPU_S_I_CH_R8)
#define FLOW_CTRL_MPU_S_I_CH_R9     (FLOW_CTRL_MPU_S_I_CH_R8 ^   FLOW_STEP_MPU_S_I_CH_R9)
#define FLOW_CTRL_MPU_S_I_CH        (FLOW_CTRL_MPU_S_I_CH_R9 ^   FLOW_STEP_MPU_S_I_CH)

#define FLOW_CTRL_SAU_I_CH_R1       (FLOW_CTRL_MPU_S_I_CH ^      FLOW_STEP_SAU_I_CH_R1)
#define FLOW_CTRL_SAU_I_CH_R2       (FLOW_CTRL_SAU_I_CH_R1 ^     FLOW_STEP_SAU_I_CH_R2)
#define FLOW_CTRL_SAU_I_CH_R3       (FLOW_CTRL_SAU_I_CH_R2 ^     FLOW_STEP_SAU_I_CH_R3)
#define FLOW_CTRL_SAU_I_CH          (FLOW_CTRL_SAU_I_CH_R3 ^     FLOW_STEP_SAU_I_CH)

#define FLOW_CTRL_TAMP_ACT_CH       (FLOW_CTRL_SAU_I_CH ^        FLOW_STEP_TAMP_ACT_CH)
#define FLOW_CTRL_TAMP_INT_CH       (FLOW_CTRL_TAMP_ACT_CH ^     FLOW_STEP_TAMP_INT_CH)
#define FLOW_CTRL_TAMP_SEC_CH       (FLOW_CTRL_TAMP_INT_CH ^     FLOW_STEP_TAMP_SEC_CH)
#define FLOW_CTRL_TAMP_PRIV_CH      (FLOW_CTRL_TAMP_SEC_CH ^     FLOW_STEP_TAMP_PRIV_CH)
#define FLOW_CTRL_TAMP_CFG_CH       (FLOW_CTRL_TAMP_PRIV_CH ^    FLOW_STEP_TAMP_CFG_CH)

#define FLOW_CTRL_FLASH_P_CH        (FLOW_CTRL_TAMP_CFG_CH ^     FLOW_STEP_FLASH_P_CH)

#define FLOW_CTRL_STAGE_2           FLOW_CTRL_FLASH_P_CH

/* Flow control Stage 3 Appli */
#define FLOW_CTRL_MPU_A_EN_R1       (FLOW_CTRL_STAGE_2 ^         FLOW_STEP_MPU_A_EN_R1)
#define FLOW_CTRL_MPU_A_EN_R2       (FLOW_CTRL_MPU_A_EN_R1 ^     FLOW_STEP_MPU_A_EN_R2)
#define FLOW_CTRL_MPU_A_EN_R3       (FLOW_CTRL_MPU_A_EN_R2 ^     FLOW_STEP_MPU_A_EN_R3)

#define FLOW_CTRL_STAGE_3_A         FLOW_CTRL_MPU_A_EN_R3

/* Flow control Stage 4 Appli */
#define FLOW_CTRL_MPU_A_CH_R1       (FLOW_CTRL_STAGE_3_A ^       FLOW_STEP_MPU_A_CH_R1)
#define FLOW_CTRL_MPU_A_CH_R2       (FLOW_CTRL_MPU_A_CH_R1 ^     FLOW_STEP_MPU_A_CH_R2)
#define FLOW_CTRL_MPU_A_CH_R3       (FLOW_CTRL_MPU_A_CH_R2 ^     FLOW_STEP_MPU_A_CH_R3)

#define FLOW_CTRL_STAGE_4_A         FLOW_CTRL_MPU_A_CH_R3

/* External variables --------------------------------------------------------*/
/**
  *  Flow control protection values
  *  Flow control stage
  */
extern volatile uint32_t uFlowProtectValue;
extern volatile uint32_t uFlowStage;

/* Exported macros -----------------------------------------------------------*/
/** Control with STEP operation :
  * (uFlowValue XOR STEP_VALUE) should be equal to CTRL_VALUE ==> execution stopped if failed !
  */
#if defined(FLOW_CONTROL)
#define FLOW_CONTROL_STEP(C,B,A) \
  do{ \
    (C) ^= (B);\
    if ((C) != (A))\
    { \
      Error_Handler();\
    } \
  }while(0)
#else
#define FLOW_CONTROL_STEP(C,B,A) ((void)0)
#endif

/** Control without STEP operation :
  * uFlowValue should be equal to CTRL_VALUE ==> execution stopped if failed !
  */
#if defined(FLOW_CONTROL)
#define FLOW_CONTROL_CHECK(B,A) \
  do{ \
    if ((B) != (A))\
    { \
      Error_Handler();\
    } \
  }while(0)
#else
#define FLOW_CONTROL_CHECK(B,A) ((void)0)
#endif

/** Control flow initialization
  */
#if defined(FLOW_CONTROL)
#define FLOW_CONTROL_INIT(B,A) \
  do{ \
    (B) = (A);\
  }while(0)
#else
#define FLOW_CONTROL_INIT(B,A) ((void)0)
#endif

/** STEP update only :
  * (uFlowValue XOR STEP_VALUE)
  */
#if defined(FLOW_CONTROL)
#define FLOW_STEP(B,A) \
  do{ \
    (B) ^= (A);\
  }while(0)
#else
#define FLOW_STEP(B,A) ((void)0)
#endif

/* Exported functions ------------------------------------------------------- */

/** @defgroup BOOT_FLOWCONTROL_Exported_Functions Exported Functions
  * @{
  */


/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_FLOWCONTROL_H */
