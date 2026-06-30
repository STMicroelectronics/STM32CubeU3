## <b>LPUART_WakeUpFromStop Example Description</b>

Configuration of an LPUART to wake up the MCU from Stop 2 mode
when a given stimulus is received.

  - Board:  NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)
  - Tx Pin: PA2 : pin 35 of CN10 connector
  - Rx Pin: PA3 : pin 37 of CN10 connector

Board 1 enters Stop 2 mode and is awakened by Board 2, which sends "RXNE wake-up" data to trigger the wake-up event. Upon recognizing the wake-up event, the RXNE interrupt is activated, waking up Board 1.
To confirm its wake-up, Board 1 sends the received buffer back to Board 2, which then verifies if it is the expected message.

WARNING: as both boards do not behave the same way, "BOARD_IN_STOP_MODE"
compilation switch is defined in UART/LPUART_WakeUpFromStop/Src/main.c and must
be enabled at compilation time before loading the executable in board 1 (that
which is set in Stop 2 mode).
The stimuli-transmitting board (board 2) needs to be loaded with an executable
software obtained with BOARD_IN_STOP_MODE disabled.

- Initially, Board 1's LD2 is turned on for 2 seconds. During this time, the LPUART is prepared to wake up from Stop 2 Mode : reception process is started in interrupt mode using HAL_UART_Receive_IT, LD2 is turned off, and Board 1 enters Stop 2 Mode. On first received character from Board 2, Board 1 will exit from Stop 2 mode and reception goes on till the expected length is received, then the HAL_UART_RxCpltCallback() callback is triggered.

- Meanwhile, Board 2's LD2 blinks rapidly with a 100 ms period. The user should wait for Board 1's LD2 to turn off and then press Board 2's BUTTON_USER to send a wake-up signal to Board 1. Upon receiving the first character, Board 1's MCU wakes up due to the LPUART RXNE interrupt, and LD2 turns on to indicate the wake-up. Board 1 then transmits the received buffer back to Board 2, which verifies if it is the expected message. If the test passes, LD2 on both boards turns on.

- If the test fails or if there is an initialization or transfer error, LD2 turns off.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.

Then the SystemClock_Config() function is used to configure the system 
(SYSCLK) to run with HSI as source clock.
Additionally, MSIK is enabled as kernel clock source for LPUART1 (about 6 MHz)
to demonstrate LPUART wake-up capabilities with low power. Each time board 1
is awoken, the system clock (SYSCLK) is restored.

The LPUART is configured as follows:

    - BaudRate = 115200 baud
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - Parity none
    - Hardware flow control disabled (RTS and CTS signals)

This example can not be used in DEBUG mode due to the fact that the
    Cortex-M33 core is no longer clocked during low power mode and so
    debugging features are not available.

#### <b>Notes</b>

  1.  This example can not be used in DEBUG mode due to the fact that the
      Cortex-M33 core is no longer clocked during low power mode and so
      debugging features are not available.

  2.  Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process,
      then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be
      blocked. To change the SysTick interrupt priority you have to use
      HAL_NVIC_SetPriority() function.

  3. The application needs to ensure that the SysTick time base is always set
      to 1 millisecond to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
1. Board1 with Stop 2 mode (@3.3V)
- Current Consumption during Stop 2 mode ~26-27 µA (with LPUART1 wake-up enabled and MSIK as LPUART kernel clock)
- Current Consumption during run mode 484.8 µA

2. Board2 (@3.3V)
- Current consumption during run mode 499.8 µA

### <b>Keywords</b>

Connectivity, LPUART, baud rate, RS-232, HyperTerminal, full-duplex,
Transmitter, Receiver, Asynchronous, Low Power, Wake Up

### <b>Directory contents</b>

  - UART/LPUART_WakeUpFromStop/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - UART/LPUART_WakeUpFromStop/Inc/stm32u3xx_hal_conf.h        HAL configuration file
  - UART/LPUART_WakeUpFromStop/Inc/stm32u3xx_it.h              Interrupt handlers header file
  - UART/LPUART_WakeUpFromStop/Inc/main.h                      Header for main.c module
  - UART/LPUART_WakeUpFromStop/Src/stm32u3xx_it.c              Interrupt handlers
  - UART/LPUART_WakeUpFromStop/Src/main.c                      Main program
  - UART/LPUART_WakeUpFromStop/Src/stm32u3xx_hal_msp.c         HAL MSP module
  - UART/LPUART_WakeUpFromStop/Src/system_stm32u3xx.c          STM32U3xx system source file

### <b>Hardware and Software environment</b>
  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with a couple of NUCLEO-U385RG-Q boards embedding a
    STM32U385RGTxQ device and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U385RG-Q set-up

    - Connect a wire between 1st board PA2 (pin 35 of CN10 connector)(LPUART1 Tx) and 2nd board PA3  (pin 37 of CN10 connector)(LPUART1 Rx)
    - Connect a wire between 1st board PA3 (pin 37 of CN10 connector) (LPUART1 Rx) and 2nd board PA2 (pin 35 of CN10 connector) (LPUART1 Tx)

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example