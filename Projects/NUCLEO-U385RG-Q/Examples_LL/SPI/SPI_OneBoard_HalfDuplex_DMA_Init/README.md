## <b>SPI_OneBoard_HalfDuplex_DMA_Init Example Description</b>

Configuration of GPIO and SPI peripherals to transmit
bytes from an SPI Master device to an SPI Slave device in DMA mode. This example
is based on the STM32U3xx SPI LL API. The peripheral initialization uses the
LL initialization function to demonstrate LL init usage.

This example works with only one NUCLEO-U385RG-Q.

SPI1 Peripheral is configured in Master mode Half-Duplex Tx.
SPI3 Peripheral is configured in Slave mode Half-Duplex Rx.
GPIO associated to User push-button is linked with EXTI.

Example execution:

LD2 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD start a Half-Duplex communication through DMA.
On MASTER side (SPI1), Clock will be generated on SCK line, Transmission done on MOSI Line.
On SLAVE side (SPI3) reception is done through the MISO Line.

LD2 is On if data is well received.

In case of errors, LD2 is blinking Slowly (1s).

### <b>Keywords</b>

Connectivity, SPI, Half-duplex, Transmission, Reception, Master, Slave, MISO, MOSI, DMA

### <b>Directory contents</b>

  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/stm32u3xx_it.c          Interrupt handlers
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/main.c                  Main program
  - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on NUCLEO-U385RG-Q devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect Master SCK  PA1 to Slave SCK  PC10
    - Connect Master MOSI PA7 to Slave MISO PC11

Relation with Board connector:

  - PA1 is connected to pin 30 of CN7 connector
  - PA7 is connected to pin 15 of CN10 connector
  - PC10 is connected to pin 1 of CN7 connector
  - PC11 is connected to pin 2 of CN7 connector

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

