## <b>UART_Printf Example Description</b>

Re-routing of the C library printf function to the UART.
The UART outputs a message on the HyperTerminal.

Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

- Board : NUCLEO-U385RG-Q
- Tx Pin: PA.09: available through ST-Link
- Rx Pin: PA.10: available through ST-Link

LD2 toggles when there is an error.

The USART is configured as follows:

    - BaudRate = 115200 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

#### <b>Notes</b>

1. When the parity is enabled, the computed parity is inserted at the MSB
   position of the transmitted data.

2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
   based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
   a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
   than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
   To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
   to have correct HAL operation.

### <b>Keywords</b>

Connectivity, UART/USART, Printf, baud rate, HyperTerminal, full-duplex, Interrupt,
Transmitter, Receiver, Asynchronous

### <b>Directory contents</b>

  - UART/UART_Printf/Inc/stm32u3xx_nucleo_conf.h BSP configuration file
  - UART/UART_Printf/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - UART/UART_Printf/Inc/stm32u3xx_it.h          IT interrupt handlers header file
  - UART/UART_Printf/Inc/main.h                   Header for main.c module
  - UART/UART_Printf/Src/stm32u3xx_it.c          Interrupt handlers
  - UART/UART_Printf/Src/main.c                   Main program
  - UART/UART_Printf/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - UART/UART_Printf/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - Hyperterminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 115200 baud
    - Flow control: None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

