/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_BOARD_H__
#define __STM32_BOARD_H__

#define LED1_Pin GPIO_PIN_7
#define LED1_GPIO_Port GPIOH
#define LED1_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED2_Pin  GPIO_PIN_5
#define LED2_GPIO_Port GPIOA
#define LED2_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()


#endif /* __STM32_BOARD_H__ */
