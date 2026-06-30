/**
  ******************************************************************************
  * @file    app_main.h
  * @author  MCD Application Team
  * @brief   Main application (root menu) interface header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#undef GLOBAL
#ifdef __APP_MAIN_C
#define GLOBAL
#else
#define GLOBAL extern
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
/* External variables --------------------------------------------------------*/
extern TSC_HandleTypeDef htsc;
/* Exported constants --------------------------------------------------------*/
#define DEMO_NAME_CHAR_NB         20
#define MODE_NAME_CHAR_NB         15
/* Exported types ------------------------------------------------------------*/

/* USER CODE BEGIN ET */
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[DEMO_NAME_CHAR_NB];
  uint32_t DemoIndex;
}Demo_Typedef;

typedef struct
{
  void   (*ModeFunc)(void);
  uint8_t ModeName[MODE_NAME_CHAR_NB];
  uint32_t ModeIndex;
  uint8_t Message[6];
}StopModes_Typedef;

typedef enum {
  MODULE_OK,
  /* Module exec */
  MODULE_ERROR_PRE,
  MODULE_ERROR_EXEC,
  MODULE_ERROR_POST,
  MODULE_RESMISSIG
} MODULE_RETURN;

typedef enum
{
  TMODE_CELSIUS     = 0x00U,
  TMODE_FAHRENHEIT  = 0x01U,
} Temp_ModeTypedef;

typedef enum
{
  DISP_TEMP = 0,
  DISP_HUMI
} DisplayMode_t;

/* USER CODE END ET */
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define nINT_Pin GPIO_PIN_4
#define nINT_GPIO_Port GPIOB
#if defined (USE_STM32U366E_DISCO)
#define NWAKE_Pin GPIO_PIN_15
#define NWAKE_GPIO_Port GPIOA
#endif /* USE_STM32U366E_DISCO */
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA


/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)       (sizeof(x)/sizeof(Demo_Typedef))
#define COUNT_OF_MODES(x)         (sizeof(x)/sizeof(StopModes_Typedef))
#define ABS(x)                    (x < 0) ? (-x) : x
/* Exported functions prototypes ---------------------------------------------*/
void LPModes_demo(void);
void TouchWakeup_demo(void);
void TempHumiSensor_demo(void);
void Demo_Start(void);
void Menu_Init(void);
void Display_DemoDescription(void);
void SystemHardwareInit(void);
void SystemHardwareDeInit(void);

void Sleep_Mode(void);
void Stop2_Mode(void);
void Stop3_Mode(void);
void Standby_Mode(void);
void LpSleep_Mode(void);

void SystemPower_Config(void);