/**
  ******************************************************************************
  * @file    stm32_cert.c
  * @author  GPM Application Team
  * @brief   This file provides function to get a certificate X-509
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
#include "string.h"
#include "stm32_cert.h"

/* DUA User certificate base address */
__IO uint8_t *p_Blob = (uint8_t *)CERT_CHIP_PACK2_ADDR;

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_DISCO
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @defgroup STM32_CERT CERTIFICATES Utility
  * @{
  */

/**
  * @brief  CERTIFICATES Utility general definitions values
  */

#define CERT_SERIAL_NUMBER_SIZE_BYTES (10U)
#define MODULUS_MAX_SIZE_BYTES (32U)

#define CERT_VALIDITY_DURATION_YEARS (30U)

/* Template offset of the DUA User */
#define CERT_TEMP_OFFSET_SERIAL_DUA_USER (15U)
#define CERT_TEMP_OFFSET_ISSUER_CA_NUMBER_DUA_USER (101U)
#define CERT_TEMP_OFFSET_VALIDITY_START_YEAR_DUA_USER (122U)
#define CERT_TEMP_OFFSET_VALIDITY_END_YEAR_DUA_USER (139U)
#define CERT_TEMP_OFFSET_SUBJECT_DUA_USER (225U)
#define CERT_TEMP_OFFSET_PUBKEY_DUA_USER (272U)

/**
  * @brief  CERTIFICATES Utility Template offset definition
  */
typedef struct
{
  const uint32_t serial;     /*!< ID of the certificate                 */
  const uint32_t ca_nb;      /*!< Certification authority               */
  const uint32_t val_start;  /*!< Start of the certificate validity     */
  const uint32_t val_end;    /*!< End of the certificate validity       */
  const uint32_t subject;    /*!< Name of the object of the certificate */
  const uint32_t pub;        /*!< Public key template                   */
} offsetsTemp_t;

/**
  * @brief  CERTIFICATES Utility Non Secure System Flash offset definition
  */
typedef struct
{
  const uint32_t serial;          /*!< Unique ID of the certificate           */
  const uint32_t pub_key;         /*!< Public key of the certificate          */
  const uint32_t sig;             /*!< Algorithm of the certificate signature */
} offsetBlob_t;

/**
  * @brief  CERTIFICATES Utility Template definition
  */
typedef struct
{
  const offsetsTemp_t *offsets;  /*!< Offset of the template certificate component */
  const uint8_t *cert;          /*!< Components of the template certificate       */
} template_t;

/* Data Template to complete the certificate authority field :
   Common to DUA USER and DUA INITIAL ATTESTATION
*/
static const uint8_t X509ExtNoCA[] =
{
  0xA3, 0x10, 0x30, 0x0E, 0x30, 0x0C, 0x06, 0x03,
  0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x02,
  0x30, 0x00
};

/* Data Template to complete the signature algorithm field :
   Common to DUA USER and DUA INITIAL ATTESTATION
*/
static const uint8_t X509SignatureObjectId[] =
{
  0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE,
  0x3D, 0x04, 0x03, 0x02
};

static const uint8_t NibbleToHex[] =
{
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

/* Offset for Template data DUA User */
static const offsetsTemp_t OffsetsTempDuaUser =
{
  CERT_TEMP_OFFSET_SERIAL_DUA_USER,
  CERT_TEMP_OFFSET_ISSUER_CA_NUMBER_DUA_USER,
  CERT_TEMP_OFFSET_VALIDITY_START_YEAR_DUA_USER,
  CERT_TEMP_OFFSET_VALIDITY_END_YEAR_DUA_USER,
  CERT_TEMP_OFFSET_SUBJECT_DUA_USER,
  CERT_TEMP_OFFSET_PUBKEY_DUA_USER
};

static const template_t TemplateDua[2] =
{
  {
    &OffsetsTempDuaUser,
    CertDuaUserFU
  },
  {
    &OffsetsTempDuaUser,
    CertDuaUserLU
  }
};

/* Offset in the system flash to retrieve DUA USER certificate information */
static const offsetBlob_t OBlobDuaUserFU =
{
  CERT_ST_DUA_USER_FU_SERIAL_OFFSET,
  CERT_ST_DUA_USER_FU_PUB_KEY_OFFSET,
  CERT_ST_DUA_USER_FU_SIGN_OFFSET,
};

/* Offset in the system flash to retrieve DUA USLU certificate information */
static const offsetBlob_t OBlobDuaUserLU =
{
  CERT_ST_DUA_USER_LU_SERIAL_OFFSET,
  CERT_ST_DUA_USER_LU_PUB_KEY_OFFSET,
  CERT_ST_DUA_USER_LU_SIGN_OFFSET,
};

/** @defgroup UTIL_STM32_CERT_Private_FunctionPrototypes CERTIFICATES Utility Private FunctionPrototypes
  * @{
  */
static uint32_t GetAsn1IntegerEncodingInfo(const uint8_t *p_raw, uint32_t size, uint32_t *p_offset);
static uint32_t Asn1EncodeInteger(const uint8_t *p_raw, uint32_t size, uint8_t *p_encoded);
static uint32_t GetAsn1SignatureEncodingLength(const uint8_t *p_signature);
static uint32_t Asn1EncodeSignature(const uint8_t *p_raw_signature, uint8_t *p_asn1_signature);
/**
  * @}
  */

/**
  * @brief  Get ASN.1 information on integer encoded
  * @param  p_raw Input Integer
  * @param  size Size of the input data
  * @param  p_offset Offset of first non-zero byte in integer
  * @retval Length of the ASN1 encoded integer
  */
static uint32_t GetAsn1IntegerEncodingInfo(const uint8_t *p_raw, uint32_t size, uint32_t *p_offset)
{
  uint32_t start = 0U;
  uint32_t len = 0U;

  /* Remove leading 0s from p_raw integer */
  while ((p_raw[start] == 0U) && (start < size))
  {
    start++;
  }

  if (start >= size)
  {
    /* zero */
    *p_offset = size - 1U;
    return 3U;
  }

  len = size - start;

  if ((p_raw[start] >> 7) == 1U)
  {
    len += 1U;
  }

  if (len >= 128U)
  {
    len += 1U;
  }
  *p_offset = start;
  return len + 2U;
}

/**
  * @brief  Encode integer in ASN.1 format
  * @param  p_raw Integer to encode in ASN.1 format
  * @param  size Size of the input integer
  * @param  p_encoded Integer encoded in ASN.1 format
  * @retval Length of the integer encoded
  */
static uint32_t Asn1EncodeInteger(const uint8_t *p_raw, uint32_t size, uint8_t *p_encoded)
{
  /* Produce 02 SIZE X Number */
  uint32_t start = 0U;
  uint32_t len = 0U;
  uint32_t index = 0U;
  p_encoded[index++] = 0x02U;
  len = GetAsn1IntegerEncodingInfo(p_raw, size, &start);

  if ((len - 2U) >= 128U)
  {
    p_encoded[index++] = 0x81U;
  }
  p_encoded[index++] = len - 2U;
  if ((p_raw[start] >> 7) == 1U)
  {
    p_encoded[index++] = 0x00U;
  }
  memcpy(&p_encoded[index], &p_raw[start], size - start);
  return len;
}

/**
  * @brief  Get length of the signature encoded in ASN.1 format
  * @param  p_signature Signature
  * @retval Length of the encoded signature
  */
static uint32_t GetAsn1SignatureEncodingLength(const uint8_t *p_signature)
{
  uint32_t sigR_start = 0U;
  uint32_t sigS_start = 0U;
  uint32_t encoded_r_len = GetAsn1IntegerEncodingInfo(&p_signature[0], MODULUS_MAX_SIZE_BYTES, &sigR_start);
  uint32_t encoded_s_len = GetAsn1IntegerEncodingInfo(&p_signature[MODULUS_MAX_SIZE_BYTES],
                                                      MODULUS_MAX_SIZE_BYTES, &sigS_start);
  uint32_t len = encoded_r_len + encoded_s_len;
  if (len >= 128U)
  {
    len += 1U;
  }
  return len + 2U;
}

/**
  * @brief  Encode signature in ASN.1 format
  * @param  p_raw_signature  Signature to be encode in ASN.1 format
  * @param  p_asn1_signature Signature encoded in ASN.1 format
  * @retval Length of the signature encoded
  */
static uint32_t Asn1EncodeSignature(const uint8_t *p_raw_signature, uint8_t *p_asn1_signature)
{
  uint32_t index = 0U;
  uint32_t sigR_start = 0U;
  uint32_t sigS_start = 0U;
  uint32_t encoded_sig_len = 0U;
  uint32_t encoded_r_len = GetAsn1IntegerEncodingInfo(&p_raw_signature[0], MODULUS_MAX_SIZE_BYTES, &sigR_start);
  uint32_t encoded_s_len = GetAsn1IntegerEncodingInfo(&p_raw_signature[MODULUS_MAX_SIZE_BYTES], MODULUS_MAX_SIZE_BYTES,
                                                      &sigS_start);
  encoded_sig_len = encoded_r_len + encoded_s_len;
  p_asn1_signature[index++] = 0x30U;
  if (encoded_sig_len >= 128U)
  {
    p_asn1_signature[index++] = 0x81U;
  }
  p_asn1_signature[index++] = encoded_sig_len;
  Asn1EncodeInteger(&p_raw_signature[0], MODULUS_MAX_SIZE_BYTES, &p_asn1_signature[index]);
  index += encoded_r_len;
  Asn1EncodeInteger(&p_raw_signature[MODULUS_MAX_SIZE_BYTES], MODULUS_MAX_SIZE_BYTES, &p_asn1_signature[index]);
  index += encoded_s_len;
  return index;
}

/** @defgroup UTIL_STM32_CERT_Exported_Functions CERTIFICATES Utility Exported Functions
  * @{
  */

/**
  * @brief  Get the X509 certificate size.
  * @param  certificate_id     ID of the certificate type
  *                            - 1 : DUA USER
  *                            - 2 : DUA USLU
  * @param  p_certificate_size  Pointer to store the certificate size
  * @retval CERT_Status_t       Certificate status
  */
CERT_Status_t UTIL_CERT_GetCertificateSize(const CERT_Id_t certificate_id, uint32_t *p_certificate_size)
{
  const template_t *template;
  const offsetBlob_t *offset_blob;
  uint32_t certif_index = 0U;

  /* Select the offset and data of the DUA type certificate */
  if ((certificate_id == DUA_USER_FU) && (p_certificate_size != NULL))
  {
    template = &TemplateDua[0];
    offset_blob = &OBlobDuaUserFU;
    p_Blob = (uint8_t *)CERT_CHIP_PACK2_ADDR;
  }
  else if ((certificate_id == DUA_USER_LU) && (p_certificate_size != NULL))
  {
    template = &TemplateDua[1];
    offset_blob = &OBlobDuaUserLU;
    p_Blob = (uint8_t *)CERT_CHIP_PACK2_ADDR;
  }
  else
  {
    return CERT_ERROR;
  }

  /* Add the length of certificate components */
  certif_index = template->offsets->pub + 2 * MODULUS_MAX_SIZE_BYTES + sizeof(X509ExtNoCA) + \
                 sizeof(X509SignatureObjectId) + 3;

  /* Calculate and add the length of the signature */
  certif_index += GetAsn1SignatureEncodingLength((uint8_t const *)&p_Blob[offset_blob->sig]);
  *p_certificate_size = certif_index;

  return CERT_OK;
}

/**
  * @brief  Get the X509 certificate.
  * @param  certificate_id     ID of the certificate type
  *                            - 1 : DUA USER
  *                            - 2 : DUA USLU
  * @param  p_certificate       Pointer to store the certificate
  * @retval CERT_Status_t       Certificate status
  */
CERT_Status_t UTIL_CERT_GetCertificate(const CERT_Id_t certificate_id, uint8_t *p_certificate)
{
  uint8_t cur_serial_byte = 0U;
  uint32_t certif_index = 0U;
  uint32_t signature_encoded_length = 0U;
  const template_t *template;
  const offsetBlob_t *offset_blob;

  /* Select the offset and data of the DUA type certificate */
  if ((certificate_id == DUA_USER_FU) && (p_certificate != NULL))
  {
    template = &TemplateDua[0];
    offset_blob = &OBlobDuaUserFU;
    p_Blob = (uint8_t *)CERT_CHIP_PACK2_ADDR;
  }
  else if ((certificate_id == DUA_USER_LU) && (p_certificate != NULL))
  {
    template = &TemplateDua[1];
    offset_blob = &OBlobDuaUserLU;
    p_Blob = (uint8_t *)CERT_CHIP_PACK2_ADDR;
  }
  else
  {
    return CERT_ERROR;
  }

  memcpy(p_certificate, &(template->cert[0]), template->offsets->serial);

  /* Overwrite serial number */
  certif_index += template->offsets->serial;
  memcpy(&p_certificate[certif_index], (void const *)&p_Blob[offset_blob->serial], CERT_SERIAL_NUMBER_SIZE_BYTES);
  certif_index += CERT_SERIAL_NUMBER_SIZE_BYTES;
  memcpy(&p_certificate[certif_index], &(template->cert[certif_index]), template->offsets->ca_nb - certif_index);

  /* Overwrite CA number */
  certif_index = template->offsets->ca_nb;
  cur_serial_byte =  p_Blob[offset_blob->serial];
  p_certificate[certif_index++] = NibbleToHex[(cur_serial_byte & 0xF0) >> 4];
  p_certificate[certif_index++] = NibbleToHex[(cur_serial_byte & 0x0F)];
  memcpy(&p_certificate[certif_index], &(template->cert[certif_index]), template->offsets->val_start - certif_index);

  /* Overwrite validity start year */
  certif_index = template->offsets->val_start;
  cur_serial_byte =  p_Blob[offset_blob->serial + 1];
  /* ascii encoding of decimal value for decade */
  p_certificate[certif_index++] = (cur_serial_byte / 10) + 0x30;
  /* ascii encoding of decimal value for year in decade */
  p_certificate[certif_index++] = (cur_serial_byte % 10) + 0x30;
  memcpy(&p_certificate[certif_index], &(template->cert[certif_index]), template->offsets->val_end - certif_index);

  /* Overwrite validity end year */
  certif_index = template->offsets->val_end;
  cur_serial_byte += CERT_VALIDITY_DURATION_YEARS;
  p_certificate[certif_index++] = (cur_serial_byte / 10) + 0x30;
  p_certificate[certif_index++] = (cur_serial_byte % 10) + 0x30;
  memcpy(&p_certificate[certif_index], &(template->cert[certif_index]), template->offsets->subject - certif_index);
  certif_index = template->offsets->subject;
  for (uint32_t i = 0; i < CERT_SERIAL_NUMBER_SIZE_BYTES; i++)
  {
    cur_serial_byte =  p_Blob[offset_blob->serial + i];
    p_certificate[certif_index++] = NibbleToHex[(cur_serial_byte & 0xF0) >> 4];
    p_certificate[certif_index++] = NibbleToHex[(cur_serial_byte & 0x0F)];
  }
  memcpy(&p_certificate[certif_index], &(template->cert[certif_index]), template->offsets->pub - certif_index);
  certif_index = template->offsets->pub;
  memcpy(&p_certificate[certif_index], (void const *)&p_Blob[offset_blob->pub_key], 2 * MODULUS_MAX_SIZE_BYTES);
  certif_index += 2 * MODULUS_MAX_SIZE_BYTES;

  /* Append x509 extension indicating CA = False */
  memcpy(&p_certificate[certif_index], X509ExtNoCA, sizeof(X509ExtNoCA));
  certif_index += sizeof(X509ExtNoCA);

  /* Append signature identifier */
  memcpy(&p_certificate[certif_index], X509SignatureObjectId, sizeof(X509SignatureObjectId));
  certif_index += sizeof(X509SignatureObjectId);

  /* Append signature */
  signature_encoded_length = GetAsn1SignatureEncodingLength((uint8_t const *)&p_Blob[offset_blob->sig]) + 1;
  p_certificate[certif_index++] = 0x03U;
  p_certificate[certif_index++] = signature_encoded_length;
  p_certificate[certif_index++] = 0U;
  certif_index += Asn1EncodeSignature((uint8_t const *)&p_Blob[offset_blob->sig], &p_certificate[certif_index]);

  /* Adjust length of total certificate */
  p_certificate[2] = ((certif_index - 4) & 0xFF00) >> 8;
  p_certificate[3] = ((certif_index - 4) & 0xFF);

  return CERT_OK;
}
/**
  * @}
  */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
