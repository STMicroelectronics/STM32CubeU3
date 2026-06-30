## <b>PWR_STOP3_RTC Example Description</b>

This example shows how to enter the system in Stop 3 mode and wake-up from this
mode using the RTC Internal wake-up interrupt.

In the associated software, the system clock is set to 96 MHz and RTC is configured to wake-up the CPU.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LD2 is toggled in order to indicate whether the MCU is in Stop 3 mode
or RUN mode.

2 seconds after start-up, the system automatically enters Stop 3 mode and
LD2 stops toggling.

The CPU stays in Stop 3 mode until the RTC triggers the Internal WakeUp. The delay is manageable by changing the RTC_WAKEUP_DELAY define in main.c.

The software then comes back in RUN mode for 2 sec. before automatically entering Stop 3 mode again.

LD2 is used to monitor the system state as follows:
 - LD2 toggling: system in RUN mode
 - LD2 OFF : system in Stop 3 mode
 - LD2 ON : configuration failed (system will go to an infinite loop)

These steps are repeated in an infinite loop.

**Note:** To measure the current consumption in Stop 3 mode, remove JP5 jumper
      and connect an amperemeter to JP5 to measure IDD current.

**Note:** This example can not be used in DEBUG mode due to the fact
      that the Cortex-M33 core is no longer clocked during low power mode
      so debugging features are disabled.

**Note:** Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

**Note:** The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption (Stop 3 + RTC) - 2.88 uA
- Current Consumption during RUN Mode - 1890 uA

### <b>Keywords</b>

Power, PWR, Stop 3 mode, Interrupt, Wakeup, Low Power, RTC

### <b>Directory contents</b>

  - PWR/PWR_STOP3_RTC/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_STOP3_RTC/Inc/stm32u3xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP3_RTC/Inc/stm32u3xx_it.h           Header for stm32u3xx_it.c
  - PWR/PWR_STOP3_RTC/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP3_RTC/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
  - PWR/PWR_STOP3_RTC/Src/stm32u3xx_it.c           Interrupt handlers
  - PWR/PWR_STOP3_RTC/Src/main.c                   Main program
  - PWR/PWR_STOP3_RTC/Src/stm32u3xx_hal_msp.c      HAL MSP module

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

 - UnPlug then Plug STLINK connection to perform a power-on-reset