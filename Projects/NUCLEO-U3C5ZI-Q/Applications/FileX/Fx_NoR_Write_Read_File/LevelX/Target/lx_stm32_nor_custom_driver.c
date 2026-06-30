/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#include "lx_stm32_nor_custom_driver.h"

/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#ifndef NOR_SECTOR_BUFFER_SIZE
#define NOR_SECTOR_BUFFER_SIZE 512
#endif

#ifndef LX_DIRECT_READ
static ULONG nor_sector_memory[NOR_SECTOR_BUFFER_SIZE];
#endif
/* USER CODE BEGIN PV */
extern SPI_HandleTypeDef hspi2;
static ULONG nor_flash_timeout = 2000;
static ULONG *nor_flash_read_buffer = nor_sector_memory;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static inline UINT nor_flash_check_status(SPI_HandleTypeDef *phspi, ULONG max_timeout);
/* USER CODE END PFP */

static UINT  lx_nor_driver_read(ULONG *flash_address, ULONG *destination, ULONG words);
static UINT  lx_nor_driver_write(ULONG *flash_address, ULONG *source, ULONG words);

static UINT  lx_nor_driver_block_erase(ULONG block, ULONG erase_count);
static UINT  lx_nor_driver_block_erased_verify(ULONG block);

/* USER CODE BEGIN USER_CODE_SECTION_1 */

/* USER CODE END USER_CODE_SECTION_1 */

UINT  lx_stm32_nor_custom_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;

  ULONG total_blocks = 0;
  ULONG words_per_block = 0;

  /* USER CODE BEGIN Init_Section_0 */
  w25q128j_status_t w25q128j_status = W25Q128J_ERROR;

  /* get the memory part characteristics */
   w25q128j_status = w25q128j_Init(&hspi2);
   if(w25q128j_status != W25Q128J_OK)
   {
    return LX_ERROR;
   }

  total_blocks = (W25Q128J_FLASH_SIZE / W25Q128J_BLOCK_SIZE);
  words_per_block = W25Q128J_BLOCK_SIZE / sizeof(ULONG);
  nor_flash->lx_nor_flash_base_address = (ULONG *)0;
  /* USER CODE END Init_Section_0 */

  nor_flash->lx_nor_flash_total_blocks    = total_blocks;
  nor_flash->lx_nor_flash_words_per_block = words_per_block;

  /* USER CODE BEGIN Init_Section_1 */

  /* USER CODE END Init_Section_1 */

  nor_flash->lx_nor_flash_driver_read = lx_nor_driver_read;
  nor_flash->lx_nor_flash_driver_write = lx_nor_driver_write;

  nor_flash->lx_nor_flash_driver_block_erase = lx_nor_driver_block_erase;
  nor_flash->lx_nor_flash_driver_block_erased_verify = lx_nor_driver_block_erased_verify;

#ifndef LX_DIRECT_READ
    nor_flash->lx_nor_flash_sector_buffer = nor_sector_memory;
#endif

  /* USER CODE BEGIN Init_Section_2 */

  /* USER CODE END Init_Section_2 */

    return ret;
}

/* USER CODE BEGIN USER_CODE_SECTION_2 */
/**
  * @brief Check the status of the memory part
  * @param  phspi : SPI handle pointer
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static inline UINT nor_flash_check_status(SPI_HandleTypeDef *phspi, ULONG max_timeout)
{
  uint32_t start = HAL_GetTick();

  w25q128j_status_t w25q128j_status = W25Q128J_ERROR;
  w25q128j_status_t p_status = W25Q128J_ERROR;

  while (HAL_GetTick() - start < max_timeout)
  {
    /* get current status of the flash memory */
    w25q128j_status = w25q128j_GetStatus(phspi, &p_status);
    if (w25q128j_status == W25Q128J_OK)
    {
      return LX_SUCCESS;
    }
  }
  return LX_ERROR;
}

/* USER CODE END USER_CODE_SECTION_2 */

/**
  * @brief  Read data from NOR flash memory
  * @param  flash_address: Pointer to the source address in NOR flash
  * @param  destination: Pointer to the destination buffer in RAM
  * @param  words: Number of 32-bit words to read
  * @retval LX_SUCCESS if successful, LX_ERROR otherwise
  */
static UINT lx_nor_driver_read(ULONG *flash_address, ULONG *destination, ULONG words)
{
  UINT ret = LX_SUCCESS;

  /* USER CODE BEGIN NOR_READ */

  /* check that the memory part is ready */
  ret = nor_flash_check_status(&hspi2, nor_flash_timeout);
  if (ret != LX_SUCCESS)
  {
    return ret;
  }
  else
  {
    /* perform the read operation */
    ret = w25q128j_Read(&hspi2, (uint8_t *) destination, (uint32_t)flash_address, words * sizeof(ULONG));
    if (ret != W25Q128J_OK)
    {
      return LX_ERROR;
    }
  }
  /* USER CODE END NOR_READ */

  return ret;
}

/**
  * @brief  Write data to NOR flash memory
  * @param  flash_address: Pointer to the destination address in NOR flash
  * @param  source: Pointer to the source buffer in RAM
  * @param  words: Number of 32-bit words to write
  * @retval LX_SUCCESS if successful, LX_ERROR otherwise
  */
static UINT lx_nor_driver_write(ULONG *flash_address, ULONG *source, ULONG words)
{
  UINT ret = LX_SUCCESS;

  /* USER CODE BEGIN NOR_DRIVER_WRITE */

  /* check that the memory part is ready */
  ret = nor_flash_check_status(&hspi2, nor_flash_timeout);

  if (ret != LX_SUCCESS)
  {
    return ret;
  }
  else
  {
    /* perform the write operation */
    ret = w25q128j_Write(&hspi2, (uint8_t *) source,
                         (uint32_t)flash_address, words * sizeof(ULONG));
    if (ret != W25Q128J_OK)
    {
      return LX_ERROR;
    }
  }
  /* USER CODE END  NOR_DRIVER_WRITE */

  return ret;
}

/**
  * @brief  Erase a block in NOR flash memory
  * @param  block: Block number to erase
  * @param  erase_count: Number of times to erase (unused)
  * @retval LX_SUCCESS if successful, LX_ERROR otherwise
  */
static UINT lx_nor_driver_block_erase(ULONG block, ULONG erase_count)
{

  UINT ret = LX_SUCCESS;

  /* USER CODE BEGIN NOR_DRIVER_BLOCK */

  /* check that the memory part is ready */
  ret = nor_flash_check_status(&hspi2, nor_flash_timeout);

  if (ret != LX_SUCCESS)
  {
    return ret;
  }
  else
  {
    /* perform the block erase operation */
    ret = w25q128j_Erase(&hspi2, block, W25Q128J_ERASE_64K_BLOCK);
    if (ret != W25Q128J_OK)
    {
      return LX_ERROR;
    }
  }
  /* USER CODE END  NOR_DRIVER_BLOCK */

  return ret;
}

/**
  * @brief  Verify if a block in NOR flash memory is erased
  * @param  block: Block number to verify
  * @retval LX_SUCCESS if erased, LX_ERROR otherwise
  */
static UINT lx_nor_driver_block_erased_verify(ULONG block)
{
  UINT ret = LX_SUCCESS;

  /* USER CODE BEGIN NOR_DRIVER_VERIFY */
  ULONG start_address, end_address;

  /* check that the memory part is ready */
  ret = nor_flash_check_status(&hspi2, nor_flash_timeout);
  if (ret != LX_SUCCESS)
  {
    return ret;
  }

  /* calculate the start and the end block address */
  start_address = block * W25Q128J_BLOCK_SIZE;
  end_address = start_address + W25Q128J_BLOCK_SIZE;

  while (start_address < end_address)
  {
    ret = w25q128j_Read(&hspi2, (uint8_t *)nor_flash_read_buffer, (uint32_t)start_address, LX_NOR_SECTOR_SIZE * sizeof(ULONG));
    if (ret != W25Q128J_OK)
    {
      return LX_ERROR;
    }

    for (UINT i = 0; i < LX_NOR_SECTOR_SIZE; i++)
    {
      if (nor_flash_read_buffer[i] != 0xFFFFFFFF)
      {
        return LX_ERROR;
      }
    }

    start_address +=  LX_NOR_SECTOR_SIZE * sizeof(ULONG);
  }
  /* USER CODE END  NOR_DRIVER_VERIFY */

  return ret;
}

/* USER CODE BEGIN USER_CODE_SECTION_3 */

/* USER CODE END USER_CODE_SECTION_3 */
