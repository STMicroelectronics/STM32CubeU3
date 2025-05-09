/**
  ******************************************************************************
  * @file    i2c_interface.c
  * @author  MCD Application Team
  * @brief   Contains I2C HW configuration
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
#include "openbl_i2c_cmd.h"

#include "app_openbootloader.h"
#include "i2c_interface.h"
#include "iwdg_interface.h"
#include "flash_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t I2cDetected = 0U;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_I2C_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the used I2C instance.
  * @retval None.
  */
static void OPENBL_I2C_Init(void)
{
  LL_I2C_InitTypeDef i2c_init_struct;

  /* Configure the I2C IP:
      - 7-bit address.
      - Slave mode.
      - Enable analog filter.
  */
  i2c_init_struct.Timing              = I2C_TIMING;
  i2c_init_struct.AnalogFilter        = LL_I2C_ANALOGFILTER_ENABLE;
  i2c_init_struct.DigitalFilter       = 0U;
  i2c_init_struct.OwnAddress1         = I2C_ADDRESS;
  i2c_init_struct.TypeAcknowledge     = LL_I2C_NACK;
  i2c_init_struct.OwnAddrSize         = LL_I2C_OWNADDRESS1_7BIT;

  if (LL_I2C_Init(I2Cx, &i2c_init_struct) != SUCCESS)
  {
    Error_Handler();
  }

  LL_I2C_Enable(I2Cx);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure I2C pins and then initialize the used I2C instance.
  * @retval None.
  */
void OPENBL_I2C_Configuration(void)
{
  GPIO_InitTypeDef gpio_init_struct;

  /* I2Cx pins configuration -------------------------------------------------*/

  /*      +--------------+
          |     I2C1     |
  +-------+--------------+
  |  SCL  |     PB6      |
  +-------+--------------+
  |  SDA  |     PB7      |
  +-------+--------------+ */

  /* Common configuration for I2Cx PINs:
      Mode        : Alternate function.
      Output type : Open drain (Pull-Up).
      Speed       : High speed.
  */

  /* Enable used GPIOx clocks */
  I2Cx_GPIO_CLK_SCL_ENABLE();
  I2Cx_GPIO_CLK_SDA_ENABLE();

  /* Enable I2C clock */
  I2Cx_CLK_ENABLE();

  gpio_init_struct.Mode      = GPIO_MODE_AF_OD;
  gpio_init_struct.Pull      = GPIO_PULLUP;
  gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_struct.Alternate = I2Cx_ALTERNATE;

  /* Configure I2Cx SCL PIN */
  gpio_init_struct.Pin       = I2Cx_SCL_PIN;
  HAL_GPIO_Init(I2Cx_SCL_PIN_PORT, &gpio_init_struct);

  /* Configure I2Cx SDA PIN */
  gpio_init_struct.Pin       = I2Cx_SDA_PIN;
  HAL_GPIO_Init(I2Cx_SDA_PIN_PORT, &gpio_init_struct);

  OPENBL_I2C_Init();
}

/**
  * @brief  This function is used to De-initialize the I2C pins and instance.
  * @retval None.
  */
void OPENBL_I2C_DeInit(void)
{
  /* Only de-initialize the I2C if it is not the current detected interface */
  if (I2cDetected == 0U)
  {
    LL_I2C_Disable(I2Cx);

    I2Cx_CLK_DISABLE();
  }
}

/**
  * @brief  This function is used to detect if there is any activity on I2C protocol.
  * @retval Returns 1 if interface is detected else 0.
  */
uint8_t OPENBL_I2C_ProtocolDetection(void)
{
  /* Check if the I2Cx is addressed */
  if (LL_I2C_IsActiveFlag_ADDR(I2Cx) != 0U)
  {
    I2cDetected = 1U;
  }
  else
  {
    I2cDetected = 0U;
  }

  return I2cDetected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @retval Returns the command.
  */
uint8_t OPENBL_I2C_GetCommandOpcode(void)
{
  uint8_t command_opc;

  /* Wait until the Address Matched flag is set */
  while (LL_I2C_IsActiveFlag_ADDR(I2Cx) == 0U)
  {
    OPENBL_IWDG_Refresh();
  }

  LL_I2C_ClearFlag_ADDR(I2Cx);

  /* Get the command opcode */
  command_opc = OPENBL_I2C_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_I2C_ReadByte()) != 0xFFU)
  {
    command_opc = ERROR_COMMAND;
  }

  OPENBL_I2C_WaitStop();

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from I2C pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_I2C_ReadByte(void)
{
  uint32_t timeout = 0U;

  while (LL_I2C_IsActiveFlag_RXNE(I2Cx) == 0U)
  {
    OPENBL_IWDG_Refresh();

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      NVIC_SystemReset();
    }
  }

  return LL_I2C_ReceiveData8(I2Cx);
}

/**
  * @brief  This function is used to send one byte through I2C pipe.
  * @param  byte The byte to be sent.
  * @retval None.
  */
void OPENBL_I2C_SendByte(uint8_t byte)
{
  uint32_t timeout = 0U;

  while (LL_I2C_IsActiveFlag_TXIS(I2Cx) == 0U)
  {
    OPENBL_IWDG_Refresh();

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      NVIC_SystemReset();
    }
  }

  LL_I2C_TransmitData8(I2Cx, byte);
}

/**
  * @brief  This function is used to wait until the address is matched.
  * @retval None.
  */
void OPENBL_I2C_WaitAddress(void)
{
  uint32_t timeout = 0U;

  while (LL_I2C_IsActiveFlag_ADDR(I2Cx) == 0U)
  {
    OPENBL_IWDG_Refresh();

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      NVIC_SystemReset();
    }
  }

  LL_I2C_ClearFlag_ADDR(I2Cx);
}

/**
  * @brief  This function is used to wait until NACK is detected.
  * @retval None.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_I2C_WaitNack(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_I2C_WaitNack(void)
#endif /* (__ICCARM__) */
{
  uint32_t timeout = 0U;

  /* While the I2C NACK is not detected, the IWDG is refreshed,
     if the timeout is reached a system reset occurs */
  while ((I2Cx->ISR & I2C_ISR_NACKF) == 0U)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      /* System Reset */
      SCB->AIRCR = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)
                              | (SCB->AIRCR | SCB_AIRCR_SYSRESETREQ_Msk));
    }
  }

  /* Clear Not Acknowledge detection flag */
  I2Cx->ICR |= I2C_ICR_NACKCF;
}

/**
  * @brief  This function is used to wait until STOP is detected.
  * @retval None.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_I2C_WaitStop(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_I2C_WaitStop(void)
#endif /* (__ICCARM__) */
{
  uint32_t timeout = 0U;

  /* While the I2C stop is not detected, refresh the IWDG,
     if the timeout is reached a system reset occurs */
  while ((I2Cx->ISR & I2C_ISR_STOPF) == 0U)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      /* System Reset */
      SCB->AIRCR = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)
                              | (SCB->AIRCR | SCB_AIRCR_SYSRESETREQ_Msk));
    }
  }

  /* Clear Stop detection flag */
  I2Cx->ICR |= I2C_ICR_STOPCF;
}

/**
  * @brief  This function is used to send Acknowledgment.
  * @param  acknowledge The acknowledge byte to be sent.
  * @retval None.
  */
void OPENBL_I2C_SendAcknowledgeByte(uint8_t acknowledge)
{
  /* Wait until address is matched */
  OPENBL_I2C_WaitAddress();

  /* Send ACK or NACK byte */
  OPENBL_I2C_SendByte(acknowledge);

  /* Wait until NACK is detected */
  OPENBL_I2C_WaitNack();

  /* Wait until STOP byte is detected */
  OPENBL_I2C_WaitStop();
}

/**
  * @brief  This function is used to send busy byte through I2C pipe.
  * @retval None.
  */
#if defined (__ICCARM__)
__ramfunc void OPENBL_I2C_SendBusyByte(void)
#else
__attribute__((section(".ramfunc"))) void OPENBL_I2C_SendBusyByte(void)
#endif /* (__ICCARM__) */
{
  uint32_t timeout = 0U;

  /* Wait for the received address to match with the device address */
  if (((I2Cx->ISR & I2C_ISR_ADDR) != 0U))
  {
    /* Clear the flag of address match */
    I2Cx->ICR |= I2C_ICR_ADDRCF;

    /* While the transmit data is not empty, refresh the IWDG,
       if the timeout is reached a system reset occurs */
    while ((I2Cx->ISR & I2C_ISR_TXIS) == 0U)
    {
      IWDG->KR = IWDG_KEY_RELOAD;

      timeout++;

      if (timeout >= OPENBL_I2C_TIMEOUT)
      {
        /* System Reset */
        SCB->AIRCR = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)
                                | (SCB->AIRCR | SCB_AIRCR_SYSRESETREQ_Msk));
      }
    }

    /* Send busy byte */
    I2Cx->TXDR = BUSY_BYTE;

    /* Wait until NACK is detected */
    OPENBL_I2C_WaitNack();

    /* Wait until STOP byte is detected */
    OPENBL_I2C_WaitStop();
  }
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  p_special_cmd Pointer to the OPENBL_SpecialCmdTypeDef structure.
  * @retval None.
  */
void OPENBL_I2C_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *p_special_cmd)
{
  switch (p_special_cmd->OpCode)
  {
    default:
      if (p_special_cmd->CmdType == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_I2C_SendByte(0x00U);
        OPENBL_I2C_SendByte(0x00U);

        /* Wait for address to match */
        OPENBL_I2C_WaitAddress();

        /* Send NULL status size */
        OPENBL_I2C_SendByte(0x00U);
        OPENBL_I2C_SendByte(0x00U);

        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
           to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
         */
      }
      break;
  }
}

/**
  * @brief  This function is used to Set Flash busy state variable to activate busy state sending
  *         during flash operations
  * @retval None.
  */
void OPENBL_Enable_BusyState_Sending(void)
{
  /* Enable Flash busy state sending */
  OPENBL_Enable_BusyState_Flag();
}

/**
  * @brief  This function is used to disable the send of busy state in I2C non stretch mode.
  * @retval None.
  */
void OPENBL_Disable_BusyState_Sending(void)
{
  /* Disable Flash busy state sending */
  OPENBL_Disable_BusyState_Flag();
}
