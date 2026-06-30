/**
  ******************************************************************************
  * @file    appli_region_defs.h
  * @author  MCD Application Team
  * @brief   This file contains definitions of memory regions.
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

#ifndef __APPLI_REGION_DEFS_H__
#define __APPLI_REGION_DEFS_H__

#if 0
#include "stm32u3xx.h"
#else

/*!< Flash, Peripheral and internal SRAMs base addresses - secure */
#ifndef FLASH_BASE_S
#define FLASH_BASE_S                    0x0C000000    /*!< FLASH secure base address        */
#endif

#ifndef SRAM2_BASE_S
#define SRAM2_BASE_S                    0x30020000    /*!< SRAM2 secure base address        */
#endif

#ifndef SRAM2_SIZE
#define SRAM2_SIZE                      0x00010000    /*!< SRAM2=64k  */
#endif

#endif

#include "appli_flash_layout.h"

#define BL2_HEADER_SIZE                     (0x400) /*!< Appli image header size */
#define BL2_DATA_HEADER_SIZE                (0x20)  /*!< Data image header size */
#define BL2_TRAILER_SIZE                    (0x2000)

#define S_ROM_ALIAS_BASE                    (FLASH_BASE_S)

#define S_RAM_ALIAS_BASE                    (SRAM1_BASE_S)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)                      (S_ROM_ALIAS_BASE + (x))

#define S_RAM_ALIAS(x)                      (S_RAM_ALIAS_BASE + (x))

#define IMAGE_S_CODE_SIZE                   (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

#define S_IMAGE_PRIMARY_AREA_OFFSET         (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)

#define S_CODE_START                        (S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET))
#define S_CODE_SIZE                         (IMAGE_S_CODE_SIZE)

#define S_RAM_START                         (SRAM2_BASE_S)
#define S_RAM_SIZE                          (SRAM2_SIZE)

/* Secondary partition for new images/ in case of firmware upgrade */
#define SECONDARY_PARTITION_START           (NS_ROM_ALIAS(S_IMAGE_SECONDARY_PARTITION_OFFSET))
#define SECONDARY_PARTITION_SIZE            (FLASH_AREA_2_SIZE)

#define NSC_CODE_START                      (S_ROM_ALIAS(S_IMAGE_PRIMARY_PARTITION_OFFSET) + FLASH_S_PARTITION_SIZE - NSC_CODE_SIZE - PARTITION_RESERVED)

#endif /* __APPLI_REGION_DEFS_H__ */
