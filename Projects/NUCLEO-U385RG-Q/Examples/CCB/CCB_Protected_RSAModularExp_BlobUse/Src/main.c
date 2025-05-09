/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u3xx_nucleo.h"
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
CCB_HandleTypeDef hccb;

/* USER CODE BEGIN PV */
CCB_WrappingKeyTypeDef wrappedKeyConf;
CCB_RSAParamTypeDef RSAparam;
CCB_RSAKeyBlobTypeDef RSAKeyBlob;
CCB_RSAClearKeyTypeDef RSAKey;

/* Input buffers */
uint32_t AESIV[4]                       = {0x00010203 , 0x04050607 , 0x08090A0B , 0x0C0D0E0F};
uint32_t Wrapped_key_Buffer[8]          = {0x1ff4d7fb, 0xbb515f14, 0x676df6b4, 0xcb58c0c8, 0x2b48e65e, 0xf9bfcf32, 0xd9c0a66c, 0xeec362d7};

uint8_t RSA4096_exp_P[512]              = { 0x28, 0x28, 0x5d, 0x3a, 0x7e, 0x14, 0x8f, 0x2f, 0x63, 0x4b, 0x90, 0x97, 0x37, 0xa2, 0xf0,
                                            0x6a, 0xba, 0xa5, 0x7e, 0xe8, 0x6a, 0xe5, 0xf3, 0x4d, 0x52, 0xe2, 0xfb, 0xbe, 0xf3, 0x27,
                                            0xab, 0xdb, 0x5a, 0x45, 0xf2, 0xa0, 0xa6, 0x70, 0x12, 0x2d, 0x5d, 0xc0, 0x42, 0x5d, 0xee,
                                            0xf8, 0xc1, 0xd7, 0x10, 0x46, 0xef, 0xd5, 0x03, 0x47, 0xed, 0xcd, 0x71, 0x19, 0xf1, 0x39,
                                            0x65, 0xbb, 0x17, 0xd4, 0x1f, 0x85, 0x54, 0x9e, 0x16, 0xcf, 0xc0, 0xe7, 0x1f, 0x52, 0x67,
                                            0xd1, 0xbc, 0x00, 0x32, 0x95, 0x6c, 0x60, 0x87, 0x76, 0xd8, 0x73, 0x37, 0x46, 0x5d, 0xbd,
                                            0x7c, 0x07, 0xcb, 0x9b, 0x36, 0x35, 0xd9, 0x89, 0xbc, 0x61, 0x73, 0xed, 0x0e, 0x2d, 0xc3,
                                            0x1e, 0x88, 0x11, 0xd5, 0x5a, 0x32, 0xf2, 0xe7, 0xff, 0x77, 0x54, 0xbe, 0x07, 0xa0, 0xc7,
                                            0xa7, 0xd9, 0x08, 0x3c, 0x82, 0xa4, 0x17, 0xb5, 0x42, 0x68, 0x8c, 0x53, 0x93, 0x03, 0x5d,
                                            0x91, 0x0a, 0xbc, 0x01, 0x32, 0xd1, 0x0d, 0xbc, 0x4d, 0x11, 0x42, 0x6d, 0x02, 0x64, 0x33,
                                            0xbd, 0x6d, 0x3a, 0x42, 0x7b, 0x52, 0x14, 0x49, 0x2d, 0x04, 0x28, 0x84, 0xaa, 0xb6, 0x82,
                                            0x42, 0x45, 0x8c, 0x5f, 0x21, 0x19, 0xe9, 0x8f, 0xb5, 0x49, 0xf7, 0xfa, 0x5f, 0xc2, 0xf8,
                                            0x8d, 0x79, 0x71, 0x7f, 0x0c, 0xd0, 0x5c, 0x46, 0x98, 0x4c, 0x27, 0x32, 0x61, 0xab, 0x9f,
                                            0x05, 0x79, 0xcb, 0x67, 0xa3, 0xce, 0x35, 0x21, 0x91, 0x17, 0x3f, 0x9c, 0x19, 0xea, 0x88,
                                            0x4b, 0x1a, 0x51, 0x97, 0x73, 0xcd, 0x77, 0x0d, 0x54, 0xe6, 0x80, 0x33, 0x6a, 0x0a, 0x5e,
                                            0x65, 0x9f, 0x2e, 0xe9, 0x05, 0xb0, 0x20, 0x95, 0xf4, 0x94, 0x42, 0x1a, 0xca, 0xd9, 0x9f,
                                            0x63, 0xe9, 0x7b, 0xde, 0x9d, 0x57, 0x5c, 0x0b, 0xe5, 0x52, 0x4c, 0x31, 0xae, 0x34, 0x19,
                                            0x73, 0x4e, 0xc2, 0x48, 0x62, 0xb3, 0x71, 0x70, 0x59, 0x92, 0x67, 0x0e, 0xd8, 0xdf, 0x1c,
                                            0xad, 0x71, 0xd8, 0x0d, 0x35, 0x81, 0x9e, 0x69, 0x30, 0x03, 0x3f, 0x3d, 0x2e, 0xf6, 0x59,
                                            0x09, 0xc0, 0x3b, 0xda, 0x87, 0xbe, 0xde, 0x68, 0x9a, 0x56, 0x55, 0x47, 0x4c, 0xc0, 0x5f,
                                            0xa1, 0xb9, 0x10, 0xb3, 0x31, 0x22, 0x93, 0x55, 0x92, 0x53, 0x3f, 0x21, 0xde, 0x72, 0x24,
                                            0x9c, 0x54, 0x09, 0x26, 0x4d, 0x25, 0xef, 0x8d, 0x67, 0x06, 0x5b, 0x60, 0x5c, 0x66, 0x2e,
                                            0xc6, 0xe8, 0x8b, 0x3d, 0x70, 0xe0, 0xe5, 0x4f, 0x6d, 0x3f, 0x8a, 0xcf, 0x85, 0xb9, 0x9e,
                                            0xd9, 0x16, 0xdc, 0x77, 0x69, 0x4b, 0xca, 0xdd, 0xf3, 0xb8, 0xd8, 0x60, 0x16, 0x43, 0xb8,
                                            0x03, 0xc3, 0x9a, 0x8f, 0xa0, 0x64, 0x72, 0x79, 0x8f, 0x39, 0x5a, 0x71, 0x7f, 0xbe, 0xd7,
                                            0x00, 0x1c, 0x03, 0x8f, 0x04, 0x49, 0x2d, 0x42, 0x98, 0x33, 0xa0, 0x85, 0xae, 0xf4, 0x8c,
                                            0x8e, 0xb6, 0xaf, 0xea, 0xba, 0xb1, 0x4a, 0x0a, 0xc2, 0x8c, 0x22, 0x59, 0xec, 0x04, 0xbc,
                                            0x70, 0x1c, 0x47, 0x0f, 0x2b, 0x97, 0x67, 0xb1, 0x74, 0xc3, 0xc6, 0x27, 0x73, 0xac, 0xe6,
                                            0x09, 0x40, 0x25, 0x69, 0xd5, 0xaf, 0xba, 0xc1, 0xda, 0xa2, 0x42, 0xb9, 0x35, 0x37, 0x95,
                                            0x69, 0xff, 0x32, 0xae, 0xe6, 0x46, 0xaf, 0xa7, 0xbf, 0xe8, 0x35, 0xfd, 0x04, 0xa2, 0xb7,
                                            0xf9, 0xc6, 0xe4, 0xa7, 0x2c, 0x61, 0xc2, 0x2b, 0xd1, 0xc8, 0x41, 0x49, 0x42, 0x07, 0xcb,
                                            0xa0, 0xe5, 0x16, 0x5f, 0xcd, 0x72, 0xb0, 0xd5, 0xa2, 0x83, 0x06, 0x0a, 0xf7, 0x09, 0x79,
                                            0x06, 0x97, 0xb8, 0x72, 0xe5, 0x8a, 0x1f, 0x42, 0x13, 0x68, 0xbf, 0x5a, 0xf5, 0x0a, 0xa7,
                                            0xa2, 0x59, 0x8e, 0x23, 0x61, 0x05, 0x75, 0x22, 0x97, 0xe9, 0x0b, 0x1b, 0xbd, 0x2e, 0x71,
                                            0x5a, 0x8f};
uint8_t RSA4096_expBase[512]            = { 0x4b, 0x26, 0xe6, 0xd6, 0x27, 0x3c, 0xb6, 0x5e, 0x5c, 0xff, 0x06, 0x97, 0x02, 0x67, 0x5b,
                                            0x71, 0x13, 0x5d, 0x3d, 0x0a, 0xb5, 0x4d, 0x4b, 0xbc, 0xbf, 0xf2, 0x91, 0x69, 0x7e, 0xb5,
                                            0xd8, 0x9f, 0x19, 0xed, 0xda, 0x9e, 0x30, 0x92, 0x33, 0x46, 0x70, 0xef, 0xe5, 0xb6, 0x02,
                                            0x15, 0x6c, 0x68, 0x31, 0x3b, 0x81, 0xe2, 0xfb, 0xc2, 0xc7, 0x56, 0x1d, 0x98, 0x14, 0xfa,
                                            0x2b, 0x5d, 0x32, 0xab, 0x78, 0x0b, 0x68, 0x1e, 0x6a, 0xbe, 0xfd, 0x36, 0x97, 0xe8, 0x2a, /* encrypted_data is the result of the openssl command: */
                                            0xf6, 0x6b, 0x08, 0x02, 0x37, 0xc5, 0x2d, 0xba, 0x61, 0x12, 0x2f, 0xc4, 0xb0, 0x20, 0xbd, /* $ openssl pkeyutl -encrypt -pubin -inkey pub4096.pem  -out encrypt_with_pub.txt -in clair_data.txt */
                                            0x59, 0xd9, 0x83, 0x6b, 0xc9, 0x17, 0xc0, 0x8a, 0xd9, 0x3d, 0xbc, 0x5d, 0x96, 0x5c, 0x95, /* clair_data.txt contains the message "Hello world" */
                                            0x8f, 0x8e, 0xa4, 0x6f, 0xd0, 0xb5, 0xd3, 0x8e, 0x4a, 0x7b, 0xef, 0xef, 0x29, 0xd1, 0x30,
                                            0xfc, 0x20, 0x3c, 0x84, 0x5d, 0x08, 0x51, 0xc0, 0xd4, 0x45, 0xe8, 0xfe, 0x70, 0x5c, 0x01,
                                            0x06, 0x55, 0xe3, 0x7d, 0x7b, 0x85, 0xb0, 0x8c, 0xcd, 0x62, 0x20, 0x32, 0xe5, 0x28, 0xff,
                                            0x7f, 0xc4, 0xaf, 0x09, 0xb3, 0xe2, 0xf5, 0x78, 0x4a, 0x68, 0xe0, 0xa4, 0x6b, 0x09, 0xe3,
                                            0x70, 0xfe, 0xe8, 0x97, 0x8d, 0x48, 0x3a, 0xe9, 0x65, 0xbc, 0x97, 0x0e, 0x1c, 0xea, 0xb3,
                                            0xe1, 0x31, 0x21, 0xd6, 0xcc, 0xf7, 0x7c, 0xb3, 0x72, 0x91, 0xb0, 0xff, 0xaa, 0x3c, 0x05,
                                            0x21, 0xb6, 0x88, 0x49, 0xb9, 0xcb, 0x03, 0xdc, 0x17, 0x98, 0xd1, 0x37, 0x77, 0x3a, 0xd7,
                                            0x72, 0x62, 0xbf, 0x9e, 0xf9, 0x17, 0xa4, 0xcf, 0x56, 0x6f, 0x8e, 0x65, 0x72, 0xf5, 0xa2,
                                            0x15, 0x6c, 0x73, 0x40, 0xae, 0xa8, 0x37, 0x07, 0x76, 0x9d, 0xed, 0xe0, 0xae, 0xbd, 0x35,
                                            0x7d, 0x0c, 0x2a, 0x1c, 0x64, 0x71, 0x18, 0x75, 0x82, 0x68, 0x5c, 0x64, 0xcd, 0xda, 0x18,
                                            0x5b, 0x7a, 0xb9, 0xca, 0x55, 0xfb, 0x0b, 0x57, 0xb8, 0x9c, 0x3d, 0x43, 0x5c, 0x5a, 0xb0,
                                            0xfb, 0x9c, 0x83, 0xec, 0x08, 0x06, 0xf5, 0x95, 0x96, 0x77, 0x5d, 0x3f, 0xb3, 0x27, 0xdd,
                                            0xd3, 0xf8, 0xc6, 0x53, 0xf8, 0xdc, 0x2d, 0x0a, 0x5c, 0xda, 0xd8, 0xac, 0xfa, 0x98, 0x2a,
                                            0x50, 0x64, 0x39, 0x9d, 0x3f, 0x78, 0xa4, 0x5f, 0x06, 0xd4, 0x16, 0x47, 0x59, 0x74, 0xd9,
                                            0x95, 0x25, 0xc2, 0xf3, 0x02, 0x2b, 0x81, 0x55, 0x2c, 0xde, 0x99, 0x20, 0x82, 0xf2, 0xae,
                                            0x1f, 0x57, 0x35, 0x14, 0xfb, 0x18, 0xbb, 0x3d, 0xed, 0x53, 0x2f, 0xf9, 0x4e, 0xd0, 0xb0,
                                            0x02, 0xfc, 0xb7, 0xaf, 0x7b, 0xd1, 0x4d, 0x8e, 0xa1, 0xf9, 0x10, 0xcc, 0x9f, 0xbc, 0x9c,
                                            0xb4, 0xd8, 0x4a, 0x21, 0xac, 0xe8, 0x2f, 0x4f, 0xfd, 0x98, 0xbe, 0xcc, 0xb2, 0xaf, 0x4a,
                                            0xdd, 0xda, 0x3b, 0xcd, 0x1a, 0x34, 0xaf, 0xa4, 0x5b, 0x81, 0xd6, 0xa4, 0xfa, 0x9c, 0xc3,
                                            0xbb, 0x11, 0x56, 0x98, 0xf3, 0x09, 0x69, 0xda, 0x48, 0x85, 0x0e, 0x7a, 0xf7, 0xad, 0xec,
                                            0x0c, 0x09, 0x1c, 0xcf, 0x13, 0x8e, 0x55, 0x6f, 0xad, 0x2c, 0x11, 0x2b, 0xd2, 0x7d, 0x44,
                                            0x9b, 0xd5, 0xcb, 0x8d, 0xce, 0x43, 0x7f, 0xf7, 0x12, 0x11, 0x74, 0xe4, 0xe0, 0xaf, 0x90,
                                            0x30, 0x62, 0xbc, 0xee, 0x88, 0x0c, 0xb9, 0xc0, 0x42, 0xac, 0x9a, 0xbd, 0xa4, 0xeb, 0xb9,
                                            0x76, 0x25, 0x4e, 0x45, 0x8b, 0x3b, 0xff, 0x74, 0xf6, 0x1a, 0x54, 0x60, 0x66, 0x4e, 0x39,
                                            0x65, 0x77, 0x2d, 0xe2, 0x89, 0xcf, 0x00, 0x7c, 0x91, 0x2f, 0x81, 0x8f, 0x18, 0x3e, 0xdb,
                                            0x0a, 0xee, 0x37, 0x9f, 0x28, 0x98, 0x0f, 0xa1, 0xd1, 0x1d, 0x75, 0x32, 0x48, 0x8d, 0x17,
                                            0x8f, 0x10, 0x6d, 0x6c, 0x1d, 0xfa, 0x4f, 0xd2, 0x52, 0x3b, 0x52, 0x3d, 0x8d, 0x86, 0xed,
                                            0x6b, 0x08};

uint8_t RSA4096_phi_P[512]              = { 0xd1, 0xea, 0x52, 0xbb, 0xdb, 0xd6, 0xe7, 0x09, 0x79, 0x51, 0x26, 0xe7, 0x49, 0xa9, 0xef,
                                            0xa3, 0x47, 0x9f, 0x92, 0xd6, 0xf1, 0xb4, 0x45, 0x5e, 0x39, 0xe4, 0xb6, 0x8a, 0xcd, 0xf9,
                                            0xff, 0x43, 0xf2, 0x2e, 0xc1, 0xc4, 0x02, 0xd5, 0x1f, 0x7c, 0x03, 0x03, 0xaa, 0xe6, 0x0b,
                                            0x63, 0x28, 0xe7, 0xc1, 0x50, 0x80, 0xcc, 0xe7, 0x86, 0xe5, 0xec, 0x31, 0x1d, 0x6b, 0x7b,
                                            0x22, 0x62, 0x8d, 0x62, 0x40, 0x7d, 0xdd, 0x2c, 0x97, 0xd4, 0x9f, 0xef, 0xff, 0x0d, 0x14,
                                            0xc2, 0x2d, 0xb7, 0x57, 0x74, 0xb9, 0x13, 0x13, 0xfc, 0xc5, 0xa7, 0x9a, 0x05, 0x2f, 0xc9,
                                            0x7a, 0x2e, 0x67, 0x85, 0xee, 0xd9, 0x79, 0x93, 0x2f, 0xd5, 0xb2, 0xd9, 0x12, 0xe4, 0xd1,
                                            0x8b, 0x0b, 0x98, 0x57, 0xb8, 0x36, 0xbe, 0xab, 0x4c, 0xe9, 0xf6, 0x54, 0x31, 0xac, 0xc7,
                                            0x6c, 0x19, 0xc5, 0x2a, 0x0c, 0xc6, 0x26, 0x8e, 0xca, 0x68, 0x36, 0x3c, 0x14, 0x31, 0x62,
                                            0x65, 0x62, 0x9c, 0xff, 0x93, 0xe8, 0x87, 0x9b, 0xad, 0xbe, 0x8c, 0x2e, 0x1f, 0x49, 0x2c,
                                            0x5a, 0x25, 0xe9, 0x44, 0x9b, 0xce, 0x74, 0xc2, 0x74, 0x30, 0x81, 0x63, 0xd7, 0x98, 0x4a,
                                            0xe9, 0x71, 0x8d, 0x33, 0x01, 0x66, 0x47, 0xe2, 0xfb, 0x5e, 0x59, 0xaa, 0x6b, 0x73, 0x52,
                                            0x3e, 0xde, 0x85, 0xda, 0xdd, 0x58, 0xf9, 0xfc, 0x5b, 0x53, 0xf0, 0xfb, 0x93, 0x9e, 0x0b,
                                            0x84, 0xa5, 0x88, 0x45, 0xfa, 0xb5, 0x5b, 0xa5, 0x19, 0xac, 0x3c, 0x77, 0x8f, 0xfe, 0xe6,
                                            0xd3, 0x92, 0x33, 0xc1, 0xc0, 0x55, 0x29, 0xe2, 0x88, 0xe7, 0x7d, 0x47, 0x08, 0x12, 0x38,
                                            0xf2, 0x9b, 0x60, 0x88, 0xa7, 0x89, 0xf1, 0xc5, 0x13, 0x2a, 0xd7, 0x9f, 0x7a, 0xcb, 0xed,
                                            0x49, 0x74, 0x30, 0x3b, 0x28, 0x4b, 0xbd, 0xb4, 0x9f, 0x1b, 0x57, 0x4c, 0xc5, 0xbb, 0x09,
                                            0xf1, 0x28, 0xeb, 0xc7, 0x37, 0x13, 0x78, 0xdd, 0x51, 0xa8, 0xe2, 0xd6, 0x40, 0x9f, 0xde,
                                            0x9b, 0x36, 0xa9, 0x30, 0xd7, 0xc2, 0xe7, 0xca, 0x24, 0xdd, 0x5d, 0x5f, 0x9d, 0x68, 0xa2,
                                            0xc7, 0xbd, 0xe0, 0x43, 0xb3, 0x4b, 0x80, 0x5f, 0x3f, 0x8d, 0x52, 0x0b, 0x93, 0xfa, 0x69,
                                            0xc0, 0x2a, 0x31, 0x9a, 0x35, 0x8d, 0xb9, 0xd2, 0xe3, 0x56, 0xad, 0x3c, 0x27, 0xbc, 0x30,
                                            0xc5, 0x03, 0x63, 0x99, 0x6c, 0x1c, 0xa4, 0x31, 0xab, 0x90, 0x46, 0xde, 0x21, 0x84, 0xac,
                                            0xa4, 0x70, 0x4e, 0x5d, 0xe8, 0x81, 0x41, 0x27, 0x0f, 0xef, 0xaa, 0x8c, 0x83, 0x27, 0x0a,
                                            0xc9, 0x0e, 0xb3, 0xdf, 0xd0, 0xb1, 0xa5, 0x7a, 0x7e, 0x6d, 0xc0, 0xb3, 0x54, 0x99, 0xe1,
                                            0xa0, 0x73, 0xf1, 0xbb, 0x64, 0x5e, 0xad, 0xd3, 0x2c, 0xb9, 0xb8, 0xdb, 0xab, 0x2f, 0xfd,
                                            0x33, 0x9b, 0xd1, 0xa1, 0x31, 0x4e, 0x15, 0x0c, 0x0b, 0xd8, 0xee, 0xed, 0x12, 0x44, 0xf1,
                                            0xb3, 0xb8, 0x77, 0x31, 0x55, 0x7b, 0xc3, 0x46, 0x91, 0xaa, 0x47, 0xb0, 0xfc, 0xac, 0xca,
                                            0xf0, 0x52, 0xa3, 0x5b, 0xed, 0xa2, 0x87, 0x5f, 0x99, 0x85, 0xe4, 0xed, 0xea, 0x6c, 0xae,
                                            0xde, 0x81, 0xbb, 0xdc, 0xbd, 0x26, 0x7c, 0xf9, 0x81, 0x2e, 0x78, 0x5b, 0x8e, 0x72, 0x75,
                                            0xbf, 0x0f, 0x7b, 0x28, 0x5b, 0x58, 0x51, 0xe3, 0x57, 0xf3, 0xf3, 0x68, 0x03, 0xff, 0x27,
                                            0xa7, 0x32, 0x3a, 0x8a, 0xb9, 0xc4, 0xb1, 0xe1, 0x25, 0x75, 0xaf, 0x50, 0xcb, 0x10, 0xc4,
                                            0x5c, 0x59, 0x22, 0x20, 0xaf, 0xd0, 0xc1, 0xbd, 0x5c, 0x3c, 0x15, 0x51, 0x22, 0xb0, 0xaf,
                                            0x46, 0x9c, 0xf0, 0x76, 0xb6, 0xde, 0xe3, 0x0a, 0x40, 0xed, 0x91, 0x38, 0xe7, 0xa9, 0xc3,
                                            0xd8, 0xb2, 0xa2, 0x15, 0x28, 0x36, 0x1c, 0xe4, 0xc0, 0x8a, 0x95, 0x17, 0xdd, 0x7e, 0xfc,
                                            0x21, 0x74};

uint8_t RSA4096_mod[512]               = {  0xd1, 0xea, 0x52, 0xbb, 0xdb, 0xd6, 0xe7, 0x09, 0x79, 0x51, 0x26, 0xe7, 0x49, 0xa9, 0xef,
                                            0xa3, 0x47, 0x9f, 0x92, 0xd6, 0xf1, 0xb4, 0x45, 0x5e, 0x39, 0xe4, 0xb6, 0x8a, 0xcd, 0xf9,
                                            0xff, 0x43, 0xf2, 0x2e, 0xc1, 0xc4, 0x02, 0xd5, 0x1f, 0x7c, 0x03, 0x03, 0xaa, 0xe6, 0x0b,
                                            0x63, 0x28, 0xe7, 0xc1, 0x50, 0x80, 0xcc, 0xe7, 0x86, 0xe5, 0xec, 0x31, 0x1d, 0x6b, 0x7b,
                                            0x22, 0x62, 0x8d, 0x62, 0x40, 0x7d, 0xdd, 0x2c, 0x97, 0xd4, 0x9f, 0xef, 0xff, 0x0d, 0x14,
                                            0xc2, 0x2d, 0xb7, 0x57, 0x74, 0xb9, 0x13, 0x13, 0xfc, 0xc5, 0xa7, 0x9a, 0x05, 0x2f, 0xc9,
                                            0x7a, 0x2e, 0x67, 0x85, 0xee, 0xd9, 0x79, 0x93, 0x2f, 0xd5, 0xb2, 0xd9, 0x12, 0xe4, 0xd1,
                                            0x8b, 0x0b, 0x98, 0x57, 0xb8, 0x36, 0xbe, 0xab, 0x4c, 0xe9, 0xf6, 0x54, 0x31, 0xac, 0xc7,
                                            0x6c, 0x19, 0xc5, 0x2a, 0x0c, 0xc6, 0x26, 0x8e, 0xca, 0x68, 0x36, 0x3c, 0x14, 0x31, 0x62,
                                            0x65, 0x62, 0x9c, 0xff, 0x93, 0xe8, 0x87, 0x9b, 0xad, 0xbe, 0x8c, 0x2e, 0x1f, 0x49, 0x2c,
                                            0x5a, 0x25, 0xe9, 0x44, 0x9b, 0xce, 0x74, 0xc2, 0x74, 0x30, 0x81, 0x63, 0xd7, 0x98, 0x4a,
                                            0xe9, 0x71, 0x8d, 0x33, 0x01, 0x66, 0x47, 0xe2, 0xfb, 0x5e, 0x59, 0xaa, 0x6b, 0x73, 0x52,
                                            0x3e, 0xde, 0x85, 0xda, 0xdd, 0x58, 0xf9, 0xfc, 0x5b, 0x53, 0xf0, 0xfb, 0x93, 0x9e, 0x0b,
                                            0x84, 0xa5, 0x88, 0x45, 0xfa, 0xb5, 0x5b, 0xa5, 0x19, 0xac, 0x3c, 0x77, 0x8f, 0xfe, 0xe6,
                                            0xd3, 0x92, 0x33, 0xc1, 0xc0, 0x55, 0x29, 0xe2, 0x88, 0xe7, 0x7d, 0x47, 0x08, 0x12, 0x38,
                                            0xf2, 0x9b, 0x60, 0x88, 0xa7, 0x89, 0xf1, 0xc5, 0x13, 0x2a, 0xd7, 0x9f, 0x7a, 0xcb, 0xed,
                                            0x49, 0x74, 0x30, 0x3b, 0x28, 0x4b, 0xbd, 0xb4, 0x9f, 0x1b, 0x57, 0x4c, 0xc5, 0xbb, 0x09,
                                            0xf2, 0xf9, 0xce, 0xe3, 0xcb, 0x79, 0x76, 0x0f, 0x93, 0x8e, 0x0e, 0x94, 0x1b, 0xda, 0x5c,
                                            0xd0, 0x09, 0xdd, 0xd3, 0x39, 0xe7, 0xbd, 0x38, 0x42, 0xf1, 0xe1, 0x69, 0xc9, 0xe0, 0xda,
                                            0x78, 0xc5, 0xef, 0x28, 0x75, 0x86, 0xda, 0xa8, 0xf5, 0x37, 0xb9, 0xac, 0x1e, 0xe5, 0xd9,
                                            0x41, 0xb6, 0x85, 0x1d, 0xe6, 0x60, 0x63, 0xb3, 0x2e, 0xdb, 0x39, 0xba, 0x70, 0xef, 0x89,
                                            0xe3, 0x34, 0xce, 0x4b, 0x65, 0xd0, 0x85, 0xae, 0x9e, 0x13, 0x1b, 0x4b, 0x38, 0x36, 0xcc,
                                            0xce, 0xfb, 0xa9, 0xb5, 0x70, 0x79, 0x53, 0xc8, 0xb4, 0x51, 0x40, 0x9d, 0xd9, 0x85, 0x9d,
                                            0x59, 0xbb, 0xe7, 0x48, 0x0f, 0x5f, 0xa6, 0x4c, 0xc4, 0x3d, 0x20, 0x81, 0x93, 0x48, 0xcd,
                                            0x65, 0x31, 0xd8, 0x46, 0xff, 0xfb, 0xae, 0x9b, 0xae, 0x9b, 0x14, 0x45, 0x19, 0xea, 0x3c,
                                            0x38, 0x33, 0x90, 0xe2, 0xa7, 0xdf, 0x83, 0xaa, 0x06, 0x94, 0x86, 0x93, 0xdf, 0xfd, 0xdb,
                                            0xd9, 0x66, 0xd5, 0x4c, 0xce, 0x2f, 0x5a, 0x71, 0xce, 0x8b, 0xd0, 0xf4, 0xdb, 0x16, 0x6b,
                                            0x32, 0xd9, 0x8a, 0xfc, 0x65, 0xf1, 0x49, 0x5e, 0x53, 0x4a, 0x7a, 0xe7, 0x1f, 0x65, 0x41,
                                            0x2c, 0x27, 0x94, 0xde, 0x38, 0x69, 0x85, 0xf5, 0x74, 0x20, 0xc0, 0x2f, 0xc7, 0x67, 0x10,
                                            0xb2, 0x0a, 0xac, 0x23, 0xd8, 0xd8, 0x1e, 0x5d, 0x96, 0xaa, 0x5f, 0x21, 0xae, 0x9c, 0xd0,
                                            0x36, 0xa8, 0x9d, 0x49, 0xee, 0x70, 0x2e, 0x4b, 0xb1, 0x6d, 0xa4, 0xb9, 0xb8, 0x34, 0xd5,
                                            0xdc, 0x3f, 0x07, 0x73, 0xe4, 0xaa, 0xeb, 0xf6, 0xf0, 0x3b, 0x24, 0x3d, 0x40, 0xf9, 0x41,
                                            0x8c, 0xd3, 0xe5, 0x15, 0x01, 0x51, 0xbe, 0x75, 0x47, 0x33, 0xe7, 0x57, 0xf3, 0xe4, 0xb4,
                                            0x3b, 0x9f, 0x72, 0x7b, 0x5a, 0xcd, 0xca, 0x80, 0x57, 0xc6, 0x9f, 0x68, 0xcd, 0x8c, 0x01,
                                            0x7f, 0xf1};

uint8_t check_data_out4096[512]      = {   0x00, 0x02, 0x04, 0x7e, 0x89, 0xff, 0xd1, 0xf9, 0xdc, 0x23, 0xfd, 0x24, 0x52,
                                            0x70, 0xce, 0x4a, 0x69, 0x96, 0xd5, 0x70, 0x25, 0x2d, 0xb2, 0x69, 0xdf, 0x4e,
                                            0xef, 0x86, 0x7b, 0x86, 0xab, 0x39, 0x0c, 0x5c, 0xd0, 0xad, 0x03, 0xe7, 0xb9,
                                            0xf8, 0x9e, 0xb5, 0x5d, 0x05, 0xa7, 0xbf, 0x2c, 0xf3, 0x32, 0x2d, 0x3e, 0xeb,
                                            0xe5, 0xdf, 0x64, 0xc0, 0x14, 0x19, 0xf5, 0x8f, 0x36, 0x0c, 0x97, 0x76, 0x8d,
                                            0x1c, 0xe0, 0x7b, 0x4b, 0x41, 0xf7, 0x1e, 0x8e, 0xb6, 0xf7, 0x3d, 0x77, 0xc4,
                                            0x79, 0x5f, 0xa0, 0x24, 0x0a, 0x20, 0xb1, 0x23, 0xba, 0x22, 0xb0, 0xf6, 0x7b,
                                            0x71, 0x63, 0xe5, 0xc6, 0x19, 0x70, 0xa3, 0x79, 0xf1, 0x32, 0xbb, 0x21, 0x3b,
                                            0xd9, 0x15, 0x2d, 0xac, 0xda, 0xd7, 0x0f, 0x01, 0xb3, 0x77, 0xc9, 0x92, 0x23,
                                            0xd8, 0x34, 0x5e, 0xb1, 0xab, 0xc4, 0x13, 0x38, 0x3e, 0xab, 0x60, 0x50, 0x45,
                                            0x9d, 0x87, 0x53, 0xb3, 0x42, 0x9b, 0xc1, 0x45, 0x53, 0x5f, 0x68, 0x04, 0x30,
                                            0x24, 0x92, 0xaf, 0x39, 0xc3, 0x86, 0xca, 0x7f, 0x8c, 0x94, 0x94, 0x47, 0x75,
                                            0xf4, 0xc9, 0x12, 0x1e, 0xbf, 0x7a, 0xc9, 0xd4, 0x60, 0x01, 0x52, 0x6f, 0x4d,
                                            0x2a, 0x4d, 0x24, 0x76, 0xe7, 0x31, 0x70, 0x73, 0xd9, 0x22, 0xda, 0xdb, 0x7e,
                                            0x3e, 0xfc, 0x25, 0xe6, 0x0d, 0x02, 0xec, 0x0f, 0x9c, 0xc4, 0xf2, 0x84, 0x8e,
                                            0x91, 0x0b, 0xff, 0xf1, 0x41, 0x39, 0x84, 0xd2, 0xfd, 0x9a, 0xa3, 0x2e, 0x6c, /* decrypted_data (RSAES-PKCS1-v1_5_padding + message ""Hello world" )*/
                                            0x67, 0xaf, 0xaf, 0xe0, 0x93, 0xa0, 0x3d, 0xd5, 0x65, 0xd4, 0x0e, 0x56, 0xba, /* is the result of the openssl command: */
                                            0xbd, 0x49, 0xe1, 0xb3, 0xfd, 0xf4, 0x27, 0xe9, 0xce, 0x84, 0xae, 0x0c, 0xe6, /* $ openssl pkeyutl -decrypt  -inkey privkey4096.pem -in encrypt_with_pub.txt -out decryption_result.txt */
                                            0xd3, 0xda, 0x55, 0x4d, 0x28, 0x9c, 0x0e, 0x1e, 0xc5, 0xf0, 0x3d, 0x73, 0x26,
                                            0x96, 0x83, 0x8a, 0x5f, 0x63, 0xe3, 0x41, 0xe4, 0xe1, 0xa2, 0xd6, 0xc8, 0x38,
                                            0xd2, 0x39, 0x13, 0x76, 0xc4, 0x87, 0x3a, 0x01, 0xd2, 0x29, 0xf4, 0x65, 0xef,
                                            0x88, 0xc9, 0x75, 0x9e, 0x49, 0x8c, 0x47, 0xfe, 0x14, 0x7e, 0xd2, 0xe3, 0xa8,
                                            0x68, 0x10, 0x31, 0x31, 0xde, 0xa0, 0xc3, 0x8b, 0x92, 0xa7, 0x86, 0x3a, 0x61,
                                            0x74, 0x87, 0x33, 0xdb, 0xe7, 0x94, 0x58, 0x14, 0x86, 0xae, 0x3f, 0xb0, 0x41,
                                            0x81, 0x6c, 0x49, 0x23, 0x0c, 0xe4, 0xf2, 0x8b, 0x22, 0xd3, 0x3c, 0x7e, 0xfb,
                                            0xc3, 0x04, 0xc6, 0x21, 0x53, 0x34, 0xb6, 0xc6, 0x30, 0x90, 0xaa, 0x4a, 0x26,
                                            0xe9, 0xdd, 0xe6, 0x34, 0xc3, 0x02, 0xa1, 0xe6, 0xcf, 0x83, 0xf0, 0x97, 0x35,
                                            0x16, 0x6f, 0x85, 0x7a, 0x96, 0xaf, 0x1a, 0x8b, 0x67, 0xe4, 0xd9, 0xf8, 0x7a,
                                            0xd6, 0x7f, 0x3b, 0x50, 0x3e, 0x9e, 0xdd, 0x7f, 0x14, 0xfe, 0xfb, 0xe9, 0x45,
                                            0xf7, 0xfc, 0x14, 0xb5, 0xe7, 0x4f, 0xe2, 0x94, 0x13, 0x8a, 0xb1, 0xe7, 0x97,
                                            0xab, 0x48, 0xa0, 0xbd, 0x4c, 0xaa, 0x5c, 0x92, 0xfa, 0x0c, 0xd0, 0xb4, 0x56,
                                            0xea, 0xa7, 0x65, 0xaa, 0x04, 0x46, 0x23, 0x1d, 0x3f, 0x44, 0xec, 0x70, 0xfa,
                                            0x21, 0xa6, 0xcb, 0xe2, 0xfb, 0x0a, 0xa3, 0xe7, 0xce, 0x76, 0x47, 0x10, 0xf3,
                                            0xa8, 0x60, 0x06, 0x05, 0xf0, 0x59, 0xee, 0x9b, 0x17, 0x40, 0x92, 0x7e, 0x1b,
                                            0xd6, 0x65, 0xd4, 0x9c, 0x84, 0xd1, 0x6b, 0x60, 0xd5, 0x58, 0x01, 0xa1, 0xef,
                                            0x20, 0xf0, 0x93, 0x5b, 0x22, 0x7a, 0x44, 0xe6, 0xdb, 0xdd, 0x4d, 0x38, 0x0e,
                                            0xfb, 0xbd, 0x0e, 0xf3, 0x8e, 0x19, 0x0b, 0x41, 0x11, 0x2f, 0x1f, 0x28, 0x51,
                                            0xc2, 0x61, 0x5b, 0x70, 0xeb, 0xcc, 0xdd, 0x58, 0x8c, 0x78, 0xd4, 0x7b, 0xd9,
                                            0xec, 0x78, 0x71, 0xa1, 0x42, 0x36, 0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
                                            0x77, 0x6f, 0x72, 0x6c, 0x64};

const uint32_t RSA4096_exp_length       = 512;
const uint32_t RSA4096_mod_length       = 512;

/* Output buffers */
uint32_t IV_Buffer[4] ;
uint32_t TAG_Buffer[4];
uint32_t phi_encrypted_4096[128]          = {0};
uint32_t exponent_encrypted_4096[128]     = {0};
uint8_t RSA_result_4096[512]              = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_CCB_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  if (BSP_ERROR_NONE != BSP_LED_Init(LED_GREEN))
  {
    Error_Handler();
  }
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_CCB_Init();
  /* USER CODE BEGIN 2 */

 /* Configure Wrapped Key */
  wrappedKeyConf.WrappingKeyType         = HAL_CCB_USER_KEY_HW;

  /* Fill RSA Modular exponentiation In parameters */
  RSAKey.pExp                            = RSA4096_exp_P;
  RSAKey.pPhi                            = RSA4096_phi_P;

  /* Fill RSA Modular exponentiation In parameters */
  RSAparam.expSizeByte                   = RSA4096_exp_length;
  RSAparam.modulusSizeByte               = RSA4096_mod_length;
  RSAparam.pMod                          = RSA4096_mod;

  /* Prepare RSA Modular exponentiation parameters  */
  RSAKeyBlob.pIV                         = IV_Buffer;
  RSAKeyBlob.pTag                        = TAG_Buffer;
  RSAKeyBlob.pWrappedExp                 = exponent_encrypted_4096;
  RSAKeyBlob.pWrappedPhi                 = phi_encrypted_4096;

  if ((HAL_CCB_RSA_WrapPrivateKey(&hccb, &RSAparam, &RSAKey, &wrappedKeyConf, &RSAKeyBlob) != HAL_OK) || (hccb.State != HAL_CCB_STATE_READY))
  {
    Error_Handler();
  }

  /* decrypt the encrypted "hello world" + PKCS1.5 padding which is RSA4096_expBas(already encrypted by public key on openssl and before CCB usage) */
  if ( HAL_CCB_RSA_ComputeModularExp(&hccb, &RSAparam, &wrappedKeyConf, &RSAKeyBlob , RSA4096_expBase, RSA_result_4096) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_CCB_DeInit(&hccb) != HAL_OK)
  {
    Error_Handler();
  }

  /* check_data_out4096 is thye plain "hello world" + PKCS1.5 padding */
  if(memcmp(RSA_result_4096, check_data_out4096, 512) != 0)
  {
    while (1)
    {
      /* Toggle green led for error */
      (void) BSP_LED_Toggle(LED_GREEN);
      HAL_Delay(500);
    }
  }

  if (HAL_CCB_DeInit(&hccb) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    BSP_LED_On(LED_GREEN);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Enable Epod Booster
  */
  if (HAL_RCCEx_EpodBoosterClkConfig(RCC_EPODBOOSTER_SOURCE_MSIS, RCC_EPODBOOSTER_DIV1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_PWREx_EnableEpodBooster() != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Set Flash latency before increasing MSIS
  */
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CCB Initialization Function
  * @param None
  * @retval None
  */
static void MX_CCB_Init(void)
{

  /* USER CODE BEGIN CCB_Init 0 */

  /* USER CODE END CCB_Init 0 */

  /* USER CODE BEGIN CCB_Init 1 */

  /* USER CODE END CCB_Init 1 */
  hccb.Instance = CCB;
  if (HAL_CCB_Init(&hccb) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CCB_Init 2 */

  /* USER CODE END CCB_Init 2 */

}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
    /* Toggle green led for error */
    (void) BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(500);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
