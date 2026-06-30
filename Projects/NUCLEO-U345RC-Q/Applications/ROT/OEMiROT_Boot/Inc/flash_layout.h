/*
 * Copyright (c) 2018 Arm Limited. All rights reserved.
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

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */


/* Flash layout configuration : begin */

#define MCUBOOT_OVERWRITE_ONLY          /* Defined: the FW installation uses overwrite method.
                                           UnDefined: The FW installation uses swap mode. */

#define MCUBOOT_APP_IMAGE_NUMBER 2      /* 1: S application only if FLASH_NS_PARTITION_SIZE = 0 ,
                                              else S and NS application binaries assembled in one single image.
                                           2: Two separated images for S and NS application binaries. */

#define MCUBOOT_S_DATA_IMAGE_NUMBER 0   /* 1: S data image for S application.
                                           0: No S data image. */

#define MCUBOOT_NS_DATA_IMAGE_NUMBER 0  /* 1: NS data image for NS application.
                                           0: No NS data image. */

/* Flash layout configuration : end */

/* Total number of images */
#define MCUBOOT_IMAGE_NUMBER (MCUBOOT_APP_IMAGE_NUMBER + MCUBOOT_S_DATA_IMAGE_NUMBER + MCUBOOT_NS_DATA_IMAGE_NUMBER)

/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_PARTITION_SIZE because we reserve space
 * for the image header and trailer introduced by the bootloader.
 */
#define BL2_HEADER_SIZE                         (0x400) /*!< Appli image header size */
#define BL2_DATA_HEADER_SIZE                    (0x20)  /*!< Data image header size */
#define BL2_TRAILER_SIZE                        (0x2000)

/* Use image hash reference to reduce boot time (signature check bypass) */
#define MCUBOOT_USE_HASH_REF

/* control configuration */
#if defined(MCUBOOT_PRIMARY_ONLY) && !defined(MCUBOOT_OVERWRITE_ONLY)
#error "Config not supported: When MCUBOOT_PRIMARY_ONLY is enabled, MCUBOOT_OVERWRITE_ONLY is required."
#endif /* defined(MCUBOOT_PRIMARY_ONLY) */

/* The size of a partition. This should be large enough to contain a S or NS
 * sw binary. Each FLASH_AREA_IMAGE contains two partitions. See Flash layout
 * above.
 */

/* Flash layout info for BL2 bootloader */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)     /* 4 KB */
#if defined(STM32U345xx)
#define FLASH_B_SIZE                    (0x20000)   /* 128 KBytes*/
#else
#error "No STM32U3 version Defined"
#endif
#define FLASH_TOTAL_SIZE                (FLASH_B_SIZE+FLASH_B_SIZE) /* 256 KBytes */
#define FLASH_BASE_ADDRESS              (0x0C000000) /* same as FLASH0_BASE_S */

/* Flash area IDs */
#define FLASH_AREA_0_ID                 (1)
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
#define FLASH_AREA_1_ID                 (2)
#endif /* MCUBOOT_APP_IMAGE_NUMBER == 2 */
#if !defined(MCUBOOT_PRIMARY_ONLY)
#define FLASH_AREA_2_ID                 (3)
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
#define FLASH_AREA_3_ID                 (4)
#endif /* MCUBOOT_APP_IMAGE_NUMBER == 2 */
#endif /* MCUBOOT_PRIMARY_ONLY */
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
#define FLASH_AREA_4_ID                 (5)
#endif /* MCUBOOT_S_DATA_IMAGE_NUMBER == 1 */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
#define FLASH_AREA_5_ID                 (6)
#endif /* MCUBOOT_NS_DATA_IMAGE_NUMBER == 1 */
#if !defined(MCUBOOT_PRIMARY_ONLY)
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
#define FLASH_AREA_6_ID                 (7)
#endif /* MCUBOOT_S_DATA_IMAGE_NUMBER == 1 */
#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
#define FLASH_AREA_7_ID                 (8)
#endif /* MCUBOOT_NS_DATA_IMAGE_NUMBER == 1 */
#define FLASH_AREA_SCRATCH_ID           (9)
#endif /* MCUBOOT_PRIMARY_ONLY */

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_1, SCRATCH
 * is used as a temporary storage during image swapping.
 */
/* area for image HASH references */
#define FLASH_HASH_REF_AREA_OFFSET      (0x0000)
#if defined(MCUBOOT_USE_HASH_REF)
#define FLASH_HASH_REF_AREA_SIZE        (FLASH_AREA_IMAGE_SECTOR_SIZE + FLASH_AREA_IMAGE_SECTOR_SIZE)
#else
#define FLASH_HASH_REF_AREA_SIZE        (0x0000)
#endif /* MCUBOOT_USE_HASH_REF */

/* area for HUK and anti roll back counter */
#define FLASH_BL2_NVCNT_AREA_OFFSET     (FLASH_HASH_REF_AREA_OFFSET + FLASH_HASH_REF_AREA_SIZE)
#define FLASH_BL2_NVCNT_AREA_SIZE       (FLASH_AREA_IMAGE_SECTOR_SIZE + FLASH_AREA_IMAGE_SECTOR_SIZE)

/* personal area */
#define FLASH_AREA_PERSO_OFFSET         (FLASH_BL2_NVCNT_AREA_OFFSET + FLASH_BL2_NVCNT_AREA_SIZE)
#define FLASH_AREA_PERSO_SIZE           (0x2000)
/* control personal area */
#if (FLASH_AREA_PERSO_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_PERSO_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* FLASH_AREA_PERSO_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* area for BL2 code protected by hdp */
#define FLASH_AREA_BL2_OFFSET           (FLASH_AREA_PERSO_OFFSET+FLASH_AREA_PERSO_SIZE)



#if !defined(MCUBOOT_PRIMARY_ONLY) && !defined(MCUBOOT_OVERWRITE_ONLY)
#define FLASH_AREA_BL2_SIZE             (0x11000)
#else
#define FLASH_AREA_BL2_SIZE             (0x11000)
#endif
/* Control area */
#if (FLASH_AREA_BL2_SIZE  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_BL2_SIZE  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_BL2_SIZE  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* scratch area */
#if defined(FLASH_AREA_SCRATCH_ID)
#define FLASH_AREA_SCRATCH_DEVICE_ID    (FLASH_DEVICE_ID - FLASH_DEVICE_ID)

#define FLASH_AREA_SCRATCH_OFFSET       (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)

#if defined(MCUBOOT_OVERWRITE_ONLY)
#define FLASH_AREA_SCRATCH_SIZE         (0x0000) /* Not used in MCUBOOT_OVERWRITE_ONLY mode */
#else

#define FLASH_AREA_SCRATCH_SIZE         (0x6000) /* 24 KB */
#endif
/* control scratch area */
#if (FLASH_AREA_SCRATCH_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_SCRATCH_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_SCRATCH_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0*/
#else /* FLASH_AREA_SCRATCH_ID */
#define FLASH_AREA_SCRATCH_SIZE         (0x0)
#endif /* FLASH_AREA_SCRATCH_ID */

/* HDP area end at this address */
#define FLASH_BL2_HDP_END_OFFSET        (FLASH_AREA_SCRATCH_OFFSET+FLASH_AREA_SCRATCH_SIZE)

/* BL2 partitions size */
#if defined(MCUBOOT_PRIMARY_ONLY)
/* For PRIMARY_ONLY we need 520 KB for NS partition */
#define FLASH_NS_PARTITION_SIZE         (0xC800)
#else
/* For Others case, we limit the size to 64 KB for NS partition (except for FULL SECURE) */
#define FLASH_NS_PARTITION_SIZE         (0x9000)

#endif /* MCUBOOT_PRIMARY_ONLY */

#if (MCUBOOT_APP_IMAGE_NUMBER == 1) && (FLASH_NS_PARTITION_SIZE == 0)
/* For FULL SECURE case, we need at least 32 KB for S partition */
#define FLASH_S_PARTITION_SIZE          (0x08000)

#else
/* For Others case, we keep 32 KB for S partition (like FULL SECURE) */
#define FLASH_S_PARTITION_SIZE          (0x06000)

#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 1) && (FLASH_NS_PARTITION_SIZE == 0) */

#define FLASH_PARTITION_SIZE            (FLASH_S_PARTITION_SIZE+FLASH_NS_PARTITION_SIZE)

#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
#define FLASH_MAX_APP_PARTITION_SIZE    ((FLASH_S_PARTITION_SIZE >   \
                                          FLASH_NS_PARTITION_SIZE) ? \
                                         FLASH_S_PARTITION_SIZE : \
                                         FLASH_NS_PARTITION_SIZE)
#else
#define FLASH_MAX_APP_PARTITION_SIZE    FLASH_PARTITION_SIZE
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */

/* BL2 flash areas */
#define OEMIROT_AREA_0_OFFSET           (FLASH_AREA_BL2_OFFSET)
#define FLASH_AREA_BEGIN_OFFSET         (FLASH_BL2_HDP_END_OFFSET)

#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
#define FLASH_S_DATA_PARTITION_SIZE     (FLASH_AREA_IMAGE_SECTOR_SIZE)
#else
#define FLASH_S_DATA_PARTITION_SIZE     (0x0)
#endif /* (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */

#if (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
#define FLASH_NS_DATA_PARTITION_SIZE    (FLASH_AREA_IMAGE_SECTOR_SIZE)
#else
#define FLASH_NS_DATA_PARTITION_SIZE    (0x0)
#endif /* (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1) */

#define FLASH_MAX_DATA_PARTITION_SIZE   ((FLASH_S_DATA_PARTITION_SIZE >   \
                                          FLASH_NS_DATA_PARTITION_SIZE) ? \
                                         FLASH_S_DATA_PARTITION_SIZE : \
                                         FLASH_NS_DATA_PARTITION_SIZE)
#define FLASH_MAX_PARTITION_SIZE        ((FLASH_MAX_APP_PARTITION_SIZE >   \
                                         FLASH_MAX_DATA_PARTITION_SIZE) ?  \
                                         ((FLASH_MAX_APP_PARTITION_SIZE >  \
                                           FLASH_AREA_SCRATCH_SIZE) ?      \
                                           FLASH_MAX_APP_PARTITION_SIZE :  \
                                           FLASH_AREA_SCRATCH_SIZE) :      \
                                         ((FLASH_MAX_DATA_PARTITION_SIZE > \
                                           FLASH_AREA_SCRATCH_SIZE) ?      \
                                           FLASH_MAX_DATA_PARTITION_SIZE : \
                                           FLASH_AREA_SCRATCH_SIZE))
#define FLASH_AREAS_DEVICE_ID           (FLASH_DEVICE_ID - FLASH_DEVICE_ID)

/* Secure data image primary slot */
#if defined (FLASH_AREA_4_ID)
#define FLASH_AREA_4_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_4_OFFSET             (FLASH_AREA_BEGIN_OFFSET)
#define FLASH_AREA_4_SIZE               (FLASH_S_DATA_PARTITION_SIZE)


/* Control Secure data image primary slot */
#if (FLASH_AREA_4_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_4_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_4_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_4_ID */
#define FLASH_AREA_4_OFFSET             (0x0)
#define FLASH_AREA_4_SIZE               (0x0)
#endif /* FLASH_AREA_4_ID */

/* Secure app image primary slot */
#if defined(FLASH_AREA_0_ID)
#define FLASH_AREA_0_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_0_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE)
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
#define FLASH_AREA_0_SIZE               (FLASH_S_PARTITION_SIZE)
#else
#define FLASH_AREA_0_SIZE               (FLASH_PARTITION_SIZE)
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */
/* Control Secure app image primary slot */
#if (FLASH_AREA_0_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_0_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_0_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_0_ID */
#define FLASH_AREA_0_OFFSET             (0x0)
#define FLASH_AREA_0_SIZE               (0x0)
#endif /* FLASH_AREA_0_ID */

/* Non-secure app image primary slot */
#if defined(FLASH_AREA_1_ID)
#define FLASH_AREA_1_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_1_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE + \
                                         FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE               (FLASH_NS_PARTITION_SIZE)
/* Control Non-secure app image primary slot */
#if (FLASH_AREA_1_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_1_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_1_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0  */
#else /* FLASH_AREA_1_ID */
#define FLASH_AREA_1_OFFSET             (0x0)
#define FLASH_AREA_1_SIZE               (0x0)
#endif /* FLASH_AREA_1_ID */

/* Non-secure data image primary slot */
#if defined(FLASH_AREA_5_ID)
#define FLASH_AREA_5_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_5_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE + \
                                         FLASH_AREA_0_SIZE + FLASH_AREA_1_SIZE)
#define FLASH_AREA_5_SIZE               (FLASH_NS_DATA_PARTITION_SIZE)
/* Control Non-secure data image primary slot */
#if (FLASH_AREA_5_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_5_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_5_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0  */
#else /* FLASH_AREA_5_ID */
#define FLASH_AREA_5_OFFSET             (0x0)
#define FLASH_AREA_5_SIZE               (0x0)
#endif /* FLASH_AREA_5_ID */

/* Secure app image secondary slot */
#if defined(FLASH_AREA_2_ID)
#define FLASH_AREA_2_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE + \
                                         FLASH_AREA_0_SIZE + FLASH_AREA_1_SIZE + \
                                         FLASH_AREA_5_SIZE)
#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
#define FLASH_AREA_2_SIZE               (FLASH_S_PARTITION_SIZE)
#else
#define FLASH_AREA_2_SIZE               (FLASH_PARTITION_SIZE)
#endif /* (MCUBOOT_APP_IMAGE_NUMBER == 2) */
/* Control Secure app image secondary slot */
#if (FLASH_AREA_2_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_2_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*   (FLASH_AREA_2_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_2_ID */
#define FLASH_AREA_2_OFFSET             (0x0)
#define FLASH_AREA_2_SIZE               (0x0)
#endif /* FLASH_AREA_2_ID */

/* Non-secure app image secondary slot */
#if defined(FLASH_AREA_3_ID)
#define FLASH_AREA_3_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_3_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE + \
                                         FLASH_AREA_0_SIZE + FLASH_AREA_1_SIZE + \
                                         FLASH_AREA_5_SIZE + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE               (FLASH_NS_PARTITION_SIZE)
/* Control Non-Secure app image secondary slot */
#if (FLASH_AREA_3_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_3_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_3_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_3_ID */
#define FLASH_AREA_3_OFFSET             (0x0)
#define FLASH_AREA_3_SIZE               (0x0)
#endif /* FLASH_AREA_3_ID */

/* Secure data image secondary slot */
#if defined(FLASH_AREA_6_ID)
#define FLASH_AREA_6_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_6_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE + \
                                         FLASH_AREA_0_SIZE + FLASH_AREA_1_SIZE + \
                                         FLASH_AREA_5_SIZE + FLASH_AREA_2_SIZE + \
                                         FLASH_AREA_3_SIZE)
#define FLASH_AREA_6_SIZE               (FLASH_S_DATA_PARTITION_SIZE)
/* Control Secure data image secondary slot */
#if (FLASH_AREA_6_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_6_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_6_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_6_ID */
#define FLASH_AREA_6_OFFSET             (0x0)
#define FLASH_AREA_6_SIZE               (0x0)
#endif /* FLASH_AREA_6_ID */

/* Non-Secure data image secondary slot */
#if defined(FLASH_AREA_7_ID)
#define FLASH_AREA_7_DEVICE_ID          (FLASH_AREAS_DEVICE_ID)
#define FLASH_AREA_7_OFFSET             (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE + \
                                         FLASH_AREA_0_SIZE + FLASH_AREA_1_SIZE + \
                                         FLASH_AREA_5_SIZE + FLASH_AREA_2_SIZE + \
                                         FLASH_AREA_3_SIZE + FLASH_AREA_6_SIZE)
#define FLASH_AREA_7_SIZE               (FLASH_NS_DATA_PARTITION_SIZE)
/* Control Non-Secure data image secondary slot */
#if (FLASH_AREA_7_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_7_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_7_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
#else /* FLASH_AREA_7_ID */
#define FLASH_AREA_7_OFFSET             (0x0)
#define FLASH_AREA_7_SIZE               (0x0)
#endif /* FLASH_AREA_7_ID */

/* flash areas end offset */
#define FLASH_AREA_END_OFFSET           (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_4_SIZE + \
                                         FLASH_AREA_0_SIZE + FLASH_AREA_1_SIZE + \
                                         FLASH_AREA_5_SIZE + FLASH_AREA_2_SIZE + \
                                         FLASH_AREA_3_SIZE + FLASH_AREA_6_SIZE + \
                                         FLASH_AREA_7_SIZE)
/* Control flash area end */
#if (FLASH_AREA_END_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_END_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_END_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/*
 * The maximum number of status entries supported by the bootloader.
 */
#define MCUBOOT_STATUS_MAX_ENTRIES      ((FLASH_MAX_PARTITION_SIZE) / \
                                         FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS         ((FLASH_MAX_PARTITION_SIZE) / \
                                         FLASH_AREA_IMAGE_SECTOR_SIZE)

#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE


/* Flash device name used by BL2 and NV Counter
 * Name is defined in flash driver file: low_level_flash.c
 */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2)
/* Flash Driver Used to Confirm Secure App Image */
#define  FLASH_PRIMARY_SECURE_DEV_NAME             Driver_FLASH0
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_APP_IMAGE_NUMBER == 2) */
/* Flash Driver Used to Confirm NonSecure App Image or MCUBOOT_APP_IMAGE_NUMBER = 1 */
#define  FLASH_PRIMARY_NONSECURE_DEV_NAME          Driver_FLASH0
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
/* Flash Driver Used to Confirm Secure Data Image */
#define  FLASH_PRIMARY_DATA_SECURE_DEV_NAME        Driver_FLASH0
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#if !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_NS_DATA_IMAGE_NUMBER == 1)
/* Flash Driver Used to Confirm NonSecure Data Image */
#define  FLASH_PRIMARY_DATA_NONSECURE_DEV_NAME     Driver_FLASH0
#endif /* !defined(MCUBOOT_OVERWRITE_ONLY) && (MCUBOOT_S_DATA_IMAGE_NUMBER == 1) */
#define FLASH_DEV_NAME                             Driver_FLASH0

#if (MCUBOOT_APP_IMAGE_NUMBER == 1) && (FLASH_NS_PARTITION_SIZE > 0)
#error "Config not supported"
#endif /* (MCUBOOT_APP_IMAGE_NUMBER=1) && (FLASH_NS_PARTITION_SIZE>0) */

/* BL2 NV Counters definitions  */
#define BL2_NV_COUNTERS_AREA_ADDR       FLASH_BL2_NVCNT_AREA_OFFSET
#define BL2_NV_COUNTERS_AREA_SIZE       FLASH_BL2_NVCNT_AREA_SIZE


/* Systeme Flash description */
/* MPU config */
#define RSS_LIB_BASE                    (0x0FF83C00U)
#define RSS_LIB_SIZE                    (0x1000U)

/* SAU config */
#define STM32_DESCRIPTOR_SIZE           (0x100U)
#define STM32_DESCRIPTOR_BASE_NS        0x0BF99000UL
#define STM32_DESCRIPTOR_END_NS         (0x0BF99000UL + STM32_DESCRIPTOR_SIZE -1)

#endif /* __FLASH_LAYOUT_H__ */
