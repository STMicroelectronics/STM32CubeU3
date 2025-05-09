/**
  ******************************************************************************
  * @file    systemmemory_interface.h
  * @author  MCD Application Team
  * @brief   Header for systemmemory_interface.c module
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
#ifndef SYSTEMMEMORY_INTERFACE_H
#define SYSTEMMEMORY_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint8_t OPENBL_ICP_Read(uint32_t address);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SYSTEMMEMORY_INTERFACE_H */
