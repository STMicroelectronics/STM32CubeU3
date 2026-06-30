/**
  ******************************************************************************
  * @file    low_level_security.c
  * @author  MCD Application Team
  * @brief   security protection implementation for secure boot
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
  */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "boot_hal_cfg.h"
#include "boot_hal_flowcontrol.h"
#include "mpu_armv8m_drv.h"
#include "region_defs.h"
#include "mcuboot_config/mcuboot_config.h"
#include "low_level_security.h"
#ifdef OEMIROT_DEV_MODE
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_INFO
#else
#define BOOT_LOG_LEVEL BOOT_LOG_LEVEL_OFF
#endif /* OEMIROT_DEV_MODE  */
#include "bootutil/bootutil_log.h"
#if (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
#include "low_level_rng.h"
#endif /*  (OEMIROT_TAMPER_ENABLE != NO_TAMPER)*/
#include "target_cfg.h"
#include "bootutil_priv.h"

/** @defgroup OEMIROT_SECURITY_Private_Defines  Private Defines
  * @{
  */
/* DUAL BANK page size */
#define PAGE_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

#define BANK_NUMBER 2
#define PAGE_MAX_NUMBER_IN_BANK 63

/* OEMiROT_Boot Vector Address  */
#define OEMIROT_BOOT_VTOR_ADDR ((uint32_t)(BL2_CODE_START))

     /**
  * @brief  check register value different from the expected value
  *
  * @param  REG 32-bit register to read
  * @param  VAL 32-bit mask and expected register value
  * @retval None
  */
#define CHECK_REG(REG, VAL)  (READ_BIT((REG), (VAL)) != (VAL))

const struct mpu_armv8m_region_cfg_t region_cfg_init_s[] =
{
  /* Region 0: Allows execution of BL2 (OEMiROT Boot) */
  {
    0,
    FLASH_BASE_S + FLASH_AREA_BL2_OFFSET,
    FLASH_BASE_S + FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
    MPU_ARMV8M_XN_EXEC_OK,
    MPU_ARMV8M_AP_RO_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R0,
    FLOW_CTRL_MPU_S_I_EN_R0,
    FLOW_STEP_MPU_S_I_CH_R0,
    FLOW_CTRL_MPU_S_I_CH_R0,
#endif /* FLOW_CONTROL */
  },
  /* Region 1: Allows RW access for HASH REF - NVCNT */
  {
    1,
    FLASH_BASE_S,
    FLASH_BASE_S + FLASH_AREA_PERSO_OFFSET - 1,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R1,
    FLOW_CTRL_MPU_S_I_EN_R1,
    FLOW_STEP_MPU_S_I_CH_R1,
    FLOW_CTRL_MPU_S_I_CH_R1,
#endif /* FLOW_CONTROL */
  },
  /* Region 2 : Allows RW access to all secure PRIMARY SLOTS areas */
  {
    2,
    FLASH_BASE_S + FLASH_AREA_BEGIN_OFFSET,
    FLASH_BASE_S + FLASH_AREA_BEGIN_OFFSET + FLASH_S_DATA_PARTITION_SIZE  + FLASH_S_PARTITION_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R2,
    FLOW_CTRL_MPU_S_I_EN_R2,
    FLOW_STEP_MPU_S_I_CH_R2,
    FLOW_CTRL_MPU_S_I_CH_R2,
#endif /* FLOW_CONTROL */
  },
  /* Region 3 : Allows RO access to PERSO DATA area */
  {
    3,
    FLASH_BASE_S + FLASH_AREA_PERSO_OFFSET,
    FLASH_BASE_S + FLASH_AREA_PERSO_OFFSET + FLASH_AREA_PERSO_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R3,
    FLOW_CTRL_MPU_S_I_EN_R3,
    FLOW_STEP_MPU_S_I_CH_R3,
    FLOW_CTRL_MPU_S_I_CH_R3,
#endif /* FLOW_CONTROL */
  },
  /* Region 4: RW/PRIV_ONLY/EXEC_NEVER Forbid execution on full SRAM's area */
  {
    4,
    BL2_DATA_START,
    BL2_DATA_START + BOOT_SHARED_DATA_SIZE + BL2_DATA_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R4,
    FLOW_CTRL_MPU_S_I_EN_R4,
    FLOW_STEP_MPU_S_I_CH_R4,
    FLOW_CTRL_MPU_S_I_CH_R4,
#endif /* FLOW_CONTROL */
  },
  /* Region 5: Allows RW access to peripherals */
  {
    5,
    PERIPH_BASE_NS,
    PERIPH_BASE_S + 0xFFFFFFF,
    MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R5,
    FLOW_CTRL_MPU_S_I_EN_R5,
    FLOW_STEP_MPU_S_I_CH_R5,
    FLOW_CTRL_MPU_S_I_CH_R5,
#endif /* FLOW_CONTROL */
  },
  /* Region 6: Allows execution of RSSLIB */
  {
    6,
    RSS_LIB_BASE,
    RSS_LIB_BASE + RSS_LIB_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
    MPU_ARMV8M_XN_EXEC_OK,
    MPU_ARMV8M_AP_RO_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R6,
    FLOW_CTRL_MPU_S_I_EN_R6,
    FLOW_STEP_MPU_S_I_CH_R6,
    FLOW_CTRL_MPU_S_I_CH_R6,
#endif /* FLOW_CONTROL */
  },
  /* Region 7: Allows read access to STM32 descriptors */
  /* start = the start of the new descriptor address and the
     end = the end of the old descriptor address */
  {
    7,
    STM32_DESCRIPTOR_BASE_NS,
    STM32_DESCRIPTOR_END_NS,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RO_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R7,
    FLOW_CTRL_MPU_S_I_EN_R7,
    FLOW_STEP_MPU_S_I_CH_R7,
    FLOW_CTRL_MPU_S_I_CH_R7,
#endif /* FLOW_CONTROL */
  },
  /* Region 8: Allows RW access to all Non Secure SLOTS areas */
  /* Secure HAL flash driver uses non secure flash address to perform access to non secure flash area */
  {
    8,
    FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET,
    FLASH_BASE_NS + FLASH_AREA_END_OFFSET - 1,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R8,
    FLOW_CTRL_MPU_S_I_EN_R8,
    FLOW_STEP_MPU_S_I_CH_R8,
    FLOW_CTRL_MPU_S_I_CH_R8,
#endif /* FLOW_CONTROL */
  },
#if !defined(MCUBOOT_OVERWRITE_ONLY)
  /* Region 9 : Allows RW access to SCRATCH area */
  {
    9,
    FLASH_BASE_S + FLASH_AREA_SCRATCH_OFFSET,
    FLASH_BASE_S + FLASH_AREA_SCRATCH_OFFSET + FLASH_AREA_SCRATCH_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_S_I_EN_R9,
    FLOW_CTRL_MPU_S_I_EN_R9,
    FLOW_STEP_MPU_S_I_CH_R9,
    FLOW_CTRL_MPU_S_I_CH_R9,
#endif /* FLOW_CONTROL */
  },
#endif
};

const struct mpu_armv8m_region_cfg_t region_cfg_appli_s[] =
{
  /* First region in this list is configured only at this stage, */
  /* the region will be activated later by RSS jump service. Following regions */
  /*  in this list are configured and activated at this stage. */

  /* Region 1 is now enable for execution */
  {
    1,
    FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET,
    FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - 1 - (~MPU_RLAR_LIMIT_Msk + 1),
    MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
    MPU_ARMV8M_XN_EXEC_OK,
    MPU_ARMV8M_AP_RO_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_A_EN_R1,
    FLOW_CTRL_MPU_A_EN_R1,
    FLOW_STEP_MPU_A_CH_R1,
    FLOW_CTRL_MPU_A_CH_R1,
#endif /* FLOW_CONTROL */
  },
  /* Region 2: Allows RW access to end of area 0 (code image) for image confirmation (swap mode) */
  {
    2,
    FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - (~MPU_RLAR_LIMIT_Msk + 1),
    FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_A_EN_R2,
    FLOW_CTRL_MPU_A_EN_R2,
    FLOW_STEP_MPU_A_CH_R2,
    FLOW_CTRL_MPU_A_CH_R2,
#endif /* FLOW_CONTROL */
  },
#if MCUBOOT_S_DATA_IMAGE_NUMBER == 1
  /* Region 3: Allows RW access to end of area 4 (data image) for image confirmation (swap mode) */
  {
    3,
    FLASH_BASE_S + S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_DATA_PARTITION_SIZE - (~MPU_RLAR_LIMIT_Msk + 1),
    FLASH_BASE_S + S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_DATA_PARTITION_SIZE - 1,
    MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
    MPU_ARMV8M_XN_EXEC_NEVER,
    MPU_ARMV8M_AP_RW_PRIV_ONLY,
    MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
    FLOW_STEP_MPU_A_EN_R3,
    FLOW_CTRL_MPU_A_EN_R3,
    FLOW_STEP_MPU_A_CH_R3,
    FLOW_CTRL_MPU_A_CH_R3,
#endif /* FLOW_CONTROL */
  },
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
};

const struct sau_cfg_t sau_init_cfg[] =
{
  /* Region 1: Allows non secure access to FLASH peripheral registers used by secure HAL flash driver */
  /*           to perform access to non secure flash area */
  {
    1,
    (uint32_t)FLASH_NS,
    ((uint32_t)FLASH_NS + 0xffff),
    OEMIROT_FALSE,
#ifdef FLOW_CONTROL
    FLOW_STEP_SAU_I_EN_R1,
    FLOW_CTRL_SAU_I_EN_R1,
    FLOW_STEP_SAU_I_CH_R1,
    FLOW_CTRL_SAU_I_CH_R1,
#endif /* FLOW_CONTROL */
  },
  /* Region 2: Allow non secure Flash base access to all Non Secure SLOTS areas */
  {
    2,
    ((uint32_t)FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET),
    ((uint32_t)FLASH_BASE_NS + FLASH_AREA_END_OFFSET - 1),
    OEMIROT_FALSE,
#ifdef FLOW_CONTROL
    FLOW_STEP_SAU_I_EN_R2,
    FLOW_CTRL_SAU_I_EN_R2,
    FLOW_STEP_SAU_I_CH_R2,
    FLOW_CTRL_SAU_I_CH_R2,
#endif /* FLOW_CONTROL */
  },
  /* Region 3: Allows non secure access to RSS lib descriptors */
  {
    3,
    STM32_DESCRIPTOR_BASE_NS,
    STM32_DESCRIPTOR_END_NS,
    OEMIROT_FALSE,
#ifdef FLOW_CONTROL
    FLOW_STEP_SAU_I_EN_R3,
    FLOW_CTRL_SAU_I_EN_R3,
    FLOW_STEP_SAU_I_CH_R3,
    FLOW_CTRL_SAU_I_CH_R3,
#endif /* FLOW_CONTROL */
  },
};

/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup OEMIROT_SECURITY_Private_Functions  Private Functions
  * @{
  */
#ifdef OEMIROT_OB_RDP_LEVEL_VALUE
static void rdp_level(uint32_t rdplevel, uint32_t current_rdplevel);
#endif /* OEMIROT_OB_RDP_LEVEL_VALUE */
static void gtzc_init_cfg(void);
static void sau_and_idau_cfg(void);
static void mpu_init_cfg(void);
static void mpu_appli_cfg(void);
static void flash_priv_cfg(void);
static void active_tamper(void);
/**
  * @}
  */

/** @defgroup OEMIROT_SECURITY_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  Apply the runtime security  protections to
  *
  * @param  None
  * @note   By default, the best security protections are applied
  * @retval None
  */
void LL_SECU_ApplyRunTimeProtections(void)
{
  /* Unsecure bottom of SRAM1 for error_handler */
  gtzc_init_cfg();

  /* Set MPU to forbidd execution outside of not muteable code  */
  /* Initialize not secure MPU to forbidd execution on Flash /SRAM */
  mpu_init_cfg();

  /* Enable SAU to gain access to flash area non secure for write/read */
  sau_and_idau_cfg();
  /* With OEMIROT_DEV_MODE , active tamper calls Error_Handler */
  /* Error_Handler requires sau_and_idau_cfg */
  active_tamper();

  /* Configure Flash Privilege access */
  flash_priv_cfg();
}

/**
  * @brief  Update the runtime security protections for application start
  *
  * @param  None
  * @retval None
  */
void LL_SECU_UpdateRunTimeProtections(void)
{
  /* Set MPU to enable execution of Secure and Non Secure active slots */
  mpu_appli_cfg();
}

/**
  * @brief  Check if the Static security  protections to
  *         all the Sections in Flash:  WRP, SECURE FLASH, SECURE USER FLASH.
  *         those protections not impacted by a Reset. They are set using the Option Bytes
  *         When the device is locked (RDP Level2), these protections cannot be changed anymore
  * @param  None
  * @note   By default, the best security protections are applied to the different
  *         flash sections in order to maximize the security level for the specific MCU.
  * @retval None
  */
void LL_SECU_CheckStaticProtections(void)
{
  static FLASH_OBProgramInitTypeDef flash_option_bytes_bank1 = {0};
  static FLASH_OBProgramInitTypeDef flash_option_bytes_bank2 = {0};
  uint32_t start;
  uint32_t end;
#ifdef OEMIROT_NSBOOT_CHECK_ENABLE
  static FLASH_OBProgramInitTypeDef flash_option_bytes_nsboot0 = {0};
  static FLASH_OBProgramInitTypeDef flash_option_bytes_nsboot1 = {0};

    /* Get NSBOOTADD0 and NSBOOTADD1 value */
  flash_option_bytes_nsboot0.BootAddrConfig = OB_BOOTADDR_NS0;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_nsboot0);
  flash_option_bytes_nsboot1.BootAddrConfig = OB_BOOTADDR_NS1;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_nsboot1);
#endif /* OEMIROT_NSBOOT_CHECK_ENABLE */

  /* Get bank1 areaA OB  */
  flash_option_bytes_bank1.WRPArea = OB_WRPAREA_BANK1_AREAA;
  flash_option_bytes_bank1.WMSecConfig = OB_WMSEC_AREA1;
  flash_option_bytes_bank1.BootAddrConfig = OB_BOOTADDR_SEC0;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_bank1);
  /* Get bank2 areaB OB  */
  flash_option_bytes_bank2.WRPArea = OB_WRPAREA_BANK2_AREAA;
  flash_option_bytes_bank2.WMSecConfig = OB_WMSEC_AREA2;
  HAL_FLASHEx_OBGetConfig(&flash_option_bytes_bank2);


  /* Check TZEN = 1 , we are in secure */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_TZEN) != FLASH_OPTR_TZEN)
  {
    BOOT_LOG_ERR("Unexpected value for TZEN");
    Error_Handler();
  }

  /* Check if dual bank is set */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_DUALBANK) != FLASH_OPTR_DUALBANK)
  {
    BOOT_LOG_ERR("Unexpected value for dual bank configuration");
    Error_Handler();
  }

  /* Check if swap bank is reset */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_SWAP_BANK) != 0)
  {
    BOOT_LOG_ERR("Unexpected value for swap bank configuration");
    Error_Handler();
  }

#if !defined (OEMUROT_ENABLE)
  /* Check secure boot address */
  if (flash_option_bytes_bank1.BootAddr != BL2_BOOT_VTOR_ADDR)
  {
    BOOT_LOG_INF("BootAddr 0x%x", flash_option_bytes_bank1.BootAddr);
    BOOT_LOG_ERR("Unexpected value for SEC BOOT Address");
    Error_Handler();
  }
#endif /* OEMUROT_ENABLE */

#ifdef OEMIROT_NSBOOT_CHECK_ENABLE
  /* Check non-secure boot addresses */
  if ((flash_option_bytes_nsboot0.BootAddr != BL2_BOOT_VTOR_ADDR)
      || (flash_option_bytes_nsboot1.BootAddr != BL2_BOOT_VTOR_ADDR))
  {
    BOOT_LOG_ERR("Unexpected value for NS BOOT Address");
    Error_Handler();
  }
#endif /* OEMIROT_NSBOOT_CHECK_ENABLE */

#if defined(OEMIROT_FIRST_BOOT_STAGE)
  /* Check bank1 secure flash protection */
  end = (S_IMAGE_PRIMARY_PARTITION_OFFSET  + FLASH_S_PARTITION_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = PAGE_MAX_NUMBER_IN_BANK;
  }
  start = 0;

  if ((start > flash_option_bytes_bank1.WMSecStartPage)
      || (end > flash_option_bytes_bank1.WMSecEndPage))
  {
    BOOT_LOG_INF("BANK 1 secure flash [%d, %d] : OB [%d, %d]",
                 start, end, flash_option_bytes_bank1.WMSecStartPage, flash_option_bytes_bank1.WMSecEndPage);

    BOOT_LOG_ERR("Unexpected value for secure flash protection");
    Error_Handler();
  }
  /* Check bank2 secure flash protection */
  start = 0;
  end = (S_IMAGE_PRIMARY_PARTITION_OFFSET  + FLASH_S_PARTITION_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = end - (PAGE_MAX_NUMBER_IN_BANK + 1);
    if ((start > flash_option_bytes_bank2.WMSecStartPage)
        || (end > flash_option_bytes_bank2.WMSecEndPage))
    {
      BOOT_LOG_INF("BANK 2 secure flash [%d, %d] : OB [%d, %d]", start, end, flash_option_bytes_bank2.WMSecStartPage,
                   flash_option_bytes_bank2.WMSecEndPage);
    }
  }
  /* the bank 2 must be fully unsecure */
  else if (flash_option_bytes_bank2.WMSecEndPage >= flash_option_bytes_bank2.WMSecStartPage)
  {
    BOOT_LOG_INF("BANK 2 secure flash [%d, %d] : OB [%d, %d]", PAGE_MAX_NUMBER_IN_BANK, 0,
                 flash_option_bytes_bank2.WMSecStartPage,
                 flash_option_bytes_bank2.WMSecEndPage);
  }
#else
  /* Check bank1 secure flash protection */
  start = 0;
  end = (S_IMAGE_PRIMARY_PARTITION_OFFSET  + FLASH_S_PARTITION_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = PAGE_MAX_NUMBER_IN_BANK;
  }
  start = 0;
  if ((start != flash_option_bytes_bank1.WMSecStartPage)
      || (end != flash_option_bytes_bank1.WMSecEndPage))
  {
    BOOT_LOG_INF("BANK 1 secure flash [%d, %d] : OB [%d, %d]",
                 start, end, flash_option_bytes_bank1.WMSecStartPage, flash_option_bytes_bank1.WMSecEndPage);

    BOOT_LOG_ERR("Unexpected value for secure flash protection");
    Error_Handler();
  }
  /* Check bank2 secure flash protection */
  start = 0;
  end = (S_IMAGE_PRIMARY_PARTITION_OFFSET  + FLASH_S_PARTITION_SIZE - 1) / PAGE_SIZE;
  if (end > PAGE_MAX_NUMBER_IN_BANK)
  {
    end = end - (PAGE_MAX_NUMBER_IN_BANK + 1);
    if ((start != flash_option_bytes_bank2.WMSecStartPage)
        || (end != flash_option_bytes_bank2.WMSecEndPage))
    {
      BOOT_LOG_INF("BANK 2 secure flash [%d, %d] : OB [%d, %d]", start, end, flash_option_bytes_bank2.WMSecStartPage,
                   flash_option_bytes_bank2.WMSecEndPage);
    }
  }
  /* the bank 2 must be fully unsecure */
  else if (flash_option_bytes_bank2.WMSecEndPage >= flash_option_bytes_bank2.WMSecStartPage)
  {
    BOOT_LOG_INF("BANK 2 secure flash [%d, %d] : OB [%d, %d]", PAGE_MAX_NUMBER_IN_BANK, 0,
                 flash_option_bytes_bank2.WMSecStartPage,
                 flash_option_bytes_bank2.WMSecEndPage);
  }
#endif /* OEMIROT_FIRST_BOOT_STAGE */

#ifdef  OEMIROT_WRP_PROTECT_ENABLE

#if !defined (OEMUROT_ENABLE)
  /* Check flash write protection */
  start = FLASH_AREA_PERSO_OFFSET / PAGE_SIZE;
  end = (FLASH_AREA_PERSO_OFFSET + FLASH_AREA_PERSO_SIZE + FLASH_AREA_BL2_SIZE - 1) / PAGE_SIZE;

  if ((flash_option_bytes_bank1.WRPStartOffset > flash_option_bytes_bank1.WRPEndOffset)
      || (start != flash_option_bytes_bank1.WRPStartOffset)
      || (end != flash_option_bytes_bank1.WRPEndOffset))
  {
    BOOT_LOG_INF("BANK 1 flash write protection [%d, %d] : OB [%d, %d]", start, end,
                 flash_option_bytes_bank1.WRPStartOffset,
                 flash_option_bytes_bank1.WRPEndOffset);

    BOOT_LOG_ERR("Unexpected value for write protection AREA A ");
    Error_Handler();
  }
#endif /* OEMUROT_ENABLE */

#if  defined(OEMIROT_WRP_LOCK_ENABLE)
  /* Check WRP lock protection */
  if (flash_option_bytes_bank1.WRPLock != ENABLE)
  {
    BOOT_LOG_INF("BANK 1 WRP Lock 0x%x", flash_option_bytes_bank1.WRPLock);
    BOOT_LOG_ERR("Unexpected value for WRP Lock");
    Error_Handler();
  }
#endif /* OEMIROT_WRP_LOCK_ENABLE */

#endif /* OEMIROT_WRP_PROTECT_ENABLE */

#ifdef  OEMIROT_HDP_PROTECT_ENABLE

#if !defined (OEMUROT_ENABLE)

  /* Check secure user flash protection (HDP) */
  start = 0;
  end = (FLASH_BL2_HDP_END_OFFSET - 1) / PAGE_SIZE;
  if (
    (flash_option_bytes_bank1.WMSecStartPage > flash_option_bytes_bank1.WMHDPEndPage)
    || (start < flash_option_bytes_bank1.WMSecStartPage)
    || (end > flash_option_bytes_bank1.WMHDPEndPage)
    || (flash_option_bytes_bank1.WMSecConfig & OB_WMSEC_HDP_AREA_DISABLE))
  {
    BOOT_LOG_INF("BANK 1 secure user flash [%d, %d] : OB [%d, %d]",
                 start,
                 end,
                 flash_option_bytes_bank1.WMSecStartPage,
                 flash_option_bytes_bank1.WMHDPEndPage);
    BOOT_LOG_ERR("Unexpected value for secure user flash protection");
    Error_Handler();
  }
#endif  /* OEMUROT_ENABLE */

#else /* OEMIROT_HDP_PROTECT_ENABLE */
  flash_option_bytes_bank1.WMSecConfig &= ~(OB_WMSEC_HDP_AREA_CONFIG | OB_WMSEC_HDP_AREA_ENABLE);
  flash_option_bytes_bank1.WMSecConfig |= OB_WMSEC_HDP_AREA_DISABLE;
#endif /* OEMIROT_HDP_PROTECT_ENABLE */

#ifdef OEMIROT_SECURE_USER_SRAM2_ERASE_AT_RESET
  /* Check SRAM2 ERASE on reset */
  if ((flash_option_bytes_bank1.USERConfig & FLASH_OPTR_SRAM2_RST) != 0)
  {
    BOOT_LOG_ERR("Unexpected value for SRAM2 ERASE at Reset");
    Error_Handler();
  }
#endif /*OEMIROT_SECURE_USER_SRAM2_ERASE_AT_RESET */

#ifdef OEMIROT_OB_BOOT_LOCK
  /* Check Boot lock protection */
  if (flash_option_bytes_bank1.BootLock != OEMIROT_OB_BOOT_LOCK)
  {
    BOOT_LOG_INF("BootLock 0x%x", flash_option_bytes_bank1.BootLock);
    BOOT_LOG_ERR("Unexpected value for SEC BOOT LOCK");
    Error_Handler();
  }
#endif /* OEMIROT_OB_BOOT_LOCK */

#ifdef OEMIROT_OB_RDP_LEVEL_VALUE
  /* Check RDL level : boot if current RDP level is greater or equal to selected RDP level */
  switch (OEMIROT_OB_RDP_LEVEL_VALUE)
  {
    case OB_RDP_LEVEL_2:
      if (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_2)
      {
        rdp_level(OEMIROT_OB_RDP_LEVEL_VALUE, flash_option_bytes_bank1.RDPLevel);
        Error_Handler_rdp();
      }
      break;

    case OB_RDP_LEVEL_1:
      if ((flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_2) &&
          (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_1))
      {
        rdp_level(OEMIROT_OB_RDP_LEVEL_VALUE, flash_option_bytes_bank1.RDPLevel);
        Error_Handler_rdp();
      }
      break;

    case OB_RDP_LEVEL_0_5:
      if ((flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_2) &&
          (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_1) &&
          (flash_option_bytes_bank1.RDPLevel != OB_RDP_LEVEL_0_5))
      {
        rdp_level(OEMIROT_OB_RDP_LEVEL_VALUE, flash_option_bytes_bank1.RDPLevel);
        Error_Handler_rdp();
      }
      break;

    case OB_RDP_LEVEL_0:
      break;

    default:
      Error_Handler();
      break;
  }
#endif /* OEMIROT_OB_RDP_LEVEL_VALUE */
}

#ifdef OEMIROT_OB_RDP_LEVEL_VALUE
static void rdp_level(uint32_t rdplevel, uint32_t current_rdplevel)
{
  BOOT_LOG_INF("RDPLevel 0x%x (0x%x)", current_rdplevel, rdplevel);
  BOOT_LOG_ERR("Unexpected value for RDP level");
}
#endif /* OEMIROT_OB_RDP_LEVEL_VALUE */

/**
  * @brief  Memory Config Init
  * @param  None
  * @retval None
  */
static void  gtzc_init_cfg(void)
{
  __HAL_RCC_GTZC1_CLK_ENABLE();
}

/**
  * @brief  Sau idau init
  * @param  None
  * @retval None
  */
static void sau_and_idau_cfg(void)
{
  uint32_t i;
  uint32_t rnr;
  uint32_t rbar;
  uint32_t rlar;
  uint32_t rnr_reg;
  uint32_t rbar_reg;
  uint32_t rlar_reg;
  uint32_t ctrl_reg;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Disable SAU */
    SAU->CTRL &= ~(SAU_CTRL_ENABLE_Msk);

    for (i = 0; i < ARRAY_SIZE(sau_init_cfg); i++)
    {
      SAU->RNR = sau_init_cfg[i].RNR;
      SAU->RBAR = sau_init_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
      SAU->RLAR = (sau_init_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
                  (sau_init_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
                  SAU_RLAR_ENABLE_Msk;

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, sau_init_cfg[i].flow_step_enable,
                        sau_init_cfg[i].flow_ctrl_enable);
    }

    /* Force memory writes before continuing */
    __DSB();
    /* Flush and refill pipeline with updated permissions */
    __ISB();
    /* Enable SAU */
    SAU->CTRL |= (SAU_CTRL_ENABLE_Msk);

    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_I_EN, FLOW_CTRL_SAU_I_EN);
  }
  /* verification stage */
  else
  {
    for (i = 0; i < ARRAY_SIZE(sau_init_cfg); i++)
    {
      SAU->RNR = sau_init_cfg[i].RNR;

      rnr = sau_init_cfg[i].RNR;
      rbar = sau_init_cfg[i].RBAR & SAU_RBAR_BADDR_Msk;
      rlar = (sau_init_cfg[i].RLAR & SAU_RLAR_LADDR_Msk) |
             (sau_init_cfg[i].nsc ? SAU_RLAR_NSC_Msk : 0U) |
             SAU_RLAR_ENABLE_Msk;

      rnr_reg = SAU->RNR;
      rbar_reg = SAU->RBAR;
      rlar_reg = SAU->RLAR;

      if ((rnr_reg != rnr) || (rbar_reg != rbar) || (rlar_reg != rlar))
      {
        Error_Handler();
      }

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, sau_init_cfg[i].flow_step_check,
                        sau_init_cfg[i].flow_ctrl_check);
    }

    ctrl_reg = SAU->CTRL;
    if ((ctrl_reg & SAU_CTRL_ENABLE_Msk) != SAU_CTRL_ENABLE_Msk)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_I_CH, FLOW_CTRL_SAU_I_CH);
    }
  }
}

#if  defined(OEMUROT_ENABLE)
void LL_SECU_DisableCleanMpu(void)
{
  struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  mpu_armv8m_disable(&dev_mpu_s);
  mpu_armv8m_clean(&dev_mpu_s);
}
#endif

/**
  * @brief  mpu init
  * @param  None
  * @retval None
  */
static void mpu_init_cfg(void)
{
#ifdef OEMIROT_BOOT_MPU_PROTECTION
  struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  int32_t i;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* configure secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_s); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_s,
                                   (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_s[i].flow_step_enable,
                          region_cfg_init_s[i].flow_ctrl_enable);
      }
    }

    /* enable secure MPU */
    mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_DISABLE, HARDFAULT_NMI_ENABLE);
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_I_EN, FLOW_CTRL_MPU_S_I_EN);
  }
  /* verification stage */
  else
  {
    /* check secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_s); i++)
    {
      if (mpu_armv8m_region_enable_check(&dev_mpu_s,
                                         (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_s[i].flow_step_check,
                          region_cfg_init_s[i].flow_ctrl_check);
      }
    }

    /* check secure MPU */
    if (mpu_armv8m_check(&dev_mpu_s, PRIVILEGED_DEFAULT_DISABLE,
                         HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_I_CH, FLOW_CTRL_MPU_S_I_CH);
    }
  }
#endif /* OEMIROT_BOOT_MPU_PROTECTION */
}

static void mpu_appli_cfg(void)
{
#ifdef OEMIROT_BOOT_MPU_PROTECTION
  static struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  int32_t i;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* configure secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_appli_s); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_s,
                                   (struct mpu_armv8m_region_cfg_t *)&region_cfg_appli_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_appli_s[i].flow_step_enable,
                          region_cfg_appli_s[i].flow_ctrl_enable);
      }
    }
  }
  else
  {
    /* check secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_appli_s); i++)
    {
      if (mpu_armv8m_region_enable_check(&dev_mpu_s,
                                         (struct mpu_armv8m_region_cfg_t *)&region_cfg_appli_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_appli_s[i].flow_step_check,
                          region_cfg_appli_s[i].flow_ctrl_check);
      }
    }
  }
#endif /* OEMIROT_BOOT_MPU_PROTECTION */
}

/**
  * @brief  configure flash privilege access
  * @param  None
  * @retval None
  */
static void flash_priv_cfg(void)
{
#ifdef OEMIROT_FLASH_PRIVONLY_ENABLE
  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Configure Flash Privilege access */
    HAL_FLASHEx_ConfigPrivMode(FLASH_SPRIV_DENIED);
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_P_EN, FLOW_CTRL_FLASH_P_EN);
  }
  /* verification stage */
  else
  {
    if (HAL_FLASHEx_GetPrivMode() != FLASH_SPRIV_DENIED)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_P_CH, FLOW_CTRL_FLASH_P_CH);
    }
  }
#endif /*  OEMIROT_FLASH_PRIVONLY_ENABLE */
}

#if (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
const RTC_InternalTamperTypeDef InternalTamperConf =
{
  .IntTamper                  = TAMP_CR1_ITAMP9E,   /* Cryptographic peripherals fault */
  .TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE,
  .NoErase                    = RTC_TAMPER_ERASE_BACKUP_ENABLE
};

void TAMP_IRQHandler(void)
{
  NVIC_SystemReset();
}

#ifdef OEMIROT_DEV_MODE
extern volatile uint32_t TamperEventCleared;
#endif
#endif /* (OEMIROT_TAMPER_ENABLE != NO_TAMPER) */
RTC_HandleTypeDef RTCHandle;

static void active_tamper(void)
{
#if (OEMIROT_TAMPER_ENABLE == ALL_TAMPER)
  RTC_TamperTypeDef sTamper = {0};
#endif /* (OEMIROT_TAMPER_ENABLE == ALL_TAMPER) */
#if (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
  uint32_t DeviceSecretConf;
#endif /* OEMIROT_TAMPER_ENABLE != NO_TAMPER) */
  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
#if defined(OEMIROT_DEV_MODE) && (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
    if (TamperEventCleared)
    {
      BOOT_LOG_INF("Boot with TAMPER Event Active");
#if (OEMIROT_TAMPER_ENABLE == ALL_TAMPER)
      /* avoid several re-boot in DEV_MODE with Tamper active */
      BOOT_LOG_INF("Unplug the tamper cable, and reboot");
      BOOT_LOG_INF("Or");
#endif /* (OEMIROT_TAMPER_ENABLE == ALL_TAMPER) */
      BOOT_LOG_INF("Build and Flash with flag #define OEMIROT_TAMPER_ENABLE NO_TAMPER\n");
      Error_Handler();
    }
#endif /*  OEMIROT_DEV_MODE && (OEMIROT_TAMPER_ENABLE != NO_TAMPER) */
    /* enable write access to backup domain */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
#if (OEMIROT_TAMPER_ENABLE == ALL_TAMPER)
   /* RTC Init */
    RTCHandle.Instance = RTC;
    RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_12;
    RTCHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
    RTCHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
    RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
    RTCHandle.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
    RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_PUSHPULL;
    RTCHandle.Init.OutPutPullUp   = RTC_OUTPUT_PULLUP_NONE;

    if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
    {
      Error_Handler();
    }

    /** Enable the RTC Tamper 1 */
    sTamper.Tamper = RTC_TAMPER_1;
    sTamper.Trigger = RTC_TAMPERTRIGGER_LOWLEVEL;
    sTamper.NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE;
    sTamper.MaskFlag = RTC_TAMPERMASK_FLAG_DISABLE;
    sTamper.Filter = RTC_TAMPERFILTER_DISABLE;
    sTamper.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768;
    sTamper.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
    sTamper.TamperPullUp = RTC_TAMPER_PULLUP_ENABLE;
    if (HAL_RTCEx_SetTamper_IT(&RTCHandle, &sTamper) != HAL_OK)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_ACT_EN, FLOW_CTRL_TAMP_ACT_EN);
#elif defined(OEMIROT_DEV_MODE)
    /* Deactivation is required when OEMIROT_TAMPER_ENABLE moved from != NO_TAMPER to == NO_TAMPER with no power off */
    HAL_RTCEx_DeactivateTamper(&RTCHandle, RTC_TAMPER_ALL);
#else
    /* Keep the user tamper configuration */
#endif  /* (OEMIROT_TAMPER_ENABLE == ALL_TAMPER) */
#if (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
    /*  Internal Tamper activation  */
    /*  Enable Cryptographic IPs fault (tamp_itamp9), Backup domain voltage threshold monitoring (tamp_itamp1)*/
    if (HAL_RTCEx_SetInternalTamper_IT(&RTCHandle, (RTC_InternalTamperTypeDef *)&InternalTamperConf) != HAL_OK)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_EN, FLOW_CTRL_TAMP_INT_EN);

    /*  Set tamper configuration secure only  */
    SET_BIT(TAMP->SECCFGR, TAMP_SECURE_FULL_YES);
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_SEC_EN, FLOW_CTRL_TAMP_SEC_EN);

    /*  Set tamper configuration privileged only   */
    SET_BIT(TAMP->PRIVCFGR, TAMP_PRIVILEGE_FULL_YES);
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_PRIV_EN, FLOW_CTRL_TAMP_PRIV_EN);

    /*  Activate Secret Erase */
    DeviceSecretConf = (uint32_t)TAMP_DEVICESECRETS_ERASE_SRAM2 | TAMP_DEVICESECRETS_ERASE_CRYPTO | TAMP_DEVICESECRETS_ERASE_ICACHE |
                       TAMP_DEVICESECRETS_ERASE_HASH | TAMP_DEVICESECRETS_ERASE_PKA_SRAM;
    HAL_RTCEx_ConfigEraseDeviceSecrets(&RTCHandle, DeviceSecretConf);

    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_CFG_EN, FLOW_CTRL_TAMP_CFG_EN);
    BOOT_LOG_INF("TAMPER Activated");
#elif defined(OEMIROT_DEV_MODE)
    /* Deactivation is required when OEMIROT_TAMPER_ENABLE moved from != NO_TAMPER to == NO_TAMPER with no power off */
    HAL_RTCEx_DeactivateTamper(&RTCHandle, RTC_TAMPER_ALL);
#endif /* (OEMIROT_TAMPER_ENABLE != NO_TAMPER) */
    /* disable write access to backup domain */
   (void)READ_REG(TAMP->CR1); /* Dummy read to wait for write effectiveness in tamper */
    HAL_PWR_DisableBkUpAccess();
  }
#if (OEMIROT_TAMPER_ENABLE != NO_TAMPER)
  /* verification stage */
  else
  {
#if (OEMIROT_TAMPER_ENABLE == ALL_TAMPER)
    /*  Check External Tamper activation */
    const uint32_t expected_fltcr = sTamper.Filter | sTamper.SamplingFrequency | sTamper.PrechargeDuration | sTamper.TamperPullUp;
    if (((READ_REG(TAMP->CR1) & TAMP_CR1_TAMP1E) != TAMP_CR1_TAMP1E) ||
       ((READ_REG(TAMP->FLTCR) & expected_fltcr) != expected_fltcr) ||
        (READ_BIT(TAMP->CR2, (TAMP_CR2_TAMP1POM | TAMP_CR2_TAMP1TRG)) != 0x00000000U))
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_ACT_CH, FLOW_CTRL_TAMP_ACT_CH);
#endif  /* (OEMIROT_TAMPER_ENABLE == ALL_TAMPER) */
    /*  Check Internal Tamper activation */
    if((READ_BIT(RTC->CR, RTC_CR_TAMPTS) != InternalTamperConf.TimeStampOnTamperDetection) ||
        (CHECK_REG(TAMP->CR1, (TAMP_CR1_ITAMP9E))) ||
        (CHECK_REG(TAMP->IER, (TAMP_IER_ITAMP9IE))) ||
        (READ_BIT(TAMP->CR3, (TAMP_CR3_ITAMP9POM)) != 0x00000000U))
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_CH, FLOW_CTRL_TAMP_INT_CH);

    /*  Check tamper configuration secure only  */
     if (CHECK_REG(TAMP->SECCFGR, TAMP_SECURE_FULL_YES))
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_SEC_CH, FLOW_CTRL_TAMP_SEC_CH);

    /*  Check tamper configuration privileged only   */
    if (CHECK_REG(TAMP->PRIVCFGR, TAMP_PRIVILEGE_FULL_YES))
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_PRIV_CH, FLOW_CTRL_TAMP_PRIV_CH);

    /*  Check Secret Erase */
    DeviceSecretConf = (uint32_t)TAMP_DEVICESECRETS_ERASE_SRAM2 | TAMP_DEVICESECRETS_ERASE_CRYPTO | TAMP_DEVICESECRETS_ERASE_ICACHE |
                       TAMP_DEVICESECRETS_ERASE_HASH | TAMP_DEVICESECRETS_ERASE_PKA_SRAM;

    if (READ_REG(TAMP->RPCFGR) != DeviceSecretConf)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_CFG_CH, FLOW_CTRL_TAMP_CFG_CH);
  }
#endif /*  OEMIROT_TAMPER_ENABLE != NO_TAMPER */
}
/**
  * @}
  */
