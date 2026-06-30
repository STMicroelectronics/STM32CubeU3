/*
 * Copyright (c) 2017-2018 ARM Limited
 * Copyright (c) 2024 STMicroelectronics. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__
#include "flash_layout.h"

#define BL2_HEAP_SIZE                           0x0000000
#define BL2_MSP_STACK_SIZE                      0x0002000

/* GTZC specific Alignment */
#define GTZC_RAM_ALIGN                          512
#define GTZC_FLASH_ALIGN                        8192

/*
 * This size of buffer is big enough to store an attestation
 * token produced by initial attestation service
 */
#define PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE       0x250

#if defined(STM32U366xx)
#define _SRAM2_TOP                              (0x40000) /* 256Kbytes */
#define _SRAM1_SIZE_MAX                         (0x20000)  /*!< SRAM1=128k*/
#define _SRAM2_SIZE_MAX                         (0x10000)  /*!< SRAM2=64k*/

/* Flash and internal SRAMs base addresses - Non secure aliased */
#define _FLASH_BASE_NS                          (0x08000000) /*!< FLASH(up to 512 KB) base address */
#define _SRAM1_BASE_NS                          (0x20000000) /*!< SRAM1(up to 128 KB) base address */
#define _SRAM2_BASE_NS                          (0x20020000) /*!< SRAM2(64 KB) base address */
/* Flash and internal SRAMs base addresses - Secure aliased */
#define _FLASH_BASE_S                           (0x0C000000) /*!< FLASH(up to 512 KB) base address */
#define _SRAM1_BASE_S                           (0x30000000) /*!< SRAM1(up to 192 KB) base address */
#define _SRAM2_BASE_S                           (0x30020000) /*!< SRAM2(64 KB) base address */

#else
#error "No STM32U3 version Defined"
#endif

#define TOTAL_ROM_SIZE                          FLASH_TOTAL_SIZE
#if defined(STM32U366xx)
#define S_TOTAL_RAM_SIZE                        (_SRAM2_SIZE_MAX) /*! size require for Secure part */
#endif

/*  This area in SRAM 2 is updated BL2 and can be lock to avoid any changes */
#define BOOT_SHARED_DATA_SIZE                   0x400
#define BOOT_SHARED_DATA_BASE                   (_SRAM2_BASE_S + _SRAM2_SIZE_MAX - BOOT_SHARED_DATA_SIZE)

/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_PARTITION_SIZE because we reserve space
 * for the image header and trailer introduced by the bootloader.
 */

#define BL2_HEADER_SIZE                         (0x400) /*!< Appli image header size */
#define BL2_DATA_HEADER_SIZE                    (0x20)  /*!< Data image header size */
#define BL2_TRAILER_SIZE                        (0x2000)

#define S_IMAGE_PRIMARY_PARTITION_OFFSET        (FLASH_AREA_0_OFFSET)
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET       (FLASH_AREA_0_OFFSET + FLASH_S_PARTITION_SIZE)
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
#define S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET   FLASH_AREA_4_OFFSET
#endif /* MCUBOOT_S_DATA_IMAGE_NUMBER == 1 */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
#define NS_DATA_IMAGE_PRIMARY_PARTITION_OFFSET  FLASH_AREA_5_OFFSET
#endif /* MCUBOOT_NS_DATA_IMAGE_NUMBER == 1 */

/* Use SRAM1 memory to store Code data */
#define S_ROM_ALIAS_BASE                        (_FLASH_BASE_S)
#define NS_ROM_ALIAS_BASE                       (_FLASH_BASE_NS)


#define S_RAM_ALIAS_BASE                        (_SRAM1_BASE_S)
#define NS_RAM_ALIAS_BASE                       (_SRAM1_BASE_NS)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)                          (S_ROM_ALIAS_BASE + (x))
#define NS_ROM_ALIAS(x)                         (NS_ROM_ALIAS_BASE + (x))

#define LOADER_NS_ROM_ALIAS(x)                  (_FLASH_BASE_NS + (x))
#define LOADER_S_ROM_ALIAS(x)                   (_FLASH_BASE_S + (x))

#define S_RAM_ALIAS(x)                          (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x)                         (NS_RAM_ALIAS_BASE + (x))

#define S_IMAGE_PRIMARY_AREA_OFFSET             (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)

/* Definition for mcuboot for fast wake-up from low power feature */
#define S_CODE_START                            (S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET))

/* Personalized region */
#define PERSO_START                             (S_ROM_ALIAS(FLASH_AREA_PERSO_OFFSET))
#define PERSO_SIZE                              (FLASH_AREA_PERSO_SIZE)
#define PERSO_LIMIT                             (PERSO_START + PERSO_SIZE - 1)

/* Bootloader region protected by hdp */
#if defined (OEMUROT_ENABLE)
#define BL2_CODE_START                          (S_ROM_ALIAS(FLASH_AREA_BL2_OFFSET + BL2_HEADER_SIZE))
#define BL2_CODE_SIZE                           (FLASH_AREA_BL2_SIZE - BL2_HEADER_SIZE)
#else
#define BL2_CODE_START                          (S_ROM_ALIAS(FLASH_AREA_BL2_OFFSET))
#define BL2_CODE_SIZE                           (FLASH_AREA_BL2_SIZE)
#endif /* OEMUROT_ENABLE */
#define BL2_CODE_LIMIT                          (BL2_CODE_START + BL2_CODE_SIZE - 1)

/* Bootloader region not protected by hdp */
#define BL2_NOHDP_CODE_START                    (S_ROM_ALIAS(FLASH_AREA_BL2_NOHDP_OFFSET))
#define BL2_NOHDP_CODE_SIZE                     (FLASH_AREA_BL2_NOHDP_SIZE)
#define BL2_NOHDP_CODE_LIMIT                    (BL2_NOHDP_CODE_START + BL2_NOHDP_CODE_SIZE - 1)

/* Bootloader boot address */
#define BL2_BOOT_VTOR_ADDR                      (BL2_CODE_START)

/*  keep 256 bytes unused to place while(1) for non secure to enable */
/*  regression from local tool with non secure attachment
 *  This avoid blocking board in case of hardening error */
#define BL2_DATA_START                          (_SRAM2_BASE_S)
#define BL2_DATA_SIZE                           (_SRAM2_SIZE_MAX - BOOT_SHARED_DATA_SIZE)
#define BL2_DATA_LIMIT                          (BL2_DATA_START + BL2_DATA_SIZE - 1)

/* Define BL2 MPU SRAM protection to remove execution capability */
/* Area is covering the complete SRAM memory space non secure alias and secure alias */
#define BL2_SRAM_AREA_BASE                      (_SRAM1_BASE_NS)
#define BL2_SRAM_AREA_END                       (_SRAM2_BASE_S + _SRAM2_SIZE_MAX - 1)

/* OEMiROT non volatile data (NVCNT/PS/ITS) region */
#define OEMIROT_NV_DATA_START                   (S_ROM_ALIAS(FLASH_NV_COUNTERS_AREA_OFFSET))
#define OEMIROT_NV_DATA_SIZE                    (FLASH_NV_COUNTER_AREA_SIZE + \
                                                 FLASH_PS_AREA_SIZE + FLASH_ITS_AREA_SIZE)

#endif /* __REGION_DEFS_H__ */
