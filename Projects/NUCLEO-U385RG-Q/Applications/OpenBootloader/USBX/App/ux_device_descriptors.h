/**
  ******************************************************************************
  * @file    ux_device_descriptors.h
  * @author  MCD Application Team
  * @brief   USBX Device descriptor header file
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
#ifndef __UX_DEVICE_DESCRIPTORS_H__
#define __UX_DEVICE_DESCRIPTORS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_stm32_config.h"

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USBD_MAX_NUM_CONFIGURATION                     1U
#define USBD_MAX_SUPPORTED_CLASS                       3U
#define USBD_MAX_CLASS_ENDPOINTS                       9U
#define USBD_MAX_CLASS_INTERFACES                      9U

#define USBD_DFU_CLASS_ACTIVATED                       1U

#define USBD_CONFIG_MAXPOWER                           25U
#define USBD_COMPOSITE_USE_IAD                         1U
#define USBD_DEVICE_FRAMEWORK_BUILDER_ENABLED          1U

/* Exported types ------------------------------------------------------------*/
/* Enum Class Type */
typedef enum
{
  CLASS_TYPE_NONE    = 0,
  CLASS_TYPE_HID     = 1,
  CLASS_TYPE_CDC_ACM = 2,
  CLASS_TYPE_MSC     = 3,
  CLASS_TYPE_CDC_ECM = 4,
  CLASS_TYPE_DFU     = 5,
} USBD_CompositeClassTypeDef;

/* USB Endpoint handle structure */
typedef struct
{
  uint32_t status;
  uint32_t total_length;
  uint32_t rem_length;
  uint32_t maxpacket;
  uint16_t is_used;
  uint16_t bInterval;
} USBD_EndpointTypeDef;

/* USB endpoint handle structure */
typedef struct
{
  uint8_t                     add;
  uint8_t                     type;
  uint16_t                    size;
  uint8_t                     is_used;
} USBD_EPTypeDef;

/* USB Composite handle structure */
typedef struct
{
  USBD_CompositeClassTypeDef   ClassType;
  uint32_t                     ClassId;
  uint8_t InterfaceType;      /* MKA */
  uint32_t                     Active;
  uint32_t                     NumEps;
  USBD_EPTypeDef               Eps[USBD_MAX_CLASS_ENDPOINTS];
  uint32_t                     NumIf;
  uint8_t                      Ifs[USBD_MAX_CLASS_INTERFACES];
} USBD_CompositeElementTypeDef;

/* USB Device handle structure */
typedef struct _USBD_DevClassHandleTypeDef
{
  uint8_t                 Speed;
  uint32_t                classId;
  uint32_t                NumClasses;
  USBD_CompositeElementTypeDef tclasslist[USBD_MAX_SUPPORTED_CLASS];
  uint32_t                CurrDevDescSz;
  uint32_t                CurrConfDescSz;
} USBD_DevClassHandleTypeDef;

/* USB Device endpoint direction */
typedef enum
{
  OUT   = 0x00,
  IN    = 0x80,
} USBD_EPDirectionTypeDef;

/* USB Device descriptors structure */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint16_t          bcdUSB;
  uint8_t           bDeviceClass;
  uint8_t           bDeviceSubClass;
  uint8_t           bDeviceProtocol;
  uint8_t           bMaxPacketSize;
  uint16_t          idVendor;
  uint16_t          idProduct;
  uint16_t          bcdDevice;
  uint8_t           iManufacturer;
  uint8_t           iProduct;
  uint8_t           iSerialNumber;
  uint8_t           bNumConfigurations;
} __PACKED USBD_DeviceDescTypedef;

/* USB Iad descriptors structure */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bFirstInterface;
  uint8_t           bInterfaceCount;
  uint8_t           bFunctionClass;
  uint8_t           bFunctionSubClass;
  uint8_t           bFunctionProtocol;
  uint8_t           iFunction;
} __PACKED USBD_IadDescTypedef;

/* USB interface descriptors structure */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bInterfaceNumber;
  uint8_t           bAlternateSetting;
  uint8_t           bNumEndpoints;
  uint8_t           bInterfaceClass;
  uint8_t           bInterfaceSubClass;
  uint8_t           bInterfaceProtocol;
  uint8_t           iInterface;
} __PACKED USBD_IfDescTypedef;

/* USB endpoint descriptors structure */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bEndpointAddress;
  uint8_t           bmAttributes;
  uint16_t          wMaxPacketSize;
  uint8_t           bInterval;
} __PACKED USBD_EpDescTypedef;

/* USB Config descriptors structure */
typedef struct
{
  uint8_t   bLength;
  uint8_t   bDescriptorType;
  uint16_t  wDescriptorLength;
  uint8_t   bNumInterfaces;
  uint8_t   bConfigurationValue;
  uint8_t   iConfiguration;
  uint8_t   bmAttributes;
  uint8_t   bMaxPower;
} __PACKED USBD_ConfigDescTypedef;

/* USB Qualifier descriptors structure */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint16_t          bcdDevice;
  uint8_t           Class;
  uint8_t           SubClass;
  uint8_t           Protocol;
  uint8_t           bMaxPacketSize;
  uint8_t           bNumConfigurations;
  uint8_t           bReserved;
} __PACKED USBD_DevQualiDescTypedef;

#if USBD_DFU_CLASS_ACTIVATED == 1
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bmAttributes;
  uint16_t          wDetachTimeout;
  uint16_t          wTransferSze;
  uint16_t          bcdDFUVersion;
} __PACKED USBD_DFUFuncDescTypedef;
#endif /* USBD_DFU_CLASS_ACTIVATED */

/* Private defines -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
uint8_t *USBD_Get_Device_Framework_Speed(uint8_t speed, ULONG *p_length);
uint8_t *USBD_Get_String_Framework(ULONG *p_length);
uint8_t *USBD_Get_Language_Id_Framework(ULONG *p_length);
uint16_t USBD_Get_Configuration_Number(uint8_t class_type, uint8_t interface_type);
uint16_t USBD_Get_Interface_Number(uint8_t class_type, uint8_t interface_type);

/* Private defines -----------------------------------------------------------*/
#define USBD_VID                                       0x0483U
#define USBD_PID                                       0xDF11U
#define USBD_LANGID_STRING                             0x409U
#define USBD_MANUFACTURER_STRING                       "STMicroelectronics"
#define USBD_PRODUCT_STRING                            "STM Device in DFU Mode"
#define USBD_SERIAL_NUMBER                             "DFU001"

#define USB_DESC_TYPE_INTERFACE                        0x04U
#define USB_DESC_TYPE_ENDPOINT                         0x05U
#define USB_DESC_TYPE_CONFIGURATION                    0x02U
#define USB_DESC_TYPE_IAD                              0x0BU

#define USBD_EP_TYPE_CTRL                              0x00U
#define USBD_EP_TYPE_ISOC                              0x01U
#define USBD_EP_TYPE_BULK                              0x02U
#define USBD_EP_TYPE_INTR                              0x03U

#define USBD_FULL_SPEED                                0x00U
#define USBD_HIGH_SPEED                                0x01U

#define USB_BCDUSB                                     0x0200U
#define LANGUAGE_ID_MAX_LENGTH                         2U

#define USBD_IDX_MFC_STR                               0x01U
#define USBD_IDX_PRODUCT_STR                           0x02U
#define USBD_IDX_SERIAL_STR                            0x03U

#define USBD_MAX_EP0_SIZE                              64U
#define USBD_DEVICE_QUALIFIER_DESC_SIZE                0x0AU

#define USBD_STRING_FRAMEWORK_MAX_LENGTH               256U

/* DFU parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define DFU_DESCRIPTOR_TYPE                           0x21U
#define USBD_DFU_BM_ATTRIBUTES                        0x0BU
#define USBD_DFU_DetachTimeout                        0xFFU
#define USBD_DFU_XFER_SIZE                            1024U

#define USBD_DFU_STRING_FLASH_DESC_INDEX              6U
#define USBD_DFU_STRING_FLASH_DESC                    "@Internal Flash   /0x08000000/256*04Kg"

#define USBD_DFU_STRING_OB1_DESC_INDEX                7U
#define USBD_DFU_STRING_OB1_DESC                     "@Option Bytes     /0x40022040/01*64 e"

#define USBD_DFU_ENGI_DESC_INDEX                      8U
#define USBD_DFU_ENGI_DESC_STR                        "@Engi Memory      /0x0BFA0500/01*2560 e"

#define USBD_DFU_STRING_OTP_DESC_INDEX                9U
#define USBD_DFU_STRING_OTP_DESC                      "@OTP Memory       /0x0BFA0000/01*512 e"

#define USBD_DFU_STRING_DEVICE_ID_DESC_INDEX          20U
#define USBD_DFU_STRING_DEVICE_ID_DESC                "@Device ID/0x454,@Revision ID/0x0001"

/* This is the maximum supported configuration descriptor size
   User may redefine this value in order to optima */
#ifndef USBD_FRAMEWORK_MAX_DESC_SZ
#define USBD_FRAMEWORK_MAX_DESC_SZ                    200U
#endif /* USBD_FRAMEWORK_MAX_DESC_SZ */

#ifndef USBD_CONFIG_STR_DESC_IDX
#define USBD_CONFIG_STR_DESC_IDX                      0U
#endif /* USBD_CONFIG_STR_DESC_IDX */

#ifndef USBD_CONFIG_BMATTRIBUTES
#define USBD_CONFIG_BMATTRIBUTES                      0xC0U
#endif /* USBD_CONFIG_BMATTRIBUTES */

/* Private macro -----------------------------------------------------------*/
#define __USBD_FRAMEWORK_SET_EP(epadd, eptype, epsize, HSinterval, FSinterval) do { \
                                                                                    /* Append Endpoint descriptor to Configuration descriptor */ \
                                                                                    pEpDesc = ((USBD_EpDescTypedef*)((uint32_t)pconf + *p_sze)); \
                                                                                    pEpDesc->bLength            = (uint8_t)sizeof(USBD_EpDescTypedef); \
                                                                                    pEpDesc->bDescriptorType    = USB_DESC_TYPE_ENDPOINT; \
                                                                                    pEpDesc->bEndpointAddress   = (epadd); \
                                                                                    pEpDesc->bmAttributes       = (eptype); \
                                                                                    pEpDesc->wMaxPacketSize     = (epsize); \
                                                                                    if(pdev->Speed == USBD_HIGH_SPEED) \
                                                                                    { \
                                                                                      pEpDesc->bInterval        = (HSinterval); \
                                                                                    } \
                                                                                    else \
                                                                                    { \
                                                                                      pEpDesc->bInterval        = (FSinterval); \
                                                                                    } \
                                                                                    *p_sze += (uint32_t)sizeof(USBD_EpDescTypedef); \
                                                                                  } while(0)

#define __USBD_FRAMEWORK_SET_IF(ifnum, alt, eps, class, subclass, protocol, istring) do {\
                                                                                          /* Interface Descriptor */ \
                                                                                          pIfDesc = ((USBD_IfDescTypedef*)((uint32_t)pconf + *p_sze)); \
                                                                                          pIfDesc->bLength = (uint8_t)sizeof(USBD_IfDescTypedef); \
                                                                                          pIfDesc->bDescriptorType = USB_DESC_TYPE_INTERFACE; \
                                                                                          pIfDesc->bInterfaceNumber = (ifnum); \
                                                                                          pIfDesc->bAlternateSetting = (alt); \
                                                                                          pIfDesc->bNumEndpoints = (eps); \
                                                                                          pIfDesc->bInterfaceClass = (class); \
                                                                                          pIfDesc->bInterfaceSubClass = (subclass); \
                                                                                          pIfDesc->bInterfaceProtocol = (protocol); \
                                                                                          pIfDesc->iInterface = (istring); \
                                                                                          *p_sze += (uint32_t)sizeof(USBD_IfDescTypedef); \
                                                                                        } while(0)
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __UX_DEVICE_DESCRIPTORS_H__ */
