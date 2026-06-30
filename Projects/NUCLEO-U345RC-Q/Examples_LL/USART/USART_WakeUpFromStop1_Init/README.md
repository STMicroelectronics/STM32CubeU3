## <b>USART_WakeUpFromStop1_Init Example Description</b>
  
Configuration of GPIO and USART1 peripherals allows characters received on the USART_RX pin to wake up the MCU from Stop 1 low-power mode.
This example is based on the STM32U3xx USART1 LL API. The peripheral
initialization uses LL unitary service functions for optimization purposes (performance and size).

USART1 peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
In normal run mode, the system clock is based on MSIS at 3 MHz.
USART1 uses HSI16 as kernel clock to keep the wakeup capability active in Stop 1 mode.

Example execution:
After startup from reset and system configuration, LED2 is blinking quickly during 3 sec,
then MCU  enters "Stop 1" mode (LD1 off).
On first  character reception by the USART1 from PC Com port (ex: using HyperTerminal)
after "Stop 1" Mode period, MCU wakes up from "Stop 1" Mode.
Received character value is checked :
- On a specific value ('S' or 's'), LED2 is turned On and program ends.
- If different from 'S' or 's', program performs a quick LED2 blinks during 3 sec and
  enters again "Stop 1" mode, waiting for next character to wake up.

In case of errors, LED2 is slowly blinking (1 sec period).

### <b>Measured current consumption @3.3V</b>

The following values were measured on the target configuration used for this example:

  - Stop 1 mode current consumption: 229 uA to 232 uA with HSI16 kept available for USART wakeup capability.
  - Normal run mode current consumption: 365 uA to 370 uA with MSIS system clock running at 3 MHz.

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal,
Receiver, Asynchronous, Low Power, Wake Up

### <b>Directory contents</b> 

  - USART/USART_WakeUpFromStop1_Init/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - USART/USART_WakeUpFromStop1_Init/Inc/main.h                  Header for main.c module
  - USART/USART_WakeUpFromStop1_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_WakeUpFromStop1_Init/Src/stm32u3xx_it.c          Interrupt handlers
  - USART/USART_WakeUpFromStop1_Init/Src/main.c                  Main program
  - USART/USART_WakeUpFromStop1_Init/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U345RCTxQ devices.
    
  - This example has been tested with NUCLEO-U345RC-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U345RC-Q Set-up
    - Connect STM32 MCU board USART1 TX pin (GPIO PA9 connected to PA.09 connected to pin 2 in CN10)
      to PC COM port RX signal
    - Connect STM32 MCU board USART1 RX pin (GPIO PA10 connected to PA.10 connected to pin 4 in CN10)
      to PC COM port TX signal
    - Connect STM32 MCU board GND to PC COM port GND signal

    - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
      (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control). 

 ### <b>How to use it ?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
