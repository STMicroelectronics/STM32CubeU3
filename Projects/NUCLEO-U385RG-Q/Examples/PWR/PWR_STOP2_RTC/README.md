## <b>PWR_STOP2_RTC Example Description</b>

How to enter the Stop 2 mode and wake-up from this mode using an external reset
or RTC wakeup timer.
It allows to measure the current consumption in Stop 2 mode with RTC enabled.

In the associated software, the system clock is set to 96 MHz and the SysTick is
programmed to generate an interrupt each 1 ms.
The Low Speed Internal (LSI) clock is used as RTC clock source by default.
RTC interrupt is internally connected to the RTC Wakeup event.

The system automatically enters Stop 2 mode 5 sec. after start-up. The RTC wake-up
is configured to generate an interrupt on rising edge about 33 sec. afterwards.
Current consumption in Stop 2 mode with RTC feature enabled can be measured during that time.
More than half a minute is chosen to ensure current convergence to its lowest operating point.

After wake-up from Stop 2 mode, program execution is resumed.

LD2 is used to monitor the system state as follows:

 - LD2 toggling: system in Run mode
 - LD2 off : system in Stop 2 mode

These steps are repeated in an infinite loop.

#### <b>Notes</b>

1- To measure the current consumption in Stop 2 mode, remove JP4 jumper
      and connect an amperemeter to JP4 to measure IDD current.

2- This example can not be used in DEBUG mode due to the fact
      that the Cortex-M33 core is no longer clocked during low power mode
      so debugging features are disabled.

3- Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
      the RTC clock source; in this case the Backup domain will be reset in
      order to modify the RTC Clock source, as consequence RTC registers (including
      the backup registers) and RCC_CSR register are set to their reset values.

4- Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

5- The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

6- A current leakage can be observed while supplying the MCU from an external source and not supplying
      the STLink (unplugged). To fix this leakage, you can configure the pin PA13 as Analog, however, the
      connection with the debugger is lost.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption (Stop 2 + RTC) - 5.07 uA
- Current Consumption during RUN Mode - 1890 uA

### <b>Keywords</b>

Power, PWR, Stop 2 mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset

### <b>Directory contents</b>

  - PWR/PWR_STOP2_RTC/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_STOP2_RTC/Inc/stm32u3xx_conf.h         HAL Configuration file
  - PWR/PWR_STOP2_RTC/Inc/stm32u3xx_it.h           Header for stm32u3xx_it.c
  - PWR/PWR_STOP2_RTC/Inc/main.h                         Header file for main.c
  - PWR/PWR_STOP2_RTC/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
  - PWR/PWR_STOP2_RTC/Src/stm32u3xx_it.c           Interrupt handlers
  - PWR/PWR_STOP2_RTC/Src/main.c                         Main program
  - PWR/PWR_STOP2_RTC/Src/stm32u3xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U385RG-Q Set-up
    - LD2 connected to PA.05 pin

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Once the image is loaded, power off the NUCLEO board in unplugging
   the power cable then power on the board again
 - Run the example