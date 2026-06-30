## <b>I3C_Controller_ResetPattern_WakeUpFromLowPower Example Description</b>

How to handle I3C as Controller to send reset pattern, when Target is in low power mode.

      - Board: NUCLEO-U345RC-Q (embeds a STM32U3xx device)
      - SCL Pin: PA7 (Arduino D14 CN5 pin 4, Morpho CN10 pin15)
      - SDA Pin: PA6 (Arduino D14/SDA CN5 pin 5, Morpho CN10 pin13)
      Connect GND between each board.
      Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The I3C peripheral configuration is ensured by the HAL_I3C_Init() function.
This later is calling the HAL_I3C_MspInit() function which core is implementing
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO and NVIC).
User may update this function to change I3C configuration.
To have a better signal startup, the user must adapt the parameter BusFreeDuration
depends on its hardware constraint. The value of BusFreeDuration set on this example
is link to Nucleo hardware environment.

The I3C communication is then initiated.
The project is split in two workspace:
the Controller Board I3C_Controller_ResetPattern_WakeUpFromLowPower and
the Target Board (I3C_Target_ResetPattern_WakeUpFromStandby or I3C_Target_ResetPattern_WakeUpFromStop)

- Controller Board
  The user press the User push-button on the Controller Board,
  then HAL_I3C_Ctrl_DynAddrAssign_IT() function allow the Controller to
  manage a Dynamic Address Assignment procedure to Target connected on the bus.
  This communication is done at 12.5Mhz based on I3C source clock which is at 96 MHz.

  The HAL_I3C_Ctrl_GeneratePatterns() function allow the Controller to generates target Reset Pattern with arbitration header in polling mode.

The Slave Reset Pattern is used to trigger the default or configured reset action.
The Slave Reset Pattern begins with fourteen SDA transitions while SCL is kept Low, and ends with a Repeated START followed by a STOP.

In a first step, after the user press the User push-button on the Controller Board, I3C Controller starts the communication by initiate
the Dynamic Address Assignment procedure through HAL_I3C_Ctrl_DynAddrAssign_IT().
This procedure allows the Controller to associate Dynamic Address to the Target connected on the Bus.
User can verify through debug the payload value by watch the content of TargetDesc1 in the field TARGET_BCR_DCR_PID.

After Dynamic Address Assignment, the Controller wait 2 seconds to let the target to enter into low power mode.

In a second step, When the target is in low power mode, I3C Controller start the communication by sending a Reset Pattern with arbitration header
through HAL_I3C_Ctrl_GeneratePatterns() to I3C Target. At the end of the transmission the LD2 is ON.

Then the Target wake up.

NUCLEO-U345RC-Q's LEDs can be used to monitor the transfer status:
 - LD2 is ON on Controller side when the transmission of the Reset Pattern process is completed.
 - LD2 is ON on Target side when the detection of the Reset Pattern process is completed.
 - LD2 toggle slowly when there is an error in transmission/reception process.

#### <b>Notes</b>

  1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, Communication, I3C, Polling, Controller, Target, ResetPattern, Wakeup, StandBy, STOP mode, Low power,

### <b>Directory contents</b>

  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Inc/stm32u3xx_hal_conf.h      HAL configuration file
  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Inc/stm32u3xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Inc/main.h                    Header for main.c module
  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Src/stm32u3xx_it.c            I3C interrupt handlers
  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Src/main.c                    Main program
  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Src/system_stm32u3xx.c        STM32U3xx system source file
  - I3C/I3C_Controller_ResetPattern_WakeUpFromLowPower/Src/stm32u3xx_hal_msp.c       HAL MSP file

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

 - Open your preferred toolchain
 - Rebuild all files and load your image into Controller memory (I3C_Controller_ResetPattern_WakeUpFromLowPower)
 - Then rebuild all files and load your image into Target memory (I3C_Target_ResetPattern_WakeUpFromStandBy)
 - Run the Controller before run the Target, this will let a false startup phase on Target side
 as there is no high level on the bus, if the Target is started before the Controller.
