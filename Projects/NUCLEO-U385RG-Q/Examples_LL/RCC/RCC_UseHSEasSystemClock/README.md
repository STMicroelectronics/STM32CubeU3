## <b>RCC_UseHSEasSystemClock Example Description</b>

Use of the RCC LL API to start the HSE and use it as system clock.

At start-up, MSIS clock is used as system clock (default clock after reset) and then Systick is 
configured at 1 ms after calculation of MSIS frequency.

Then, HSE is started and we wait for ready state. System clock is set to HSE thanks to IT 
triggered by HSE ready.

- LD2 is turned ON to indicate that MSIS is ready.
- LD2 is toggled with a timing of 1 second in case of error to switch SYSCLK to HSE or if HSE CSS
failure is detected (through NMI IT).


#### <b>Keywords</b>

System, RCC, SYSCLK, HSE, Clock, Oscillator, MSIS


### <b>Directory contents</b>

  - RCC/RCC_UseHSEasSystemClock/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - RCC/RCC_UseHSEasSystemClock/Inc/main.h                  Header for main.c module
  - RCC/RCC_UseHSEasSystemClock/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_UseHSEasSystemClock/Src/stm32u3xx_it.c          Interrupt handlers
  - RCC/RCC_UseHSEasSystemClock/Src/main.c                  Main program
  - RCC/RCC_UseHSEasSystemClock/Src/system_stm32u3xx.c      STM32U3xx system source file

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
