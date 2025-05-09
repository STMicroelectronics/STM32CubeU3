## <b>RCC_OutputSystemClockOnMCO Example Description</b>

Configuration of MCO pin (PA8) to output the system clock.

At start-up, User push-button and MCO pin are configured. The program configures SYSCLK
to the max frequency using with MSIS as clock source.

The signal on PA8 (pin 23 of CN10 connector) can be monitored with an oscilloscope
to check the different MCO configuration set at each User push-button press.

Different configuration will be observed :

 - SYSCLK frequency value divided by 1, hence around 96MHz.
 - HSI48 frequency value divided by 1, hence around 48MHz.
 - HSE frequency value divided by 1, hence around 16MHz.

When user press User push-button, a LD2 toggle is done to indicate a change in MCO config.

### <b>Keywords</b>

System, RCC, MSIS, HSI48, SYSCLK, HSE, Clock, Oscillator

### <b>Directory contents</b>

  - RCC/RCC_OutputSystemClockOnMCO/Inc/stm32u3xx_it.h        Interrupt handlers header file
  - RCC/RCC_OutputSystemClockOnMCO/Inc/main.h                  Header for main.c module
  - RCC/RCC_OutputSystemClockOnMCO/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_OutputSystemClockOnMCO/Src/stm32u3xx_it.c        Interrupt handlers
  - RCC/RCC_OutputSystemClockOnMCO/Src/main.c                  Main program
  - RCC/RCC_OutputSystemClockOnMCO/Src/system_stm32u3xx.c    STM32U3xx system source file


### <b>Hardware and Software environment</b> 

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect the MCO pin to an oscilloscope to monitor the different waveforms:
      - PA.08: connected to pin 23 of CN10 connector

### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
