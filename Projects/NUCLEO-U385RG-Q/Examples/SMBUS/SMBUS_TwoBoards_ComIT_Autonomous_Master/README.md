## <b>SMBUS_TwoBoards_ComIT_Autonomous_Master Example Description</b>

How to handle SMBUS data buffer transmission/reception between two boards,
using autonomous mode.

       Board: NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)
       - SCL Pin: PB10 (Arduino D6 CN9 pin 7, Morpho CN10 pin 25)
       - SDA Pin: PA6 (Arduino D12 CN5 pin 5, Morpho CN10 pin 13)

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U385RGTxQ Devices :
The CPU at 96 MHz.

The SMBUS peripheral configuration is ensured by HAL_SMBUS_Init() function which calls HAL_SMBUS_MspInit() function.
This HAL_SMBUS_MspInit() function allows the user to configure the needed SMBUS resources according to the used
hardware (CLOCK,GPIO).

The SMBUS communication is then initiated.
The project is split in two parts:

the Master Board and the Slave Board

- Master Board
  The HAL_SMBUS_Master_Transmit_IT() functions allow the transmission of a predefined data buffer
  in Master mode using interrupt when a hardware trigger signal detected.
  
- Slave Board

  The HAL_SMBUS_Slave_Receive_IT() functions allow  the reception of a predefined data buffer
  in Slave mode using interrupt.
If the Master board is used, the project SMBUS_TwoBoards_ComIT_Autonomous_Master must be used.
If the Slave board is used, the project SMBUS_TwoBoards_ComIT_Autonomous_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

The user press the User push-button on the Master Board,
SMBUS Master configures the communication through HAL_SMBUS_Master_Transmit_IT()
and wait trigger signal from DMA to start sending data. Once DMA finish transfer
from aTxConstBuffer to aRxConstBuffer, the trigger signal is generated and SMBUS
transmission is automatically launched to SMBUS Slave which receives aRxBuffer through HAL_SMBUS_Slave_Receive_IT().
Finally, aTxBuffer and aRxBuffer are compared in slave board through Buffercmp() in order to
check buffers correctness.

NUCLEO-U385RG-Q's LEDs can be used to monitor the transfer status:

 - LD2 toggles quickly on master board waiting User push-button to be pressed.
 - LD2 turns OFF as soon as the push-button is pressed.
 - LD2 is ON when the transmission process is complete on master board.
 - LD2 is ON when the reception process is complete on slave board.
 - LD2 is OFF when there is an error in transmission/reception process.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, SMBUS, Timing, half-duplex, Interrupt, Trigger Mode, Master, Slave, Transmitter, Receiver

### <b>Directory contents</b>

  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Src/main.c                  Main program
  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Src/system_stm32u3xx.c      STM32U3xx system clock configuration file
  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Src/stm32u3xx_it.c          Interrupt handlers
  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Inc/main.h                  Main program header file
  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Inc/stm32u3xx_hal_conf.h    HAL Configuration file
  - SMBUS/SMBUS_TwoBoards_ComIT_Autonomous_Master/Inc/stm32u3xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect I2C_SCL line of Master board (PB10, Arduino D6 CN9 pin 7, Morpho CN10 pin 25) to
	I2C_SCL line of Slave Board (PB10, Arduino D6 CN9 pin 7, Morpho CN10 pin 25).
    - Connect I2C_SDA line of Master board (PA6, Arduino D12 CN5 pin 5, Morpho CN10 pin 13) to
	I2C_SDA line of Slave Board (PA6, Arduino D12 CN5 pin 5, Morpho CN10 pin 13).
    - Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

