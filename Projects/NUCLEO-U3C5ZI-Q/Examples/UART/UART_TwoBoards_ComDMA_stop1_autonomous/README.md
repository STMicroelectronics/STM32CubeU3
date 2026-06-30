## <b>UART_TwoBoards_ComDMA_stop1_autonomous Example Description</b>

USART1 transmission and reception between two NUCLEO-U3C5ZI-Q boards using DMA.

The user presses the User push-button on board 1.
Then board 1 sends a buffer to board 2 using USART1 TX DMA.
Board 2 prepares USART1 RX DMA, enables USART1 wakeup from Stop 1 mode,
enters Stop 1, wakes up when data is received, and sends the received buffer back.
Finally, board 1 compares the received buffer with the transmitted buffer.
If both buffers are identical, the test passes.

The system clock runs at 96 MHz in normal run mode.

Board: NUCLEO-U3C5ZI-Q (STM32U3C5ZITxQ)
Tx Pin: PA.09 (CN11 pin 16)
Rx Pin: PA.10 (CN11 pin 18)

WARNING: both boards do not use the same build.
Enable `TRANSMITTER_BOARD` in `Src/main.c` for board 1.
Leave `TRANSMITTER_BOARD` disabled for board 2.

At the beginning of the main program, `HAL_Init()` resets all peripherals,
initializes the Flash interface, and configures the SysTick.
Then `SystemClock_Config()` configures the system clock to run at 96 MHz.

The UART is configured as follows:

- BaudRate = 115200 baud
- Word Length = 8 bits
- One Stop Bit
- No parity
- Hardware flow control disabled
- TX and RX enabled

DMA is used for USART1 TX and RX.
On the receiver build, USART1 wakeup from Stop 1 is enabled on RX non-empty.
Autonomus mode is used so the RX DMA transfer complete event triggers the echo TX.

NUCLEO-U3C5ZI-Q board LED (LD1) indicates status:

- Board 1 blinks LED (LD1) rapidly while waiting for the user button.
- Board 2 turns LED (LD1) on for 2 seconds, then turns it off before entering Stop 1.
- After wakeup and successful transfer, LED (LD1) is on.
- If an error occurs, LED (LD1) blinks slowly.

### <b>Notes</b>

1. The example uses two different builds. Enable `TRANSMITTER_BOARD` for the board that transmits first, and keep it disabled for the board that receives first.
2. On the receiver build, USART1 RX DMA is started before entering Stop 1 mode so the incoming data can be captured as soon as the board wakes up.
3. After wakeup, the receiver board restores the run clock configuration and completes the transmit-back operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
1. Board1 with Stop 1 mode (@3.3V)
- Current Consumption during Stop 1 mode ~25-27 µA
- Current Consumption during run mode 1.7 mA

2. Board2 (@3.3V)
- Current consumption during run mode 1.7 mA

### <b>Keywords</b>

USART1, DMA, Stop 1, wakeup, autonomus, transmitter, receiver

### <b>Directory contents</b>

- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Inc/stm32u3xx_hal_conf.h        HAL configuration file
- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Inc/stm32u3xx_it.h              Interrupt handlers header file
- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Inc/main.h                      Header for main.c module
- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Src/stm32u3xx_it.c              Interrupt handlers
- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Src/main.c                      Main program
- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Src/stm32u3xx_hal_msp.c         HAL MSP module
- UART/UART_TwoBoards_ComDMA_stop1_autonomous/Src/system_stm32u3xx.c          STM32U3xx system source file

### <b>Hardware and Software environment</b>

- This example runs on STM32U3C5ZITxQ devices.
- This example has been tested with two NUCLEO-U3C5ZI-Q boards.
- Hardware setup:
  - Connect board 1 PA9 to board 2 PA10.
  - Connect board 1 PA10 to board 2 PA9.
  - Connect board 1 GND to board 2 GND.

### <b>How to use it ?</b>

- Open your preferred toolchain.
- Build the transmitter image with `TRANSMITTER_BOARD` enabled and program board 1.
- Build the receiver image with `TRANSMITTER_BOARD` disabled and program board 2.
- Run both boards.
- Press the user button on board 1 to start the transfer.
