## <b>RCC_MSIPLLMode Example Description</b>
How to change the multi-speed internal (MSI) RC oscillator frequency at run time with or wthout PLL mode,
using the PWR and RCC HAL API. MSI use LSE as it's PLL input.

Board: NUCLEO-U385RG-Q (embeds a STM32U385RGTxQ device)
Microcontroller Clock Output MCO1 Pin: PA.08

In this example, after startup SYSCLK is configured to the max frequency using the MSIS with
MSIS as clock source, the User push-button (connected to External line 13)
is used to change the MSIS frequency:
each time the User push-button is pressed, External line 13 interrupt is generated and the ISR
modifies the MSI PLL frequency. It uses both RC0 and RC1 oscillator with or without PLL.

The application cycles between three cases:
1. 96 MHz: the voltage range 1 is selected, the power booster is enabled.
   In this mode MSIS is used as SYSCLK.
   LD2 is emitting a double flash every half-second.
2. 48 MHz: the voltage range 2 is selected, the power booster is enabled.
   MSI RC0 oscillator is locked to LSE in PLL mode and it generates
   an accurate 48MHz.In this mode HSI is used as SYSCLK.
   LD2 is fast-toggling (every 100 ms).
3. 24 MHz: the voltage range 2 is selected, the power booster is disabled.
   MSI RC01 oscillator is locked to LSE in PLL mode and it generates
   an accurate 24MHz.In this mode HSI is used as SYSCLK.
   LD2 is slow-toggling (every second).

SYSCLK clock waveform is available on the MCO1 pin PA.08 and can be captured on an oscilloscope.

In case of configuration error, LD2 is on.


#### <b>Notes</b>
1.  Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

2.  The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

System, RCC, MSI, MSIS, SYSCLK, Clock, Oscillator

### <b>Directory contents</b>

  - RCC/RCC_MSIPLLMode/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - RCC/RCC_MSIPLLMode/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - RCC/RCC_MSIPLLMode/Inc/main.h                  Header for main.c module
  - RCC/RCC_MSIPLLMode/Src/stm32u3xx_it.c          Interrupt handlers
  - RCC/RCC_MSIPLLMode/Src/main.c                  Main program
  - RCC/RCC_MSIPLLMode/Src/system_stm32u3xx.c      STM32U3xx system source file
  - RCC/RCC_MSIPLLMode/Src/stm32u3xx_hal_msp.c     HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
