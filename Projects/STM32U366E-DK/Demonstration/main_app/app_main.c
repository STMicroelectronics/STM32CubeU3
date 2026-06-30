/**
  ******************************************************************************
  * @file    app_main.c
  * @author  MCD Application Team
  * @brief   Main application (root menu) implementation
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

/* Includes ------------------------------------------------------------------*/
#include "app_main.h"
#include "touchsensing.h"
#include "tsl.h"

/* Exported types ------------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hbus_i2c1;
extern RTC_HandleTypeDef hrtc;

/* Private constants ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void Format_TempHumi(float value, uint16_t *tab);
static void SystemClock_3MHz(void);
void PB_Config(void);
void JOY_Config(void);
void SystemPeripheralClockDisable(void);

/* Private Variable ----------------------------------------------------------*/
/* Menu / Application management */
uint32_t AppStatus = 0x00;
FlagStatus AppLaunched = RESET;
uint8_t DemoIndex = 0;
uint8_t ModeIndex = 0;

JOYPin_TypeDef JoyState = JOY_NONE;
int32_t Joy_State;

uint32_t joystickstatus = JOY_NONE;

__IO uint8_t JoystickValue = 0;

/* initialization variable */
__IO FlagStatus KeyPressed = RESET;
FlagStatus LPModesInitialized     = RESET;
FlagStatus TouchWakeupInitialized = RESET;
FlagStatus TempHumiInitialized    = RESET;
FlagStatus JoyInitialized         = RESET;
FlagStatus LedInitialized         = RESET;
FlagStatus LcdInitialized         = RESET;

uint8_t msg[32];
uint8_t buf_rx[8];
uint8_t buf_tx[8];

int32_t Value_Temp;
int32_t Value_Humi;
float Fahrenheit_Temp;
uint8_t Id_Env;
uint16_t tab[6];

static __IO uint32_t TimingDelay;
static DisplayMode_t DisplayMode = DISP_TEMP;

uint8_t ModeStatus = 0;

/* TSC variable and define */
extern uint32_t idx_bank_it;
extern uint32_t config_done_it;
extern uint32_t acq_done_it;

#define TKEY_DET(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_DETECT)
#define TKEY_PRX(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_PROX)
#define TKEY_REL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_RELEASE)
#define TKEY_CAL(NB) (MyTKeys[(NB)].p_Data->StateId == TSL_STATEID_CALIB)
extern __IO TSL_tTick_ms_T ECSLastTick;

extern TSL_TouchKeyParam_T MyTKeys_Param[];
uint32_t uwTSC_WakeupDetected = 0;

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  DEMO_STATE_TEMP_HUMI,                 /*!< TEMP and Humidity Sensor application launched */
  DEMO_STATE_TOUCH_WAKEUP,              /*!< Touch Wakeup application launched */
  DEMO_STATE_LOW_POWER_MODES,           /*!< Low Power Modes application launched */
  DEMO_STATE_FIRST_STARTUP,             /*!< Demo first Startup */
  DEMO_STATE_IDLE,                      /*!< Demo in idle */
} Demo_StateTypeDef;

Demo_Typedef  MainMenuItems[]=
{
  {TempHumiSensor_demo, " TEMHU", DEMO_STATE_TEMP_HUMI},
  {TouchWakeup_demo, " TOUCH", DEMO_STATE_TOUCH_WAKEUP},
  {LPModes_demo, "  ULP ", DEMO_STATE_LOW_POWER_MODES}
};

StopModes_Typedef  StopModesItems[]=
{
  {Standby_Mode, " STDBY", 0,"   2uA" },
  {Sleep_Mode, " SLEEP", 1,"188uA" },
  {LpSleep_Mode, "LP SLP", 2,"185uA" },
  {Stop2_Mode, " STOP2", 4,"15uA" },
  {Stop3_Mode, " STOP3", 3,"14uA" }
};

/* Private define ------------------------------------------------------------*/
#define LED_TOGGLE_DELAY         100

/* External variables --------------------------------------------------------*/


/* Private function ----------------------------------------------------------*/

void Demo_Start(void)
{
  uint8_t i = 0;
  uint8_t menudisplay[DEMO_NAME_CHAR_NB + 5] = {0};

  AppStatus = DEMO_STATE_FIRST_STARTUP;
  
  /* Menu display first characters initialization */
  for(i = 0; i < 5; i++)
  {
    menudisplay[i] = ' ';
  }

  while(1)
  {
    if(TAMP->BKP0R == 1U)
    {
      DemoIndex = TAMP->BKP2R;
      ModeIndex = TAMP->BKP1R;
      MainMenuItems[DemoIndex].DemoFunc();
    }
    for(i = 0; i < DEMO_NAME_CHAR_NB; i++)
      {
        menudisplay[i] = MainMenuItems[DemoIndex].DemoName[i];
      }

      BSP_LCD_GLASS_DisplayString(menudisplay);
      HAL_Delay(200);

    while (joystickstatus  == JOY_NONE)
    {
      /* Get JoyStick status */
      joystickstatus  = BSP_JOY_GetState(JOY1);
      HAL_Delay(200);
    }

    if (joystickstatus  == JOY_UP)
    {
      DemoIndex++;
      if (DemoIndex >= COUNT_OF_EXAMPLE(MainMenuItems))
      {
         DemoIndex = 0U;
      }
    }

    if (joystickstatus  == JOY_DOWN)
    {
      DemoIndex--;
      if (DemoIndex >= COUNT_OF_EXAMPLE(MainMenuItems))
      {
        DemoIndex = (COUNT_OF_EXAMPLE(MainMenuItems) - 1);
      }
    }

    if ((joystickstatus  == JOY_SEL) || (joystickstatus  == JOY_RIGHT))
    {
      JoystickValue++;
      if (JoystickValue > 2)
      {
        JoystickValue = 1;
      }
      /* Set Application status in case of specific joystick usage */
      AppStatus = MainMenuItems[DemoIndex].DemoIndex;

      /* launched application */
      MainMenuItems[DemoIndex].DemoFunc();

        /* Set Application status to Idle */
        AppStatus = DEMO_STATE_IDLE;

        joystickstatus = JOY_NONE;
    }
    joystickstatus  = JOY_NONE;
  }

}


void LPModes_demo(void)
{
  uint8_t modedisplay[MODE_NAME_CHAR_NB + 5] = {0};
  uint8_t i = 0;
  uint32_t tickstart;


   /* Menu display first characters initialization */
  for(i = 0; i < 5; i++)
  {
    modedisplay[i] = ' ';
  }
  ModeStatus = 0;
  joystickstatus  = JOY_NONE;

  /* Ensure that MSI is wake-up system clock */ 
  HAL_RCCEx_StopWakeupSysclkConfig(RCC_STOP_WKUP_SYSCLK_MSIS);

  /* Configure the system Power */
  SystemPower_Config();

  /* Turn off LEDs */
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_BLUE);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_DeInit(LED_RED);
  BSP_LED_DeInit(LED_GREEN);

  ModeIndex = 0;
  while(1)
  {
    if(TAMP->BKP0R == 1U)
    {
      DemoIndex = TAMP->BKP2R;
      ModeIndex = TAMP->BKP1R;
      TAMP->BKP0R = 0U;
    }
    for(i = 0; i < MODE_NAME_CHAR_NB; i++)
      {
        modedisplay[i] = StopModesItems[ModeIndex].ModeName[i];
      }
      ModeStatus = 2;
      BSP_LCD_GLASS_DisplayString(modedisplay);
      HAL_Delay(500);

    /* Ensure a small tempo between images display unless a user action occurs */
    while (joystickstatus  == JOY_NONE)
    {
      /* Get JoyStick status */    
      joystickstatus  = BSP_JOY_GetState(JOY1);
      HAL_Delay(50);
      
    }
    if(joystickstatus  == JOY_UP)
    {
      ModeIndex++;
      if (ModeIndex >= COUNT_OF_MODES(StopModesItems))
      {
        ModeIndex = 0U;
      }
      ModeStatus = 1; 
    }

    if (joystickstatus  == JOY_DOWN)
    {
      ModeIndex--;
      if (ModeIndex >= COUNT_OF_MODES(StopModesItems))
      {
        ModeIndex = (COUNT_OF_MODES(StopModesItems) - 1);
      }
      ModeStatus = 2;
    }

    if ((joystickstatus  == JOY_SEL) || (joystickstatus  == JOY_RIGHT))
    {      
      BSP_LED_Off(LED_BLUE);
      /* Disable all used wakeup source */
      HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

      /* Re-enable wakeup source */
      /* ## Setting the Wake up time ############################################*/
      /* RTC Wakeup Interrupt Generation: 
      the wake-up counter is set to its maximum value to yield the longest
      stop time to let the current reach its lowest operating point.
      The maximum value is 0xFFFF, corresponding to about 33 sec. when 
      RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_Div16 = 16  */

      /* Wakeup Time Base = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE))
      Wakeup Time = Wakeup Time Base * WakeUpCounter 
      = (RTC_WAKEUPCLOCK_RTCCLK_DIV /(LSE)) * WakeUpCounter
      ==> WakeUpCounter = Wakeup Time / Wakeup Time Base   */

      /* To configure the wake up timer to 33s the WakeUpCounter is set to 0xFFFF:
      Wakeup Time Base = 16 /(~32 kHz RC) = ~0.5 ms
      Wakeup Time = 0.5 ms  * WakeUpCounter
      Therefore, with wake-up counter =  0xFFFF  = 65,535 
      Wakeup Time =  0.5 ms *  65,535 = ~ 33 sec. */
      HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0xFFFF, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0);

      /* Suspend Systick */
      HAL_SuspendTick();

      StopModesItems[ModeIndex].ModeFunc();
      
      /* ... Resume Tick ... */
      HAL_ResumeTick();

      /* Re-configure LED3  */
      BSP_LED_Init(LED3);

      /* Configure the Joystick */
      JOY_Config();
      TAMP->BKP0R = 0U;
    }

    JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
    tickstart = HAL_GetTick();
    while (JoyState == JOY_LEFT) 
    {
      JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
      if ((HAL_GetTick() - tickstart) > 250)
      {
        TAMP->BKP0R = 0U;
        return;
      }
    }

    joystickstatus  = JOY_NONE;
  }
  
}

void LpSleep_Mode(void)
{
  tab[0] = '1';
  tab[1] = '8';
  tab[2] = '5';
  tab[3] = ' ';
  tab[4] = u'Âµ';
  tab[5] = 'A';
  BSP_LCD_GLASS_DisplayStrDeci(tab);

  SystemClock_3MHz();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_RTCAPB_CLK_SLEEP_ENABLE();
  HAL_NVIC_ClearPendingIRQ(RTC_IRQn);
  HAL_SuspendTick();
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void Sleep_Mode(void)
{
  tab[0] = '1';
  tab[1] = '8';
  tab[2] = '8';
  tab[3] = ' ';
  tab[4] = u'Âµ';
  tab[5] = 'A';
  BSP_LCD_GLASS_DisplayStrDeci(tab);

  SystemClock_3MHz();
  __HAL_RCC_PWR_CLK_ENABLE();
  
  HAL_SuspendTick();
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void Stop3_Mode(void)
{
  tab[0] = ' ';
  tab[1] = '1';
  tab[2] = '4';
  tab[3] = ' ';
  tab[4] = u'Âµ';
  tab[5] = 'A';
  BSP_LCD_GLASS_DisplayStrDeci(tab);

  SystemClock_3MHz();
  /* Clear Stop Flag if returned from Stop3 mode */
  if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOPF))
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOPF);
  }

  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP3, PWR_STOPENTRY_WFI);
}

void Stop2_Mode(void)
{
  tab[0] = ' ';
  tab[1] = ' ';
  tab[2] = '1';
  tab[3] = '5';
  tab[4] = u'Âµ';
  tab[5] = 'A';
  BSP_LCD_GLASS_DisplayStrDeci(tab);

  SystemClock_3MHz();
  /* Clear Stop Flag if returned from Stop2 mode */
  if ( __HAL_PWR_GET_FLAG(PWR_FLAG_STOPF))
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOPF);
  }

  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP2, PWR_STOPENTRY_WFI);
}

void Standby_Mode(void)
{
  BSP_LCD_GLASS_DisplayString(StopModesItems[ModeIndex].Message);
  /* Clear StandBy Flag if returned from StandBy mode */
  if ( __HAL_PWR_GET_FLAG(PWR_FLAG_SBF) )
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SBF);
  }
  SystemClock_3MHz();
  __HAL_RCC_PWR_CLK_ENABLE();
  
  HAL_PWR_EnableBkUpAccess();

  TAMP->BKP0R = 1U;
  TAMP->BKP1R = ModeIndex;
  TAMP->BKP2R = DemoIndex;
  HAL_SuspendTick();
  HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  System Power Configuration
  *         The system Power is configured as follow:
  *            + VREFINT OFF, with fast wakeup enabled
  *            + No IWDG
  *            + Automatic Wakeup using RTC clocked by LSI (after ~5s)
  * @param  None
  * @retval None
  */
void SystemPower_Config(void)
{
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPowerMode();

  /* Enable the fast wake up from Ultra low power mode */
  HAL_PWR_EnableWakeUpLine(PWR_WAKEUP_LINE7, PWR_WAKEUP_SELECT_3, PWR_WAKEUP_POLARITY_LOW);
}

/**
  * @brief  Joystick Configuration
  * @param  None
  * @param  None
  * @retval None
  */
void JOY_Config(void)
{
  /* This function should be implemented by the user application.
  It is called into this driver when an event on Button is triggered. */

  /* Initialize the joystick pins*/
  BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);
  /* Set Joystick init flag */
  JoyInitialized = SET;
   __HAL_RCC_LCD_CLK_ENABLE();
   __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_ResumeTick();
  if (MainMenuItems[DemoIndex].DemoIndex == DEMO_STATE_LOW_POWER_MODES)
  {
    if (ModeStatus != 0)
    {
      if (TimingDelay != 0)
      {
        TimingDelay--;
      }
      else
      {
        /* Toggle LED3 */
        BSP_LED_Toggle(LED3);
        TimingDelay = LED_TOGGLE_DELAY;
      }
    }
  }
}

/**
  * @brief  TouchWakeup/Touchsensing Demonstration
  * @note  Goal of the demonstration is to detect a touch on Touchsensor button TS1
  * @param  None
  * @retval None
  */
void TouchWakeup_demo(void)
{
  uint32_t tickstart;
  uint32_t count_nb_acq = 2000;

  /* ShutDown the debugger to reduce current consumption */
  HAL_DBGMCU_DisableDBGStopMode();

  /* Display "RUN MODE" at startup */
  BSP_LCD_GLASS_Clear();
  HAL_Delay(500);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" TOUCH SETUP ", 2, SCROLL_SPEED_MEDIUM);

  /* Let TSC and TSL run for 5s to set up ECS */
  while (count_nb_acq)
  {
    if (tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY)
    {
      /* Restart TSLPRM_TOTAL_BANKS banks acquisition */
      idx_bank_it = 0;
      acq_done_it = 0;
      TSL_acq_BankConfig(idx_bank_it);
      TSL_acq_BankStartAcq_IT();
      count_nb_acq--;
    }
    else
    {
      HAL_Delay(1);
    }
  }

  BSP_LED_Off(LED1);
  BSP_LED_Off(LED2);
  BSP_LED_Off(LED3);

  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" TOUCH SETUP COMPLETE ", 2, SCROLL_SPEED_MEDIUM);

  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" RUN MODE", 2, SCROLL_SPEED_MEDIUM);

  /* Reduce the dischage time of the sampling capacitor */
  TSL_Globals.DelayDischarge = 0;

  /* Start RTC Timer for 250ms interval */
  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x1ff, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0x0);

  while(1)
  {
    /* Run TSC acquisition in blocking mode after wake up */
    if (tsl_user_Exec_IT() != TSL_USER_STATUS_BUSY)
    {
      int id;
      for (id=0; id < TSLPRM_TOTAL_CHANNELS; id++)
      {
        if (!TKEY_CAL(id))
        {
          if (TKEY_DET(id))
          {
            BSP_LCD_GLASS_Clear();
            BSP_LED_On(LED3);
            BSP_LCD_GLASS_ScrollSentence((uint8_t *)"WAKEUP TOUCH DETECTED", 1, SCROLL_SPEED_MEDIUM);
            HAL_Delay(5000);
            uwTSC_WakeupDetected = 1;
          }
          else if (TKEY_REL(id) && (uwTSC_WakeupDetected == 1))
          {
            BSP_LCD_GLASS_Clear();
            BSP_LED_Off(LED3);
            BSP_LCD_GLASS_ScrollSentence((uint8_t *)" STOP2 MODE ENTERING", 1, SCROLL_SPEED_MEDIUM);
            uwTSC_WakeupDetected = 0;
          }
        }
      }

      /* Go into stop 2 mode */
      HAL_SuspendTick();
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP2, PWR_STOPENTRY_WFI);
      HAL_ResumeTick();

      /* Clear Wakeup flag */
      SET_BIT(RTC->SCR,RTC_SCR_CWUTF);

      /* Wakeup and Restart TSLPRM_TOTAL_BANKS banks acquisition */
      idx_bank_it = 0;
      acq_done_it = 0;
      TSL_acq_BankConfig(idx_bank_it);
      TSL_acq_BankStartAcq_IT();
    }
    else
    {
      /* SuspendTick to reduce current consumption BUT ECS WILL NOT WORK */
      HAL_SuspendTick();
      /* Enter the CPU to SLEEP mode */
      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      HAL_ResumeTick();
    }

    JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
    tickstart = HAL_GetTick();
    while(JoyState == JOY_LEFT)
    {
      JoyState =  (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
      if ((HAL_GetTick() - tickstart) > 250)
      {
        return;
      }
    }
  }
}

/**
  * @brief  Temperature and humidity sensor Demonstration
  * @note  Goal of the demonstration is to measure and display temperature
  *        in degree Celsius and Fahrenheit.
  * @param  None
  * @retval None
  */
void TempHumiSensor_demo(void)
{
  uint32_t tickstart;
  Temp_ModeTypedef Temp_mode;

  BSP_COMBO_SENSOR_Init(COMBO_SENSOR_SHT40_0);
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)" ", 1, SCROLL_SPEED_HIGH);
  HAL_Delay(2000);
  Temp_mode = TMODE_CELSIUS;

  while (1)
  {
    joystickstatus = BSP_JOY_GetState(JOY1);

    if (joystickstatus == JOY_DOWN)
    {
      Temp_mode = TMODE_FAHRENHEIT;
    }
    else if (joystickstatus == JOY_UP)
    {
      Temp_mode = TMODE_CELSIUS;
    }
    else if (joystickstatus == JOY_SEL)
    {
      /* Toggle between temperature and humidity display */
      if (DisplayMode == DISP_TEMP)
      {
        DisplayMode = DISP_HUMI;
      }
      else
      {
        DisplayMode = DISP_TEMP;
      }
    }

    /* Read milli-°C and milli-%RH from BSP */
    BSP_COMBO_SENSOR_GetValues(COMBO_SENSOR_SHT40_0, &Value_Temp, &Value_Humi);

    float tempC = (float)Value_Temp / 1000.0f;
    float tempF = tempC * 9.0f / 5.0f + 32.0f;
    float rh    = (float)Value_Humi / 1000.0f;

    if (DisplayMode == DISP_HUMI)
    {
      /* Display humidity in percentage: XX.YYPT */
      Format_TempHumi(rh, tab);
      tab[4] = 'P';
      tab[5] = 'T';
    }
    else
    {
      /* Display temperature: °C or °F */
      if (Temp_mode == TMODE_FAHRENHEIT)
      {
        Format_TempHumi(tempF, tab);
        tab[4] = u'°';
        tab[5] = 'F';
      }
      else
      {
        Format_TempHumi(tempC, tab);
        tab[4] = u'°';
        tab[5] = 'C';
      }
    }

    BSP_LCD_GLASS_DisplayStrDeci(tab);
    HAL_Delay(200);

    /* exit on long LEFT press */
    joystickstatus = (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
    tickstart = HAL_GetTick();
    while (joystickstatus == JOY_LEFT)
    {
      joystickstatus = (JOYPin_TypeDef)BSP_JOY_GetState(JOY1);
      if ((HAL_GetTick() - tickstart) > 250)
      {
        return;
      }
    }
  }
}

/**
  * @brief  Display main demo messages
  * @retval None
  */
 void Display_DemoDescription(void)
{

  /* Clear the LCD */
  BSP_LCD_GLASS_Clear();

  /* Display LCD messages */
  BSP_LCD_GLASS_ScrollSentence((uint8_t *)"      STM32U366E-DK DEMO", 1, SCROLL_SPEED_MEDIUM);

  /* Introduce short delay before new message displayed */
  HAL_Delay(50);

  /* Display message */
  BSP_LCD_GLASS_DisplayString((uint8_t*)MainMenuItems[0].DemoName);
  HAL_Delay(1000);
  BSP_LCD_GLASS_DisplayString((uint8_t*)MainMenuItems[1].DemoName);
  HAL_Delay(1000);
  BSP_LCD_GLASS_DisplayString((uint8_t*)MainMenuItems[2].DemoName);
  HAL_Delay(1000);
}

/**
  * @brief  System Power Configuration at Boot
  * @retval None
  */
void SystemHardwareInit(void)
{
  /* Init LED1, LED2 and LED3  */
  if (LedInitialized != SET)
  {
    BSP_LED_Init(LED_BLUE);
    BSP_LED_Init(LED_RED);
    BSP_LED_Init(LED_GREEN);
    BSP_LED_On(LED_BLUE);
    BSP_LED_On(LED_RED);
    BSP_LED_On(LED_GREEN);
    LedInitialized = SET;
  }

  /* Init STM32U366E-DK joystick in interrupt mode */
  if (JoyInitialized != SET)
  {
    BSP_JOY_Init(JOY1, JOY_MODE_GPIO, JOY_ALL);
    JoyInitialized = SET;
  }

  /* Initialize the LCD */
  if (LcdInitialized != SET)
  {
    BSP_LCD_GLASS_Init();
    LcdInitialized = SET;
  }

  /* Initialize the TempSensor */
  if (TempHumiInitialized != SET)
  {
    BSP_COMBO_SENSOR_Init(COMBO_SENSOR_SHT40_0);
    TempHumiInitialized = SET;
  }

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
}


/**
  * @brief  System Low Power Configuration
  * @retval None
  */
void SystemHardwareDeInit(void)
{
  RCC_OscInitTypeDef oscinitstruct = {0};

  if (LedInitialized != RESET)
  {
    BSP_LED_DeInit(LED_RED);
    BSP_LED_DeInit(LED_GREEN);
    BSP_LED_DeInit(LED_BLUE);
    LedInitialized = RESET;
  }

  if (JoyInitialized != RESET)
  {
    BSP_JOY_DeInit(JOY1, JOY_ALL);
    JoyInitialized = RESET;
  }

  if (LcdInitialized != RESET)
  {
    BSP_LCD_GLASS_DeInit();
    LcdInitialized = RESET;
  }


  /* LSE off in all modes */
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
  {
    oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    oscinitstruct.LSEState       = RCC_LSE_OFF;
    if (HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
    {
      Error_Handler();
    }
  }

  HAL_PWR_DisableBkUpAccess();

  /* Disable remaining clocks */
  __HAL_RCC_PWR_CLK_DISABLE();
  HAL_PWREx_DisablePullUpPullDownConfig();


  __HAL_RCC_FLASH_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_I2C1_CLK_DISABLE();

  RCC->AHB1SLPENR1 = 0x0;
  RCC->APB1SLPENR1 = 0x0;
  RCC->APB1SLPENR2 = 0x0;
}

/* Format a non-negative float into 4 digits with 2 decimal places: AB.CD
   tab[0..3]: chars, tab[2] has DOT set. */
static void Format_TempHumi(float value, uint16_t *tab)
{
  /* Clear tab */
  for (int i = 0; i < 6; i++)
  {
    tab[i] = ' ';
  }

  if (value < 0.0f)
  {
    value = -value; /* ignore sign for now */
  }

  /* Limit range to avoid overflow on display */
  if (value > 99.99f)
  {
    value = 99.99f;
  }

  /* Scale to 2 decimals with rounding: XX.YY -> XXYY integer */
  uint32_t scaled = (uint32_t)(value * 100.0f + 0.5f);  /* e.g. 27.615 -> 2762 */

  uint8_t d3 = scaled % 10;
  uint8_t d2 = (scaled / 10) % 10;
  uint8_t d1 = (scaled / 100) % 10;
  uint8_t d0 = (scaled / 1000) % 10;

  /* AB.CD: A,B integer part, C,D fraction */
  tab[0] = (d0 == 0) ? ' ' : (uint16_t)('0' + d0); /* leading blank if 0 */
  tab[1] = (d0 == 0 && d1 == 0) ? ' ' : (uint16_t)('0' + d1);
  tab[2] = (uint16_t)('0' + d2);
  tab[3] = (uint16_t)('0' + d3);

  /* Put decimal point between the 2nd and 3rd digit, i.e. on tab[1]! */
  tab[1] |= DOT;
}


/**
  * @brief  Configure system clock frequency to 3MHz.
  * @param  None.
  * @retval System Status.
  */
static void SystemClock_3MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISSource          = RCC_MSI_RC1;
  RCC_OscInitStruct.MSISState           = RCC_MSI_ON;
  RCC_OscInitStruct.MSISDiv             = RCC_MSI_DIV8;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0))
  {
    /* HAL RCC configuration error */
    Error_Handler();
  }
}