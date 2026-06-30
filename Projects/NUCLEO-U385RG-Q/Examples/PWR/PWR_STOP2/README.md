## <b>PWR_STOP2 Example Description</b>

This example shows how to enter the system in Stop 2 mode and wake-up from this
mode using external RESET or wake-up interrupt.

In the associated software, the system clock is set to 96 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an
interrupt on rising edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LD2 is toggled in order to indicate whether the MCU is in Stop 2 mode
or RUN mode.

5 seconds after start-up, the system automatically enters Stop 2 mode and
LD2 stops toggling.

The User push-button can be pressed at any time to wake-up the system.
The software then comes back in RUN mode for 5 sec. before automatically entering Stop 2 mode again.

LD2 is used to monitor the system state as follows:

 - LD2 toggling: system in RUN mode

 - LD2 off : system in Stop 2 mode

 - LD2 on: configuration failed (system will go to an infinite loop)

These steps are repeated in an infinite loop.

#### <b>Notes</b>

 1- To measure the current consumption in Stop 2 mode, remove JP4 jumper
      and connect an amperemeter to JP4 to measure IDD current.

 2- This example can not be used in DEBUG mode due to the fact
      that the Cortex-M33 core is no longer clocked during low power mode
      so debugging features are disabled.

 3- Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4- The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 5- A current leakage can be observed while supplying the MCU from an external source and not supplying
      the STLink (unplugged). To fix this leakage, you can configure the pin PA13 as Analog, however, the
      connection with the debugger is lost.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption (Stop 2) - 4.43 uA
- Current Consumption during RUN Mode - 1890 uA

### <b>Keywords</b>

Power, PWR, Stop 2 mode, Interrupt, EXTI, Wakeup, Low Power,

### <b>Directory contents</b>

  - PWR/PWR_STOP2/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_STOP2/Inc/stm32u3xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP2/Inc/stm32u3xx_it.h           Header for stm32u3xx_it.c
  - PWR/PWR_STOP2/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP2/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
  - PWR/PWR_STOP2/Src/stm32u3xx_it.c           Interrupt handlers
  - PWR/PWR_STOP2/Src/main.c                   Main program
  - PWR/PWR_STOP2/Src/stm32u3xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U3xx devices

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U385RG-Q set-up:
    - LD2 connected to PA.05 pin
    - User push-button connected to pin PC.13 (External line 13)


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain

 - Rebuild all files and load your image into target memory

 - Run the example