## <b>I3C_Target_WakeUpFromStop Example Description</b>

How to handle an I3C as a Target to perform data buffer transmission/reception between two boards,
using an interrupt, when the Target is in Stop 1 mode.

      - Board: NUCLEO-U385RG-Q (embeds a STM32U3xx device)
      - SDA Pin: PA6 (Arduino D12 CN5 pin 5, Morpho CN10 pin13)
      - SCL Pin: PA7 (Arduino D11 CN5 pin 4, Morpho CN10 pin15)
      Connect GND between each board
      Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.


The I3C peripheral configuration is ensured by the HAL_I3C_Init() function.
This later is calling the HAL_I3C_MspInit()function which core is implementing
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO and NVIC).
You may update this function to change I3C configuration.

The I3C communication is then initiated.

The project is split in two workspace:
the Controller Board (I3C_Controller_WakeUpFromStop) and the Target Board (I3C_Target_WakeUpFromStop)

- Target Board

  The HAL_I3C_ActivateNotification() function allow the Target application to be inform when the Controller
  assign a Dynamic Address.

  The HAL_I3C_Tgt_Receive_IT() and the HAL_I3C_Tgt_Transmit_IT() functions

  allow respectively the reception and the transmission of a predefined data buffer in Target mode using Interrupt.

  The Controller send data at 12.5Mhz during Push-pull phases.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step the Target wait the assignation of a proper associated Dynamic Address from the Controller.

Then Target goes to Low power Stop 1 Mode through HAL_PWR_EnterSTOPMode().

The second step starts when the user press the User push-button on the Controller Board,

the I3C Controller continue the communication by sending aTxBuffer through HAL_I3C_Ctrl_Transmit_IT()

to I3C Target which is in Low power mode.

Target wakes up from Stop 1 mode, and then treats and receives aRxBuffer through HAL_I3C_Tgt_Receive_IT().

The third step starts when the user press the User push-button on the Controller Board,

the I3C Target sends aTxBuffer through HAL_I3C_Tgt_Transmit_IT()

to the I3C Controller which receives aRxBuffer through HAL_I3C_Ctrl_Receive_IT().

The end of this two steps are monitored through the HAL_I3C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to
check buffers correctness.

NUCLEO-U385RG-Q's LEDs can be used to monitor the transfer status:

 - LD2 is ON when the Dynamic Address Assignment process is complete.
 - LD2 is OFF when the transmission process is complete.
 - LD2 is ON when the reception process is complete.
 - LD2 toggle slowly when there is an error in transmission/reception process.

#### <b>Notes</b>

  1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption during (Stop 1) - 108 uA
- Current Consumption during RUN Mode - 712 uA

### <b>Keywords</b>

Connectivity, Communication, I3C, Interrupt, Controller, Target, Dynamic Address Assignment, Private,
Transmission, Reception, Wakeup

### <b>Directory contents</b>

  - I3C/I3C_Target_WakeUpFromStop/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
  - I3C/I3C_Target_WakeUpFromStop/Inc/stm32u3xx_hal_conf.h      HAL configuration file
  - I3C/I3C_Target_WakeUpFromStop/Inc/stm32u3xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Target_WakeUpFromStop/Inc/main.h                    Header for main.c module
  - I3C/I3C_Target_WakeUpFromStop/Src/stm32u3xx_it.c            I3C interrupt handlers
  - I3C/I3C_Target_WakeUpFromStop/Src/main.c                    Main program
  - I3C/I3C_Target_WakeUpFromStop/Src/system_stm32u3xx.c        STM32U3xx system source file
  - I3C/I3C_Target_WakeUpFromStop/Src/stm32u3xx_hal_msp.c       HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U3xx devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up

    - Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.
    - Connect I3C_SDA line of Controller board (PA6 Arduino D12 CN5 pin 5, Morpho CN10 pin13) to I3C_SDA line of Target Board (PA6 Arduino D12 CN5 pin 5, Morpho CN10 pin13).
    - Connect I3C_SCL line of Controller board (PA7 Arduino D11 CN5 pin 4, Morpho CN10 pin15) to I3C_SCL line of Target Board (PA7 Arduino D11 CN5 pin 4, Morpho CN10 pin15).
    - Connect GND of Controller board to GND of Target Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Rebuild all files and load your image into Controller memory (I3C_Controller_WakeUpFromStop)
 - Then rebuild all files and load your image into Target memory (I3C_Target_WakeUpFromStop)
 - Run the Controller before run the Target, this will let a false startup phase on Target side
 as there is no high level on the bus, if the Target is started before the Controller.
