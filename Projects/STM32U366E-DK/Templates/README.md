## <b>Templates Example Description</b>

This projects provides a reference template that can be used to build any firmware application.

This directory provides a reference template project that can be used to build any firmware application for
STM32U366xx devices using STM32CubeU3 HAL and running on STM32U336E-DK board from STMicroelectronics. 

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Directory contents</b>

  - Templates/Src/main.c                 Main program
  - Templates/Src/system_stm32u3xx.c     STM32U3xx system clock configuration file
  - Templates/Src/stm32u3xx_it.c         Interrupt handlers 
  - Templates/Src/stm32u3xx_hal_msp.c    HAL MSP module
  - Templates/Inc/main.h                 Main program header file  
  - Templates/Inc/stm32u3xx_hal_conf.h   HAL Configuration file
  - Templates/Inc/stm32u3xx_it.h         Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U366xx devices.

  - This example has been compiled on EWARM 9.40.1
  
  - This example has been tested with STMicroelectronics STM32U336E-DK (100 pins)
    board embedding a multispeed clock MSI and can be easily tailored to any other
    supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
