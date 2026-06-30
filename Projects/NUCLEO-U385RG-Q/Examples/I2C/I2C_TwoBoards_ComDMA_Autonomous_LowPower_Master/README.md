## <b>I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master Example Description</b>

How to handle an I2C3 as a Master to perform a data buffer transmission/reception in
autonomous mode between two boards, using DMA while entering Stop 2 low-power mode.

       Board: NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)
      - SCL Pin: PC0
      - SDA Pin: PC1

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
  in Master mode using DMA after the RTC wakeup timer autonomous trigger event.

- Slave Board:
  The HAL_I2C_Slave_Receive_DMA() and the HAL_I2C_Slave_Transmit_DMA() functions
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using DMA.

If the Master board is used, the project I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master must be used.
If the Slave board is used, the project I2C_TwoBoards_ComDMA_Autonomous_LowPower_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step, Master starts transmission by arming HAL_I2C_Master_Transmit_DMA(),
programming RTC wakeup timer and entering Stop 2 mode. RTC wakeup timer event then
autonomously triggers I2C3 transfer to Slave, which receives aRxBuffer through
HAL_I2C_Slave_Receive_DMA(). In the second step, Master arms HAL_I2C_Master_Receive_DMA(),
re-programs RTC wakeup timer and enters Stop 2 again. RTC wakeup event autonomously
triggers the second I2C transaction while Slave sends aTxBuffer through
HAL_I2C_Slave_Transmit_DMA(). The end of these two steps is monitored through
HAL_I2C_GetState() function result.

Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to
check buffers correctness.

NUCLEO-U385RG-Q's LEDs can be used to monitor the transfer status:

 - LD2 is ON when the Master transmission / Slave reception process is complete.

 - LD2 is OFF when the Master reception / Slave transmission process is complete.

 - During Stop 2 wait windows, LD2 state remains unchanged until RTC wakeup trigger starts transfer.

 - In case of errors, LD2 is blinking slowly (1s).
#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
1. Master Board
- Current Consumption during RUN Mode (transmission) - 813.2 uA
- Current Consumption during RUN Mode (Reception) - 850.2 uA
- Current Consumption during Stop 2 Mode - 4.56 uA

### <b>Keywords</b>

Connectivity, Communication, I2C, DMA, Master, Slave, Transmission, Reception, Fast mode plus

### <b>Directory contents</b>

  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Inc/stm32u3xx_it.h          DMA and I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Inc/main.h                  Header for main.c module
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Src/stm32u3xx_it.c          DMA and I2C interrupt handlers
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Src/system_stm32u3xx.c      STM32U3xx system source file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_LowPower_Master/Src/stm32u3xx_hal_msp.c     HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect I2C_SCL line of Master board (PC0) to I2C_SCL line of Slave Board (PC0).
    - Connect I2C_SDA line of Master board (PC1) to I2C_SDA line of Slave Board (PC1).
    - Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

