## <b>I3C_Target_Direct_Command_Polling Example Description</b>

How to handle a Direct Command procedure between an I3C Controller and an I3C Target,
using Controller in Polling.

      - Board: NUCLEO-U385RG-Q's (embeds a STM32U385RGTxQ device)
      - SCL Pin: PA7 (Arduino SCL/D11 CN5 pin 4, Morpho CN10 pin15) to PA7 (Arduino SCL/D11 CN5 pin 4, Morpho CN10 pin15)
      - SDA Pin: PA6 (Arduino SDA/D12 CN5 pin 5, Morpho CN10 pin13) to PA6 (Arduino SDA/D12 CN5 pin 5, Morpho CN10 pin13)
      Connect GND between each board
      Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.

At the beginning of the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The I3C peripheral configuration and I3C resources (CLOCK and GPIO) are ensured by the MX_I3C1_Init() function.
User may update this function to change I3C configuration.

The I3C communication is then initiated.

The project is split in two workspaces:

the Controller Board (I3C_Controller_Direct_Command_Polling) and the Target Board (I3C_Target_Direct_Command_Polling)

- Target Board
  The MX_I3C1_Init() function allow the configuration of the internal hardware FIFOs,
  the configuration of the Target own specific parameter.

  The Controller send data at 12.5Mhz during Push-pull phase.

In a first step the Target on hardware side wait the assignation of a proper associated Dynamic Address from the Controller.
As this phase is totally manage by internal hardware state machine, there is no additional specific code to manage this
action in polling mode.

In a second step, until Common Command Code procedure from the Controller,
the Target wait in a no activity loop.
Then, at reception of the first Common Code procedure from the Controller,
the Target start the sending or receiving of the CCC additional data depending of Common Command Code.

This sending or receiving of additional data is totally manage by internal hardware state machine.

Only each time a Set CCC command is confirmed, the treatment is done through the main loop,
the associated data is compared with the expected value.

Each time a Set CCC command is confirmed, the LD2 is toggle.

NUCLEO-U385RG-Q's LEDs can be used to monitor the transfer status:

 - LD2 is toggle each time a Set Command Code process is treated on hardware side.
 - LD2 is toggle slowly when there is an error in Command Code process.  

### <b>Keywords</b>

Connectivity, Communication, I3C, Polling, Controller, Target, Dynamic Address Assignment, Direct Command,
Transmission, Reception

### <b>Directory contents</b>

  - I3C/I3C_Target_Direct_Command_Polling/Inc/stm32u3xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Target_Direct_Command_Polling/Inc/main.h                    Header for main.c module
  - I3C/I3C_Target_Direct_Command_Polling/Inc/stm32_assert.h            Template file to include assert_failed function
  - I3C/I3C_Target_Direct_Command_Polling/Src/stm32u3xx_it.c            I3C interrupt handlers
  - I3C/I3C_Target_Direct_Command_Polling/Src/main.c                    Main program
  - I3C/I3C_Target_Direct_Command_Polling/Src/system_stm32u3xx.c        STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up

    - Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.
    - Connect I3C_SCL line of Controller board (PA7 Arduino SCL/D11 CN5 pin 4, Morpho CN10 pin15) to I3C_SCL line of
      Target Board (PA7 Arduino SCL/D11 CN5 pin 4, Morpho CN10 pin15).
    - Connect I3C_SDA line of Controller board (PA6 Arduino SDA/D12 CN5 pin 5, Morpho CN10 pin13) to I3C_SDA line of
      Target Board (PA6 Arduino SDA/D12 CN5 pin 5, Morpho CN10 pin13).
    - Connect GND of Controller board to GND of Target Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into Controller memory (I3C_Controller_Direct_Command_Polling)
 - Then rebuild all files and load your image into Target memory (I3C_Target_Direct_Command_Polling)
 - Run the Controller in debug mode before run the Target in normal mode.
 
 This sequence will prevent a false startup phase on Target side
 as there is no high level on the bus, if the Target is started before the Controller.
