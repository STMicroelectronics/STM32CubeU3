## <b>RCC_ClockConfig Example Description</b>

Configuration of the system clock (SYSCLK) and modification of the clock settings in Run mode, using the RCC HAL API.

In this example, after startup SYSCLK is configured to the max frequency using the
MSI as clock source, the User push-button (connected to External line 13) will be
used to change the source:
- from HSI to MSIS
- from MSIS to HSI

Each time the User push-button is pressed External line 13 interrupt is generated and in the ISR
the SYSCLK oscillator source is checked using __HAL_RCC_GET_SYSCLK_SOURCE() macro:
- If the MSIS oscillator is selected as SYSCLK source, the following steps will be followed to switch
   the SYSCLK source to HSI oscillator:
     a. Switch the system clock source to MSIS to allow modification of the SYSCLK configuration
     b. Enable HSI Oscillator, select it as SYSCLK source and finally activate the SYSCLK
     c. Select the SYSCLK as system clock source and configure bus clocks dividers
     d. Disable the MSIS oscillator (optional, if the MSIS is no more needed by the application)


- If the HSI oscillator is selected as SYSCLK source, the following steps will be followed to switch

   the SYSCLK source to MSIS oscillator:
     a. Switch the system clock source to HSI to allow modification of the SYSCLK configuration
     b. Enable MSIS Oscillator, select it as SYSCLK source and finally activate the SYSCLK
     c. Select the SYSCLK as system clock source and configure bus clocks dividers
     d. Disable the HSI oscillator (optional, if the HSI is no more needed by the application)


LD2 is toggled with a timing defined by the HAL_Delay() API.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>
RCC, System, Clock Configuration, MSIS bypass mode, HSI, System clock, Oscillator, SYSCLK
### <b>Directory contents</b>

    - RCC/RCC_ClockConfig/Inc/stm32u3xx_nucleo_conf.h   BSP configuration file
    - RCC/RCC_ClockConfig/Inc/stm32u3xx_hal_conf.h      HAL configuration file
    - RCC/RCC_ClockConfig/Inc/stm32u3xx_it.h            Interrupt handlers header file
    - RCC/RCC_ClockConfig/Inc/main.h                    Header for main.c module
    - RCC/RCC_ClockConfig/Src/stm32u3xx_it.c            Interrupt handlers
    - RCC/RCC_ClockConfig/Src/main.c                    Main program
    - RCC/RCC_ClockConfig/Src/system_stm32u3xx.c        STM32U3xx system source file
    - RCC/RCC_ClockConfig/Src/stm32u3xx_hal_msp.c       HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U366RETxQ devices.

  - This example has been tested with NUCLEO-U366RE-Q board WS rev A02 
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
