/**
  ******************************************************************************
  * @file    appli_flash_layout.h
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

#ifndef __APPLI_FLASH_LAYOUT_H__
#define __APPLI_FLASH_LAYOUT_H__

/********** Application Flash layout configuration : begin **********/

#define NSC_CODE_SIZE                  0x400

#define PARTITION_RESERVED             0x1C00

/********** Application Flash layout configuration : end **********/


/********** All define are updated automatically from ROT_BOOT project : begin **********/

#define MCUBOOT_OVERWRITE_ONLY                  /* Defined: the FW installation uses overwrite method.
                                                   UnDefined: The FW installation uses swap mode. */

#define MCUBOOT_APP_IMAGE_NUMBER       0x1      /* 1: S and NS application binaries are assembled in one single image.
                                                   2: Two separated images for S and NS application binaries. */

#define MCUBOOT_S_DATA_IMAGE_NUMBER    0x0      /* 1: S data image for S application.
                                                   0: No S data image. */

#define MCUBOOT_NS_DATA_IMAGE_NUMBER   0x0      /* 1: NS data image for NS application.
                                                   0: No NS data image. */

#define FLASH_AREA_0_OFFSET            0x16000  /* Secure app image primary slot offset */

#define FLASH_AREA_0_SIZE              0x9000   /* Secure app image primary slot size */

#define FLASH_AREA_1_OFFSET            0x0  /* Non-secure app image primary slot offset */

#define FLASH_AREA_1_SIZE              0x0  /* Non-secure app image primary slot size */

#define FLASH_AREA_2_OFFSET            0x1F000  /* Secure app image secondary slot */

#define FLASH_AREA_2_SIZE              0x9000   /* Secure app image secondary slot size */

#define FLASH_AREA_3_OFFSET            0x0  /* Non-secure app image secondary slot */

#define FLASH_AREA_3_SIZE              0x0   /* Non-secure app image secondary slot size */

#define FLASH_AREA_4_OFFSET            0x0      /* Secure data image primary slot offset */

#define FLASH_AREA_4_SIZE              0x0      /* Secure data image primary slot size */

#define FLASH_AREA_5_OFFSET            0x0      /* Non-secure data image primary slot offset */

#define FLASH_AREA_5_SIZE              0x0      /* Non-secure data image primary slot size */

#define FLASH_AREA_6_OFFSET            0x0      /* Non-secure data image primary slot offset */

#define FLASH_AREA_6_SIZE              0x0      /* Non-secure data image primary slot size */

#define FLASH_AREA_7_OFFSET            0x0      /* Non-secure data image primary slot offset */

#define FLASH_AREA_7_SIZE              0x0      /* Non-secure data image primary slot size */

#define FLASH_PARTITION_SIZE           0x9000  /* Secure and Non Secure partition size */

#define FLASH_NS_PARTITION_SIZE        0x0  /* Non Secure partition size */

#define FLASH_S_PARTITION_SIZE         0x9000   /* secure partition size */

#define FLASH_S_DATA_PARTITION_SIZE    0x0      /* secure data partition size */

#define FLASH_NS_DATA_PARTITION_SIZE   0x0      /* non secure data partition size */

#define FLASH_B_SIZE                   0x20000  /* flash bank size: 128 KBytes*/

/********** All define are updated automatically from ROT_BOOT project : End **********/


#define FLASH_TOTAL_SIZE                          (FLASH_B_SIZE+FLASH_B_SIZE) /* total flash size: 256 KBytes */

#define S_IMAGE_PRIMARY_PARTITION_OFFSET           FLASH_AREA_0_OFFSET
#define S_IMAGE_SECONDARY_PARTITION_OFFSET         FLASH_AREA_2_OFFSET
#if (MCUBOOT_S_DATA_IMAGE_NUMBER == 1)
#define S_DATA_IMAGE_PRIMARY_PARTITION_OFFSET      FLASH_AREA_4_OFFSET
#define S_DATA_IMAGE_SECONDARY_PARTITION_OFFSET    FLASH_AREA_6_OFFSET
#endif /* MCUBOOT_S_DATA_IMAGE_NUMBER == 1 */

/*
 * Flash device name
 */
#define FLASH_DEV_NAME                             Driver_FLASH0

#define FLASH_PRIMARY_DEV_NAME                     FLASH_DEV_NAME
#define FLASH_SECONDARY_DEV_NAME                   FLASH_DEV_NAME

#endif /* __APPLI_FLASH_LAYOUT_H__ */
