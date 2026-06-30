## <b>I3C_Controller_Private_Autonomous Example Description</b>

How to handle the controller to transmit private message in autonomous mode

      - Board: NUCLEO-U385RG (embeds a STM32U385RG device)
      - SCL Pin: PB13 (Morpho CN10 pin 30).
      - SDA Pin: PB14 (Morpho CN10 pin 28).
      Connect GND between each board
      Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The I3C peripheral configuration is ensured by the HAL_I3C_Init() function.
The HAL_I3C_SetConfigAutonomousMode() function set Autonomous Mode configuration and
associate RTC alarm as trigger of Autonomous mode.
This later is calling the HAL_I3C_MspInit() function which core is implementing
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO, NVIC and DMA).
User may update this function to change I3C configuration.
To have a better signal startup, the user must adapt the parameter BusFreeDuration
depends on its hardware constraint. The value of BusFreeDuration set on this example
is link to Nucleo hardware environment.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit() function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLOCK,
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

The I3C communication is then initiated.
The project is split in two workspaces:
the Controller Board (I3C_Controller_Private_Autonomous) and the Target Board (I3C_Target_Private_Autonomous)

- Controller Board
  The HAL_I3C_Ctrl_DynAddrAssign_IT() function allow the Controller to manage a Dynamic Address Assignment procedure to Target connected on the bus.
  This communication is done at 12.5Mhz based on I3C source clock which is at 96 MHz.

In a first step, after the user press the USER push-button on the Controller Board,
I3C Controller starts the communication by initiate the Dynamic Address Assignment procedure through HAL_I3C_Ctrl_DynAddrAssign_IT().
This procedure allows the Controller to associate Dynamic Address to the Target connected on the Bus.
User can verify through debug the payload value by watch the content of TargetDesc1 in the field TARGET_BCR_DCR_PID.

In a second step, set the alarm configuration, the Time is set to 02:32:00 and
the Alarm must be generated after 05 seconds on 02:32:05. via the MX_RTC_Init() function.

Then the I3C Controller prepare the communication to send a aTxBuffer through HAL_I3C_Ctrl_Transmit_DMA() to I3C Target,
thanks to the autonomous mode, the TX transfer will be start only when a trigger is detected on I3C side.
On this example the trigger is an RTC alarm event.

To prevent wakeup, the I3C controller disables all interrupts I3C and DMA. Then Controller goes to Low Power Mode (Stop 0 Mode).

During the low power mode, the TX transfer triggered by the RTC_Alarm can be performed without waking up the system.

At the end of the transfer (When the LD2 at the target board is ON), the user press the USER push-button on the Controller Board to wake up the system.

NUCLEO-U385RG's LEDs can be used to monitor the transfer status:
 - LD2 is toggle at ENTDAA reception and each time the Command Code process is completed.
 - LD2 is toggle slowly when there is an error in Command Code process.

#### <b>Notes</b>

  1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption during Stop 0 Mode - 281 uA
- Current Consumption during RUN Mode - 1780 uA

### <b>Keywords</b>

Connectivity, Communication, I3C, Interrupt, IT, Controller, Target, Dynamic Address Assignment, Direct Command,
Transmission, Autonomous, RTC, STOP, Wakeup, Low power, DMA

### <b>Directory contents</b>

  - I3C/I3C_Controller_Private_Autonomous/Inc/desc_target1.h            Target Descriptor
  - I3C/I3C_Controller_Private_Autonomous/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
  - I3C/I3C_Controller_Private_Autonomous/Inc/stm32u3xx_hal_conf.h      HAL configuration file
  - I3C/I3C_Controller_Private_Autonomous/Inc/stm32u3xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Controller_Private_Autonomous/Inc/main.h                    Header for main.c module
  - I3C/I3C_Controller_Private_Autonomous/Src/stm32u3xx_it.c            I3C interrupt handlers
  - I3C/I3C_Controller_Private_Autonomous/Src/main.c                    Main program
  - I3C/I3C_Controller_Private_Autonomous/Src/system_stm32u3xx.c        STM32U3xx system source file
  - I3C/I3C_Controller_Private_Autonomous/Src/stm32u3xx_hal_msp.c       HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RG devices.

  - This example has been tested with NUCLEO-U385RG board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG Set-up

    - Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.
    - Connect I3C_SCL line of Controller board (PB13 Morpho CN10 pin 30) to I3C_SCL line of Target Board (PB13 Morpho CN10 pin 30).
    - Connect I3C_SDA line of Controller board (PB14 Morpho CN10 pin 28) to I3C_SDA line of Target Board (PB14 Morpho CN10 pin 28).
    - Connect GND of Controller board to GND of Target Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into Controller memory (I3C_Controller_Private_Autonomous)
 - Then rebuild all files and load your image into Target memory (I3C_Target_Private_Autonomous)
 - Run the Controller in debug mode before run the Target in normal mode.
 This sequence will prevent a false startup phase on Target side
 as there is no high level on the bus, if the Target is started before the Controller.
