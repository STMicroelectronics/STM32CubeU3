## <b>SPI_FullDuplex_ComDMA_LowPower_Slave Example Description</b>

A data buffer transmission/reception via SPI using DMA wakes up slave board from low power mode.

    Board: NUCLEO-U345RC-Q (embeds a STM32U345RCTxQ device)
    CLK Pin: PA.01 (Arduino A1 CN8 pin 2, Morpho CN7 pin30)
    MISO Pin: PA.06 (Arduino D12 CN5 pin 5, Morpho CN10 pin13)
    MOSI Pin: PA.07 (Arduino D11 CN5 pin 4, Morpho CN10 pin15)


At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_DMA() function allows the reception and the
transmission of a predefined data buffer at the same time (Full Duplex Mode).
If the Master board is used, the project SPI_FullDuplex_ComDMA_LowPower_Master must be used.
If the Slave board is used, the project SPI_FullDuplex_ComDMA_LowPower_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step, both slave and master board enter in low power mode.
RTC is configured to wake up master board after 5 seconds. Then SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive_DMA(), at the same time, initiating the transfer triggers the awakening of SPI Slave from low power and the Slave will transmit aTxBuffer
and receives aRxBuffer through HAL_SPI_TransmitReceive_DMA().
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:

 - LD2 is OFF on master board waiting RTC interrupt wakes up the system and trigger the transfer after 5 seconds.
 - LD2 turns ON when the transmission/reception process is complete.
 - LD2 toggles when there is an error in transmission/reception process.

### <b>Notes</b>
 1. You need to perform a reset on Slave board, then perform it on Master board
    to have the correct behaviour of this example.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption during Stop 1 mode - 53.9 uA
- Current Consumption during run mode - 760.8 uA

### <b>Keywords</b>

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI, DMA

### <b>Directory contents</b>
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/stm32u3xx_nucleo_conf.h                  BSP configuration file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/stm32u3xx_hal_conf.h   HAL configuration file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/stm32u3xx_it.h         Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/main.h                       Header for main.c module
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/stm32u3xx_it.c         Interrupt handlers
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/main.c                       Main program
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/system_stm32u3xx.c     stm32u3xx system source file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/stm32u3xx_hal_msp.c    HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U345RCTxQ devices.

  - This example has been tested with NUCLEO-U345RC-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U345RC-Q Set-up
    - Connect Master board PA.01 (Arduino A1 CN8 pin 2, Morpho CN7 pin30) to Slave Board PA.01 (Arduino A1 CN8 pin 2, Morpho CN7 pin30)
    - Connect Master board PA.06 (Arduino D12 CN5 pin 5, Morpho CN10 pin13) to Slave Board PA.06 (Arduino D12 CN5 pin 5, Morpho CN10 pin13)
    - Connect Master board PA.07 (Arduino D11 CN5 pin 4, Morpho CN10 pin15) to Slave Board PA.07 (Arduino D11 CN5 pin 4, Morpho CN10 pin15)
    - Connect Master board GND  to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files (master project) and load your image into target memory
    - Load the project in Master Board
 - Rebuild all files (slave project) and load your image into target memory
    - Load the project in Slave Board
 - Run the example

 
 