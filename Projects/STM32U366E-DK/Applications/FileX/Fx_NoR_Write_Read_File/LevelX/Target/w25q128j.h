/**
  ******************************************************************************
  * @file    w25q128j.h
  * @brief   This file contains all the description of the
  *          W25Q128J SPI flash memory.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef W25Q128J_H
#define W25Q128J_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "w25q128j_conf.h"

/* Private defines -----------------------------------------------------------*/
#define W25Q128J_SPI_POLL_TIMEOUT    1000U           /*!< Default SPI timeout (1s) */

/* Chip select defining  */
#define W25Q128J_CS_PIN_SET       GPIO_PIN_SET   /*!< Select the W25Q128J memory   */
#define W25Q128J_CS_PIN_RESET     GPIO_PIN_RESET /*!< Deselect the W25Q128J memory */

/* Memory address masks */
#define W25Q128J_MEM_ADDR_MASK1         0x00FF0000U  /*!< Memory address mask 1 */
#define W25Q128J_MEM_ADDR_MASK2         0x0000FF00U  /*!< Memory address mask 2 */
#define W25Q128J_MEM_ADDR_MASK3         0x000000FFU  /*!< Memory address mask 3 */

/**
  * @brief  W25Q128J Configuration
  */

#define W25Q128J_FLASH_SIZE                  0x1000000U /* 128 MBits => 16MBytes */
#define W25Q128J_BLOCK_SIZE                  0x10000U   /* 256 blocks of 64KBytes */
#define W25Q128J_SUBBLOCK_SIZE               0x8000U    /* 512 blocks of 32 KBytes */
#define W25Q128J_SECTOR_SIZE                 0x1000U    /* 4096 sectors of 4kBytes */
#define W25Q128J_PAGE_SIZE                   0x100U     /* 65536 pages of 256 bytes */

#define W25Q128J_DUMMY_BYTE                  0x00U      /*!< Dummy byte value */

#define W25Q128J_NUM_OF_BLOCK                (W25Q128J_FLASH_SIZE / W25Q128J_BLOCK_SIZE)

#define W25Q128J_NUM_PAGE_PER_BLOCK          (W25Q128J_BLOCK_SIZE / W25Q128J_PAGE_SIZE)

/*!< Total number of pages => 65536         */
#define W25Q128J_PAGE_COUNT                 (W25Q128J_NUM_PAGE_PER_BLOCK * W25Q128J_NUM_OF_BLOCK)


#define W25Q128J_CHIP_ERASE_MAX_TIME         200000      /*!< Chip erase maximum time  */
#define W25Q128J_BLOCK_ERASE_MAX_TIME        2000        /*!< Block erase maximum time */
#define W25Q128J_SUBBLOCK_ERASE_MAX_TIME     1600        /*!< Sub-block erase maximum time */
#define W25Q128J_SECTOR_ERASE_MAX_TIME       400         /*!< Sector erase maximum time */


/********************************************************************
  * @brief  W25Q128J Commands
  *******************************************************************/

/* Reset Operations */
#define W25Q128J_RESET_ENABLE_CMD                     0x66U          /*!< Reset enable                                 */
#define W25Q128J_RESET_MEMORY_CMD                     0x99U          /*!< Reset memory                                 */

/* Identification Operations */
#define W25Q128J_READ_ID_CMD                          0x90U          /*!< Read ID                                      */
#define W25Q128J_READ_ID_DUAL_CMD                     0x92U          /*!< Read ID dual                                 */
#define W25Q128J_READ_ID_QUAD_CMD                     0x94U          /*!< Read ID quad                                 */

#define W25Q128J_READ_JEDEC_ID_CMD                    0x9FU          /*!< Read JDEC ID                                 */
#define W25Q128J_READ_UNIQUE_ID_CMD                   0x4BU          /*!< Read UNIQUE ID                               */

/* Read Operations */
#define W25Q128J_READ_DATA_CMD                        0x03U          /*!< Read data                                    */
#define W25Q128J_FAST_READ_CMD                        0x0BU          /*!< Fast read                                    */

#define W25Q128J_FAST_READ_DUAL_OUT_CMD               0x3BU          /*!< Fast read dual output                        */
#define W25Q128J_FAST_READ_DUAL_INOUT_CMD             0xBBU          /*!< Fast read dual I/O                           */

#define W25Q128J_FAST_READ_QUAD_OUT_CMD               0x6BU          /*!< Fast read quad output                        */
#define W25Q128J_FAST_READ_QUAD_INOUT_CMD             0xEBU          /*!< Fast read quad I/O                           */

/* Write Operations */
#define W25Q128J_WRITE_ENABLE_CMD                     0x06U          /*!< Write enable                                 */
#define W25Q128J_VOL_SR_WRITE_ENABLE_CMD              0x50U          /*!< Volatile status register write enable        */
#define W25Q128J_WRITE_DISABLE_CMD                    0x04U

/* Register Operations */
#define W25Q128J_READ_STATUS_REG1_CMD                 0x05U          /*!< Read status register 1                       */
#define W25Q128J_READ_STATUS_REG2_CMD                 0x35U          /*!< Read status register 2                       */
#define W25Q128J_READ_STATUS_REG3_CMD                 0x15U          /*!< Read status register 3                       */
#define W25Q128J_WRITE_STATUS_REG1_CMD                0x01U          /*!< Write status register 1                      */
#define W25Q128J_WRITE_STATUS_REG2_CMD                0x31U          /*!< Write status register 2                      */
#define W25Q128J_WRITE_STATUS_REG3_CMD                0x11U          /*!< Write status register 3                      */

#define W25Q128J_READ_SFDP_REG_CMD                    0x5AU          /*!< Read SFDP register                           */
#define W25Q128J_READ_BLOCK_SECTOR_LOCK               0x3DU          /*!< Read block sector lock                       */

#define W25Q128J_ERASE_SECURITY_REG_CMD               0x44U          /*!< Erase security register                      */
#define W25Q128J_PROG_SECURITY_REG_CMD                0x42U          /*!< Program security register                    */
#define W25Q128J_READ_SECURITY_REG_CMD                0x48U          /*!< Read security register                       */

/* Program Operations */
#define W25Q128J_PAGE_PROG_CMD                        0x02U          /*!< Page program                                 */
#define W25Q128J_QUAD_PAGE_PROG_CMD                   0x32U          /*!< Quad page program                            */

/* Erase Operations */
#define W25Q128J_SECTOR_ERASE_CMD                     0x20U          /*!< Sector erase                                 */
#define W25Q128J_BLOCK_ERASE_32K_CMD                  0x52U          /*!< Bock erase 32K                               */
#define W25Q128J_BLOCK_ERASE_64K_CMD                  0xD8U          /*!< Bock erase 64K                               */
#define W25Q128J_CHIP_ERASE_CMD                       0xC7U          /*!< Chip erase                                   */
#define W25Q128J_CHIP_ERASE_CMD_1                     0x60U          /*!< Chip erase                                   */

#define W25Q128J_ERASE_PROG_SUSPEND_CMD               0x75U          /*!< Erase program suspend                        */
#define W25Q128J_ERASE_PROG_RESUME_CMD                0x7AU          /*!< Erase program resume                         */

/* Power-down operations */
#define W25Q128J_POWER_DOWN_CMD                       0xB9U          /*!< Power down                                   */
#define W25Q128J_RELEASE_POWER_DOWN_CMD               0xABU          /*!< Release power down                           */

/* Other operations */
#define W25Q128J_SET_BURST_WRAP_CMD                   0x77U          /*!< Set burst wrap                               */
#define W25Q128J_INDIV_BLOCK_SECTOR_LOCK              0x36U          /*!< Individual block sector lock                 */
#define W25Q128J_INDIV_BLOCK_SECTOR_UNLOCK            0x39U          /*!< Individual block sector unlock               */
#define W25Q128J_GLOBAL_BLOCK_SECTOR_LOCK             0x7EU          /*!< Global block sector lock                     */
#define W25Q128J_GLOBAL_BLOCK_SECTOR_UNLOCK           0x98U          /*!< Global block sector unlock                   */


#define W25Q128J_READ_ID_CMD_SIZE                     4U             /*!< Read ID command size                         */
#define W25Q128J_ID_SIZE                              2U             /*!< ID size                                      */
#define W25Q128J_READ_JDEC_ID_CMD_SIZE                1U             /*!< Read JDEC ID command size                    */
#define W25Q128J_JDEC_ID_SIZE                         4U             /*!< JDEC ID size                                 */
#define W25Q128J_UNIQUE_ID_CMD_SIZE                   1U             /*!< Unique ID command size                       */
#define W25Q128J_UNIQUE_ID_SIZE                       8U             /*!< Unique ID size                               */
#define W25Q128J_RESET_ENABLE_CMD_SIZE                1U             /*!< Reset enable command size                    */
#define W25Q128J_RESET_CMD_SIZE                       1U             /*!< Reset command size                           */
#define W25Q128J_SUSPEND_CMD_SIZE                     1U             /*!< Suspend command size                           */
#define W25Q128J_RESUME_CMD_SIZE                      1U             /*!< Resume command size                           */
#define W25Q128J_WRITE_ENABLE_CMD_SIZE                1U             /*!< Write enable command size                    */
#define W25Q128J_WRITE_DISABLE_CMD_SIZE               1U             /*!< Write disable command size                   */
#define W25Q128J_READ_STATUS_REG_CMD_SIZE             1U             /*!< Read status register command size            */
#define W25Q128J_STATUS_REG_DATA_SIZE                 1U             /*!< Status register data size                    */
#define W25Q128J_WRITE_STATUS_REG_CMD_SIZE            2U             /*!< Write status register command size           */
#define W25Q128J_BLOCK_ERASE_CMD_SIZE                 4U             /*!< Block erase command size                     */
#define W25Q128J_CHIP_ERASE_CMD_SIZE                  1U             /*!< Chip erase command size                      */
#define W25Q128J_READ_DATA_CMD_SIZE                   4U             /*!< Read data command size                       */
#define W25Q128J_FAST_DATA_CMD_SIZE                   5U             /*!< Fast read command size                       */
#define W25Q128J_PAGE_PROGRAM_CMD_SIZE                4U             /*!< Page program command size                    */
#define W25Q128J_ENTER_POWER_DOWN_CMD_SIZE            1U             /*!< Enter power down command size                */
#define W25Q128J_EXIT_POWER_DOWN_CMD_SIZE             1U             /*!< Exit power down command size                 */


/*********************************************************************************
  * @brief  W25Q128J Registers
  ********************************************************************************/

/* Status Register 1 */
#define W25Q128J_SR1_BUSY                               0x01U               /*!< Erase/Write in progress */
#define W25Q128J_SR1_WEL                                0x02U               /*!< Write enable latch */
#define W25Q128J_SR1_BP                                 0x1CU               /*!< Block protected bits */
#define W25Q128J_SR1_TB                                 0x20U               /*!< Top/Bottom protect */
#define W25Q128J_SR1_SEC                                0x40U               /*!< Sector protect */
#define W25Q128J_SR1_PROTECT_ALL                        W25Q128J_SR1_BP     /*!< Enable protection */
#define W25Q128J_SR1_WRITE_PROTECTION_DISABLE           0x00U               /*!< Disable protection */

/* Status Register 2 */
#define W25Q128J_SR2_QE_DIS                  0x00U    /*!< Quad Disable */
#define W25Q128J_SR2_SRL                     0x01U    /*!< Status Register Lock */
#define W25Q128J_SR2_QE                      0x02U    /*!< Quad Enable */
#define W25Q128J_SR2_LB                      0x3CU    /*!< Security Register Lock bits */
#define W25Q128J_SR2_CMP                     0x40U    /*!< Complement protect */
#define W25Q128J_SR2_SUS                     0x80U    /*!< Suspend Status */

/* Status Register 3 */
#define W25Q128J_SR3_WPS                     0x04U    /*!< Write Protect Selection */
#define W25Q128J_SR3_DRV                     0x60U    /*!< Output Driver Strength */


/**
  * @brief Enumeration defining the software error code.
  */
typedef enum
{
  W25Q128J_OK = 0,            /*!< Success Status                                   */
  W25Q128J_ERROR,             /*!< Error Status                                     */
  W25Q128J_SUSPENDED,         /*!< Program or Erase operation suspended             */
  W25Q128J_BUSY               /*!< Memory Busy write or erase operation in progress */
} w25q128j_status_t;

/**
  * @brief Enumeration defining the W25Q128J init state.
   */
typedef enum
{
  W25Q128J_INIT_NO = 0,                   /*!<  Instance is not initialized                */
  W25Q128J_INIT_OK                        /*!<  Instance is initialized                    */
} w25q128j_init_state_t;

/**
  * @brief Enumeration defining the memory erase size.
  */
typedef enum
{
  W25Q128J_ERASE_SECTOR = 0,              /*!< 4K size Sector erase                          */
  W25Q128J_ERASE_32K_BLOCK,               /*!< 32K size Block erase                          */
  W25Q128J_ERASE_64K_BLOCK                /*!< 64K size Block erase                          */
} w25q128j_erase_t;

/**
  * @brief Structure defining the Flash Memory information.
  */
typedef struct
{
  uint32_t flash_size;                       /*!< Size of the flash                                   */
  uint32_t erase_block_64k_size;             /*!< Size of blocks (64k) for the erase operation        */
  uint32_t erase_block_64k_number;           /*!< Number of blocks (64k) for the erase operation      */
  uint32_t erase_block_32k_size;             /*!< Size of blocks (32k) for the erase operation        */
  uint32_t erase_block_32k_number;           /*!< Number of blocks (32k) for the erase operation      */
  uint32_t erase_sector_size;                /*!< Size of sector for the erase operation              */
  uint32_t erase_sector_number;              /*!< Number of sectors for the erase operation           */
  uint32_t prog_page_size;                   /*!< Size of pages for the program operation             */
  uint32_t prog_pages_number;                /*!< Number of pages for the program operation           */
} w25q128j_info_t;

/* Exported functions prototypes ---------------------------------------------*/

/* Memory initialization and configuration */
w25q128j_status_t w25q128j_Init(SPI_HandleTypeDef *phspi);
w25q128j_status_t w25q128j_DeInit(SPI_HandleTypeDef *phspi);
w25q128j_status_t w25q128j_Reset(SPI_HandleTypeDef *phspi);
w25q128j_status_t w25q128j_Suspend(SPI_HandleTypeDef *phspi);
w25q128j_status_t w25q128j_Resume(SPI_HandleTypeDef *phspi);

/* Memory information ------------------*/
w25q128j_status_t w25q128j_GetInfo(const SPI_HandleTypeDef *phspi, w25q128j_info_t *p_info);
w25q128j_status_t w25q128j_GetStatus(SPI_HandleTypeDef *phspi, w25q128j_status_t *p_status);
w25q128j_status_t w25q128j_ReadId(SPI_HandleTypeDef *phspi, uint8_t *p_id);
w25q128j_status_t w25q128j_ReadJedecId(SPI_HandleTypeDef *phspi, uint8_t *p_id);
w25q128j_status_t w25q128j_ReadUniqueId(SPI_HandleTypeDef *phspi, uint8_t *p_id);

/* Read/Write status registers --------------------*/
w25q128j_status_t w25q128j_ReadStatusRegister1(SPI_HandleTypeDef *phspi, uint8_t *p_value);
w25q128j_status_t w25q128j_ReadStatusRegister2(SPI_HandleTypeDef *phspi, uint8_t *p_value);
w25q128j_status_t w25q128j_ReadStatusRegister3(SPI_HandleTypeDef *phspi, uint8_t *p_value);

w25q128j_status_t w25q128j_WriteStatusRegister1(SPI_HandleTypeDef *phspi, uint8_t value);
w25q128j_status_t w25q128j_WriteStatusRegister2(SPI_HandleTypeDef *phspi, uint8_t value);
w25q128j_status_t w25q128j_WriteStatusRegister3(SPI_HandleTypeDef *phspi, uint8_t value);

/* Write enable/disable functions -----------------*/
w25q128j_status_t w25q128j_WriteEnable(SPI_HandleTypeDef *phspi);
w25q128j_status_t w25q128j_WriteDisable(SPI_HandleTypeDef *phspi);

/* Read/Write Commands -----------------------*/
w25q128j_status_t w25q128j_Read(SPI_HandleTypeDef *phspi, uint8_t *p_data,
                                uint32_t read_addr, uint32_t size_byte);
w25q128j_status_t w25q128j_FastRead(SPI_HandleTypeDef *phspi, uint8_t *p_data,
                                uint32_t read_addr, uint32_t size_byte);
w25q128j_status_t w25q128j_Write(SPI_HandleTypeDef *phspi, const uint8_t *p_data,
                                 uint32_t write_addr, uint32_t size_byte);

/* Memory erase functions -------------------------*/
w25q128j_status_t w25q128j_Erase(SPI_HandleTypeDef *phspi, uint32_t addr, w25q128j_erase_t size);
w25q128j_status_t w25q128j_EraseSectors(SPI_HandleTypeDef *phspi, uint32_t erase_addr, uint32_t size_byte);
w25q128j_status_t w25q128j_ChipErase(SPI_HandleTypeDef *phspi);

/* Power down functions -------------------------*/
w25q128j_status_t w25q128j_EnterPowerDown(SPI_HandleTypeDef *phspi);
w25q128j_status_t w25q128j_ExitPowerDown(SPI_HandleTypeDef *phspi);

#ifdef __cplusplus
}
#endif

#endif /* W25Q128J_H */
