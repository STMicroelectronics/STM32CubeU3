## <b>I2C_TwoBoards_ComDMA_Autonomous_Master Example Description</b>

How to handle I2C data buffer transmission/reception between two boards,
via DMA.

       Board: NUCLEO-U366RE-Q (embeds a STM32U366RETxQ device)
       - SCL Pin: PC.0 (Arduino A5 CN8 pin 6, Morpho CN10 pin 38)
       - SDA Pin: PC.1 (Arduino A4 CN8 pin 5, Morpho CN7 pin 36)

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.

The project is split in two parts: the Master Board and the Slave Board.

- Master Board:

  The HAL_I2C_Master_Receive_DMA() and the HAL_I2C_Master_Transmit_DMA() functions
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using DMA after the trigger event (Transfer Complete GPDMA1 Channel3).

- Slave Board:

  The HAL_I2C_Slave_Receive_DMA() and the HAL_I2C_Slave_Transmit_DMA() functions
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using DMA.
  
If the Master board is used, the project I2C_TwoBoards_ComDMA_Autonomous_Master must be used.
If the Slave board is used, the project I2C_TwoBoards_ComDMA_Autonomous_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button on the Master Board,
I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit()
after the trigger event (Transfer Complete GPDMA1 Channel3) to I2C Slave which receives
aRxBuffer through HAL_I2C_Slave_Receive(). The second step starts when the user press
the User push-button on the Master Board, the I2C Slave sends aTxBuffer through
HAL_I2C_Slave_Transmit() to the I2C Master which receives aRxBuffer through
HAL_I2C_Master_Receive() after the trigger event (Transfer Complete GPDMA1 Channel3).
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.

Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to
check buffers correctness.

NUCLEO-U366RE-Q's LEDs can be used to monitor the transfer status:

 - LD2 toggles quickly on master board waiting User push-button to be pressed.
 - LD2 turns OFF as soon as the User push-button is pressed.
 - LD2 is ON when the Master transmission / Slave reception process is complete.
 - LD2 toggles quickly on master board waiting User push-button to be pressed.
 - LD2 turns OFF as soon as the User push-button is pressed.
 - LD2 is OFF when the Master reception / Slave transmission process is complete.
 - In case of errors, LD2 is blinking slowly (1s).
 
#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, Communication, I2C, DMA, Master, Slave, Transmission, Reception, Fast mode plus

### <b>Directory contents</b> 

  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Inc/stm32u3xx_it.h          DMA and I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Inc/main.h                  Header for main.c module
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Src/stm32u3xx_it.c          DMA and I2C interrupt handlers
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Src/system_stm32u3xx.c      STM32U3xx system source file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Master/Src/stm32u3xx_hal_msp.c     HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U366RETxQ devices.

  - This example has been tested with NUCLEO-U366RE-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U366RE-Q Set-up
    - Connect I2C_SCL line of Master board (PC.0, Arduino A5 CN8 pin 6, Morpho CN10 pin 38) to I2C_SCL line of Slave Board (PC.0, Arduino A5 CN8 pin 6, Morpho CN10 pin 38).
    - Connect I2C_SDA line of Master board (PC.1, Arduino A4 CN8 pin 5, Morpho CN7 pin 36) to I2C_SDA line of Slave Board (PC.1, Arduino A4 CN8 pin 5, Morpho CN7 pin 36).
    - Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

