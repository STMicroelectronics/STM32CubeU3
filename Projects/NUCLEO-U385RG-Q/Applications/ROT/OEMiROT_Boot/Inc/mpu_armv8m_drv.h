/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MPU_ARMV8M_DRV_H__
#define __MPU_ARMV8M_DRV_H__

#include <stdint.h>

#include "cmsis.h"

#define PRIVILEGED_DEFAULT_DISABLE 0
#define HARDFAULT_NMI_ENABLE      1

/* MAIR_ATTR */
#define MPU_ARMV8M_MAIR_ATTR_DEVICE_VAL      0x04
#define MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX      0
#define MPU_ARMV8M_MAIR_ATTR_CODE_VAL        0xAA
#define MPU_ARMV8M_MAIR_ATTR_CODE_IDX        1
#define MPU_ARMV8M_MAIR_ATTR_DATA_VAL        0xFF
#define MPU_ARMV8M_MAIR_ATTR_DATA_IDX        2
#define MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_VAL 0x44
#define MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX 3

struct mpu_armv8m_dev_t
{
  const uint32_t base;
};

enum mpu_armv8m_error_t
{
  MPU_ARMV8M_OK,
  MPU_ARMV8M_ERROR
};

enum mpu_armv8m_attr_exec_t
{
  MPU_ARMV8M_XN_EXEC_OK,
  MPU_ARMV8M_XN_EXEC_NEVER
};

enum mpu_armv8m_attr_access_t
{
  MPU_ARMV8M_AP_RW_PRIV_ONLY,
  MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
  MPU_ARMV8M_AP_RO_PRIV_ONLY,
  MPU_ARMV8M_AP_RO_PRIV_UNPRIV
};

enum mpu_armv8m_attr_shared_t
{
  MPU_ARMV8M_SH_NONE,
  MPU_ARMV8M_SH_UNUSED,
  MPU_ARMV8M_SH_OUTER,
  MPU_ARMV8M_SH_INNER
};

struct mpu_armv8m_region_cfg_t
{
  uint32_t region_nr;
  uint32_t region_base;
  uint32_t region_limit;
  uint32_t region_attridx;
  enum mpu_armv8m_attr_exec_t     attr_exec;
  enum mpu_armv8m_attr_access_t   attr_access;
  enum mpu_armv8m_attr_shared_t   attr_sh;
#ifdef FLOW_CONTROL
  uint32_t flow_step_enable;
  uint32_t flow_ctrl_enable;
  uint32_t flow_step_check;
  uint32_t flow_ctrl_check;
#endif
};

struct mpu_armv8m_region_cfg_raw_t
{
  uint32_t region_nr;
  uint32_t region_base;
  uint32_t region_limit;
};


/**
  * \brief Enable MPU
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  * \param[in] privdef_en     privilege default region 1:enable 0:disable
  * \param[in] hfnmi_en       mpu for hard fault & nmi  1:enable 0:disable
  *
  * \return Error code \ref mpu_armv8m_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */

enum mpu_armv8m_error_t mpu_armv8m_enable(struct mpu_armv8m_dev_t *dev,
                                          uint32_t privdef_en,
                                          uint32_t hfnmi_en);

/**
  * \brief Check MPU enabled
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  * \param[in] privdef_en     privilege default region 1:enable 0:disable
  * \param[in] hfnmi_en       mpu for hard fault & nmi  1:enable 0:disable
  *
  * \return Error code \ref mpu_armv8m_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */

enum mpu_armv8m_error_t mpu_armv8m_check(struct mpu_armv8m_dev_t *dev,
                                         uint32_t privdef_en,
                                         uint32_t hfnmi_en);

/**
  * \brief Disable MPU
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  *
  * \return Error code \ref arm_mpu_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */
enum mpu_armv8m_error_t mpu_armv8m_disable(struct mpu_armv8m_dev_t *dev);

/**
  * \brief Disable MPU and clean all regions
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  *
  * \return Error code \ref arm_mpu_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */
enum mpu_armv8m_error_t mpu_armv8m_clean(struct mpu_armv8m_dev_t *dev);

/**
  * \brief Enable MPU Region
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  * \param[in] region_cfg     MPU region config \ref mpu_armv8m_region_cfg_t
  *
  * \return Error code \ref arm_mpu_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */
enum mpu_armv8m_error_t mpu_armv8m_region_enable(
  struct mpu_armv8m_dev_t *dev,
  struct mpu_armv8m_region_cfg_t *region_cfg);

/**
  * \brief Check enabled MPU Region
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  * \param[in] region_cfg     MPU region config \ref mpu_armv8m_region_cfg_t
  *
  * \return Error code \ref arm_mpu_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */
enum mpu_armv8m_error_t mpu_armv8m_region_enable_check(
  struct mpu_armv8m_dev_t *dev,
  struct mpu_armv8m_region_cfg_t *region_cfg);

/**
  * \brief Disable MPU Region
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  * \param[in] region_nr            Region number
  *
  * \return Error code \ref arm_mpu_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */
enum mpu_armv8m_error_t mpu_armv8m_region_disable(
  struct mpu_armv8m_dev_t *dev,
  uint32_t region_nr);

/**
  * \brief Check disabled MPU Region
  *
  * \param[in] dev            MPU device \ref mpu_armv8m_dev_t
  * \param[in] region_nr            Region number
  *
  * \return Error code \ref arm_mpu_error_t
  *
  * \note This function doesn't check if dev is NULL.
  */
enum mpu_armv8m_error_t mpu_armv8m_region_disable_check(
  struct mpu_armv8m_dev_t *dev,
  uint32_t region_nr);
#endif /* __MPU_ARMV8M_DRV_H__ */
