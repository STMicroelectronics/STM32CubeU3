## <b>LPUART_WakeUpFromStop2_autonomus Example Description</b>

LPUART communication in DMA mode between two boards, with a one-shot autonomous
echo performed by the receiver board while it uses Stop2 mode.
The transmitter board remains in run mode, sends one reference frame when the
user button is pressed, then waits for the echoed frame.

    Board: NUCLEO-U3C5ZI-Q (embeds a STM32U3C5ZITxQ device)
    Tx Pin: PA2 (Pin 18 in CN10)
    Rx Pin: PA3 (Pin 34 in CN10)

The user presses the User push-button on the transmitter board.
Then, the transmitter board sends in DMA mode a message to the receiver board.
The receiver board has already armed LPUART1 and GPDMA, enters Stop2 mode,
receives the frame into RAM, and echoes the same RAM buffer back using the
autonomous trigger path.
After transmit completion, the receiver briefly wakes, disables the UART Stop
configuration, clears autonomous mode, de-initializes LPUART and DMA, disables
the GPDMA clock, and enters a final bare Stop2 state.
The measured final receiver current is about 25 uA.

WARNING: as both boards do not behave the same way, "BOARD_IN_STOP_MODE"
compilation switch is defined in /Src/main.c and must be enabled at
compilation time before loading the executable in the receiver board.
The transmitting board needs to be loaded with an executable software obtained
with BOARD_IN_STOP_MODE disabled.

NUCLEO-U3C5ZI-Q board LED is used to monitor the transfer status:

- On the receiver board, LD1 is turned on for 2 seconds after power-up, then
  the board prepares the one-shot autonomous receive-and-echo transaction and
  enters Stop2.
- On the transmitter board, LD1 is blinking rapidly (100 ms period) while the
  board is waiting for the user to press the User push-button and for the
  echoed frame to be received.
- When the test passes, LD1 on the transmitter board is turned on and remains on.
- If there is an initialization or transfer error, LD1 is turned off and the
  application remains in Error_Handler().

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock.
After that, GPIO, GPDMA1, ICACHE, and LPUART1 are initialized before the application
enters either the receiver or transmitter flow depending on the compilation switch.

The LPUART is configured as follows:

    - BaudRate = 115200 baud
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Clock prescaler = DIV1
    - One-bit sampling disabled
    - Reception and transmission are enabled

### <b>Notes</b>

 1. This receiver implementation is intentionally optimized for a single
    receive-and-echo transaction followed by the lowest-current final Stop2 state.
    It is not a continuously re-armed autonomous receiver.

 2. The key technical result is not only that the receiver re-enters Stop2,
    but that it re-enters Stop2 after shutting down the still-armed autonomous
    UART and DMA path.

 3. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR.
    This implies that if HAL_Delay() is called from a peripheral ISR process,
    then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application needs to ensure that the SysTick time base is always set to
    1 millisecond to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
1. Board1 with Stop 2 mode (@3.3V)
- Current Consumption during Stop 2 mode ~26-27 µA
- Current Consumption during autonomus data transfer 225 µA
- Current Consumption during run mode 625.8 µA

2. Board2 (@3.3V)
- Current consumption during run mode 625.8 µA

### <b>Keywords</b>

Connectivity, LPUART, baud rate, full-duplex, DMA, Stop2, autonomous mode,
low power, transmitter, receiver, asynchronous

### <b>Directory contents</b>

  - UART/LPUART_WakeUpFromStop2_autonomus/Inc/stm32u3xx_nucleo_conf.h  BSP configuration file
  - UART/LPUART_WakeUpFromStop2_autonomus/Inc/stm32u3xx_hal_conf.h     HAL configuration file
  - UART/LPUART_WakeUpFromStop2_autonomus/Inc/stm32u3xx_it.h           Interrupt handlers header file
  - UART/LPUART_WakeUpFromStop2_autonomus/Inc/main.h                   Header for main.c module
  - UART/LPUART_WakeUpFromStop2_autonomus/Src/stm32u3xx_it.c           Interrupt handlers
  - UART/LPUART_WakeUpFromStop2_autonomus/Src/main.c                   Main program
  - UART/LPUART_WakeUpFromStop2_autonomus/Src/stm32u3xx_hal_msp.c      HAL MSP module
  - UART/LPUART_WakeUpFromStop2_autonomus/Src/system_stm32u3xx.c       STM32U3xx system source file
  - UART/LPUART_WakeUpFromStop2_autonomus/autonomous_presentation.html Technical presentation file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U3C5ZITxQ devices.
  - This example has been tested with two NUCLEO-U3C5ZI-Q boards embedding
    a STM32U3C5ZITxQ device and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U3C5ZI-Q set-up
    - Connect a wire between 1st board PA2 pin (LPUART Tx) and 2nd board PA3 pin (LPUART Rx)
    - Connect a wire between 1st board PA3 pin (LPUART Rx) and 2nd board PA2 pin (LPUART Tx)
    - Connect 1st board GND to 2nd board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Enable BOARD_IN_STOP_MODE in /Src/main.c, rebuild all files, and load the image into the receiver board
 - Disable BOARD_IN_STOP_MODE in /Src/main.c, rebuild all files, and load the image into the transmitter board
 - Run the example on both boards
 - Wait for the receiver LD1 startup indication to finish
 - Press the User push-button on the transmitter board
 - Check that the echoed frame is received correctly and that the transmitter LD1 remains on
 - Measure the receiver final Stop2 current after cleanup, which is expected to be about 25 uA


