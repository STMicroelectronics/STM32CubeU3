## <b>I3C_Target_ResetPattern_WakeUpFromStandBy Example Description</b>

How to handle I3C as Target to wake up from standby, when the Target receives a reset pattern.

      - Board: NUCLEO-U345RC-Q (embeds a STM32U3xx device)
      - SCL Pin: PA7 (Arduino D14 CN5 pin 4, Morpho CN10 pin15)
      - SDA Pin: PA6 (Arduino D14/SDA CN5 pin 5, Morpho CN10 pin13)
      Connect GND between each board
      Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 96 MHz.

The I3C peripheral configuration is ensured by the HAL_I3C_Init() function.
This later is calling the HAL_I3C_MspInit()function which core is implementing
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO and NVIC).
You may update this function to change I3C configuration.

The project is split in two workspace:
the Controller Board (I3C_Controller_ResetPattern_WakeUpFromLowPower) and
the Target Board (I3C_Target_ResetPattern_WakeUpFromStandBy)

- Target Board

On product STM32U3xx, the feature Reset Pattern is both functional on PWR peripheral side and on I3C peripheral.
The PWR Reset Pattern detection is functional during low power phases.
The I3C Reset Pattern detection is functional during run mode phases.

The PWR Reset Pattern detection, help target to detect the Reset Pattern during the Stop, Standby mode.

The HAL_I3C_Tgt_Config(), the HAL_I3C_SetConfigFifo(), and the HAL_I3C_ActivateNotification() functions
allow respectively the configuration of the Target descriptor,
the configuration of the internal hardware FIFOs and the enabling of Asynchronous catching event in Target mode using Interrupt.

The HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 96 MHz.

The I3C peripheral configuration is ensured by the HAL_I3C_Init() function.
This later is calling the HAL_I3C_MspInit()function which core is implementing
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO and NVIC).
User may update this function to change I3C configuration.

In a first step, the Target wait the assignation of a proper associated Dynamic Address from the Controller.

In a second step, the Target enable the wake up line 9 for I3C1 through HAL_PWR_EnableWakeUpLine(), then Enable I3C pull-up()
for SCL and SDA pins through HAL_PWREx_EnableI3CPullUp().

Then Target goes to Low power StandBy Mode through HAL_PWR_EnterSTANDBYMode().

The I3C Controller start the communication by sending the reset pattern through HAL_I3C_Ctrl_GeneratePatterns()
to I3C Target which is in Low power mode.

Target wakeup from StandBy mode and and then wakeup the STM32U3xx.

At the beginning of the main program, the __HAL_PWR_GET_FLAG() function check Standby flag
to detect the wake up from standBy, then HAL_PWR_GetClearWakeupSource() function check and clean the wakeup event
received from the WKUP line 9.

At the end of the example, after waking up from STANDBY, the debugger loses synchronization.
The user must then attach the debugger and check that the target is ready to manage an ENTDAA sequence.

The user can debug the wake up from standby by watch the contents of the ubWakeUpFromStandBy value, if it is equal to 1.

NUCLEO-U345RC-Q's LEDs can be used to monitor the transfer status:

 - LD2 toggle slowly when there is an error in transmission/reception process.
 - LD2 ON when target wake up from standby

#### <b>Notes</b>

  1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, Communication, I3C, Interrupt, Controller, Target, Dynamic Address Assignment,
ResetPattern, Wakeup, StandBy

### <b>Directory contents</b>

  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Inc/stm32u3xx_hal_conf.h      HAL configuration file
  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Inc/stm32u3xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Inc/main.h                    Header for main.c module
  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Src/stm32u3xx_it.c            I3C interrupt handlers
  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Src/main.c                    Main program
  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Src/system_stm32u3xx.c        STM32U3xx system source file
  - I3C/I3C_Target_ResetPattern_WakeUpFromStandBy/Src/stm32u3xx_hal_msp.c       HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U3xx devices.

  - This example has been tested with NUCLEO-U345RC-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U345RC-Q Set-up

    - Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.
    - Connect I3C_SCL line of Controller board (PA7 Arduino D14 CN5 pin 4, Morpho CN10 pin15) to I3C_SCL line of Target Board (PA7 Arduino D14 CN5 pin 4, Morpho CN10 pin15).
    - Connect I3C_SDA line of Controller board (PA6 Arduino D14/SDA CN5 pin 5, Morpho CN10 pin13) to I3C_SDA line of Target Board (PA6 Arduino D14/SDA CN5 pin 5, Morpho CN10 pin13).
    - Connect GND of Controller board to GND of Target Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Rebuild all files and load your image into Controller memory (I3C_Controller_ResetPattern_WakeUpFromLowPower)
 - Then rebuild all files and load your image into Target memory (I3C_Target_ResetPattern_WakeUpFromStandBy)
 - Run the Controller before run the Target, this will let a false startup phase on Target side
 as there is no high level on the bus, if the Target is started before the Controller.
