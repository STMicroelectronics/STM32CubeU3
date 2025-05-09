/**
  ******************************************************************************
  * @file    i3c_interface.h
  * @author  MCD Application Team
  * @brief   Header for i3c_interface.c module
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
#ifndef I3C_INTERFACE_H
#define I3C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "common_interface.h"
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_I3C_Configuration(void);
void OPENBL_I3C_DeInit(void);
uint32_t OPENBL_I3C_IsActive(void);

uint8_t OPENBL_I3C_ProtocolDetection(void);
uint8_t OPENBL_I3C_GetCommandOpcode(void);
uint8_t OPENBL_I3C_ReadByte(void);
void OPENBL_I3C_SendByte(uint8_t byte);
void OPENBL_I3C_SendAcknowledgeByte(uint8_t acknowledge);
void OPENBL_I3C_SendBytes(uint8_t *p_buffer, uint32_t buffer_size);
void OPENBL_I3C_ReadBytes(uint8_t *p_buffer, uint32_t buffer_size);

void OPENBL_I3C_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *p_special_cmd);

void OPENBL_I3C_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* I3C_INTERFACE_H */
