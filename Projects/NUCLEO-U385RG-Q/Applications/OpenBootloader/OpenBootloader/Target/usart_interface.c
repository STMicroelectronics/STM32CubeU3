/**
  ******************************************************************************
  * @file    usart_interface.c
  * @author  MCD Application Team
  * @brief   Contains USART HW configuration
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
#include "main.h"
#include "platform.h"
#include "interfaces_conf.h"

#include "openbl_core.h"
#include "openbl_usart_cmd.h"

#include "app_openbootloader.h"
#include "usart_interface.h"
#include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEFAULT_USART_BAUDRATE   115200U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t UsartDetected = 0U;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USART_Init(void);

/* Private functions ---------------------------------------------------------*/

static void OPENBL_USART_Init(void)
{
  LL_USART_InitTypeDef usart_init_struct;

  /* Configure the USARTx IP:
     - Data width : 9 bits.
     - Stop bit   : 1 bit.
     - Parity     : Even.
  */
  usart_init_struct.PrescalerValue      = LL_USART_PRESCALER_DIV1;
  usart_init_struct.BaudRate            = DEFAULT_USART_BAUDRATE;
  usart_init_struct.DataWidth           = LL_USART_DATAWIDTH_9B;
  usart_init_struct.StopBits            = LL_USART_STOPBITS_1;
  usart_init_struct.Parity              = LL_USART_PARITY_EVEN;
  usart_init_struct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  usart_init_struct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
  usart_init_struct.OverSampling        = LL_USART_OVERSAMPLING_16;

  if (IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE(USARTx))
  {
    LL_USART_EnableAutoBaudRate(USARTx);
    LL_USART_SetAutoBaudRateMode(USARTx, LL_USART_AUTOBAUD_DETECT_ON_7F_FRAME);
  }
  else
  {
    LL_USART_DisableAutoBaudRate(USARTx);
  }

  LL_USART_Init(USARTx, &usart_init_struct);
  LL_USART_Enable(USARTx);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure USART pins and then initialize the used USART instance.
  * @retval None.
  */
void OPENBL_USART_Configuration(void)
{
  GPIO_InitTypeDef gpio_init_struct = {0U};

  /* Enable all resources clocks --------------------------------------------*/

  /* Enable used GPIOx clocks */
  USARTx_GPIO_CLK_TX_ENABLE();
  USARTx_GPIO_CLK_RX_ENABLE();

  /* Enable USART clock */
  USARTx_CLK_ENABLE();

  /* USARTx pins configuration -----------------------------------------------*/
  /*      +--------------+
          |     USART1   |
  +-------+--------------+
  |  Tx   |     PA9      |
  +-------+------------ -+
  |  Rx   |     PA10     |
  +-------+--------------+ */

  /* Common configuration for USARTx PINs:
      Mode        : Alternate function.
      Output type : Push-Pull (Pull-Up).
      Speed       : High speed.
  */
  gpio_init_struct.Pin       = USARTx_TX_PIN;
  gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
  gpio_init_struct.Pull      = GPIO_PULLUP;
  gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_struct.Alternate = USARTx_ALTERNATE;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &gpio_init_struct);

  gpio_init_struct.Pin = USARTx_RX_PIN;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &gpio_init_struct);

  OPENBL_USART_Init();
}

/**
  * @brief  This function is used to De-initialize the USART pins and instance.
  * @retval None.
  */
void OPENBL_USART_DeInit(void)
{
  /* Only de-initialize the USART if it is not the current detected interface */
  if (UsartDetected == 0U)
  {
    LL_USART_Disable(USARTx);

    USARTx_CLK_DISABLE();
  }
}

/**
  * @brief  This function is used to detect if there is any activity on USART protocol.
  * @retval Returns 1 if usart is detected otherwise 0.
  */
uint8_t OPENBL_USART_ProtocolDetection(void)
{
  /* Check if the USARTx is addressed */
  if (((USARTx->ISR & LL_USART_ISR_ABRF) != 0U) && ((USARTx->ISR & LL_USART_ISR_ABRE) == 0U))
  {
    /* Read byte in order to flush the 0x7F synchronization byte */
    OPENBL_USART_ReadByte();

    /* Acknowledge the host */
    OPENBL_USART_SendByte(ACK_BYTE);

    UsartDetected = 1U;
  }
  else
  {
    UsartDetected = 0U;
  }

  return UsartDetected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @retval Returns the command.
  */
uint8_t OPENBL_USART_GetCommandOpcode(void)
{
  uint8_t command_opc;

  /* Get the command opcode */
  command_opc = OPENBL_USART_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_USART_ReadByte()) != 0xFFU)
  {
    command_opc = ERROR_COMMAND;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from USART pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_USART_ReadByte(void)
{
  while (!LL_USART_IsActiveFlag_RXNE_RXFNE(USARTx))
  {
    OPENBL_IWDG_Refresh();
  }

  return LL_USART_ReceiveData8(USARTx);
}

/**
  * @brief  This function is used to send one byte through USART pipe.
  * @param  byte The byte to be sent.
  * @retval None.
  */
void OPENBL_USART_SendByte(uint8_t byte)
{
  LL_USART_TransmitData8(USARTx, (byte & 0xFFU));

  while (!LL_USART_IsActiveFlag_TC(USARTx))
  {
    /* Nothing to do */
  }
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  p_special_cmd Pointer to the @arg OPENBL_SpecialCmdTypeDef structure.
  * @retval None.
  */
void OPENBL_USART_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *p_special_cmd)
{
  switch (p_special_cmd->OpCode)
  {
    /* Unknown command opcode */
    default:
      if (p_special_cmd->CmdType == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_USART_SendByte(0x00U);
        OPENBL_USART_SendByte(0x00U);

        /* Send NULL status size */
        OPENBL_USART_SendByte(0x00U);
        OPENBL_USART_SendByte(0x00U);
      }
      break;
  }
}
