## <b>Templates_TrustZoneDisabled Example Description</b>

- This project provides a reference template based on the STM32Cube HAL API that can be used
to build any firmware application when security is not enabled (TZEN=0).
- This project is targeted to run on STM32U345RC device on NUCLEO-U345RC-Q board from STMicroelectronics.
- The reference template project configures the maximum system clock frequency at 96Mhz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.
 3. The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
    from Flash memory and external memories, and reach the maximum performance.

### <b>Keywords</b>

Reference, Template, TrustZone disabled

### <b>Directory contents</b>

    - Templates/TrustZoneDisabled/TrustZoneDisabled.ioc        STM32CubeMX project description file
	- Templates/TrustZoneDisabled/Src/main.c                   Main program
	- Templates/TrustZoneDisabled/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
	- Templates/TrustZoneDisabled/Src/stm32u3xx_it.c           Interrupt handlers
	- Templates/TrustZoneDisabled/Src/stm32u3xx_hal_msp.c      HAL MSP module
	- Templates/TrustZoneDisabled/Inc/main.h                   Main program header file
	- Templates/TrustZoneDisabled/Inc/stm32u3xx_nucleo_conf.h  BSP Configuration file
	- Templates/TrustZoneDisabled/Inc/stm32u3xx_hal_conf.h     HAL Configuration file
	- Templates/TrustZoneDisabled/Inc/stm32u3xx_it.h           Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This template runs on STM32U345RC devices without security enabled (TZEN=0).
  - This template has been tested with STMicroelectronics NUCLEO-U345RC-Q (MB1841)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it?</b>

In order to make the program work, you must do the following :

 - Open TrustZoneDisabled.ioc with STM32CubeMX if you need to review or regenerate the template configuration
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


