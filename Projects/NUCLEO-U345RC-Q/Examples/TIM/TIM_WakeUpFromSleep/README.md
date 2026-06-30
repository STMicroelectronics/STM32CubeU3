## <b>TIM_WakeUpFromSleep Example Description</b>

How to enter the Sleep mode and wake up from this mode by using the timer's update interrupt.

In the associated software, the system clock is set to 96 MHz.
A timer configured to generate an interrupt every one second.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LD2 is toggled in order to indicate whether the MCU is in SLEEP mode
or RUN mode.

5 seconds after start-up, time base generation in interrupt mode is started and 
the system automatically enters SLEEP mode. LD2 stops toggling.
After one second, the timer's update interrupt is generated and the system is woken-up.
The software then comes back in RUN mode for 5 sec. before automatically re-starting the timer
and entering SLEEP mode again.

LD2 is used to monitor the system state as follows:

 - LD2 toggling: system in RUN mode
 - LD2 off : system in SLEEP mode

#### <b>Notes</b>

 1. These steps are repeated in an infinite loop.

 3. To measure the current consumption in SLEEP mode, remove JP4 jumper
    and connect an amperemeter to JP4 to measure IDD current.

 2. This example can not be used in DEBUG mode due to the fact
    that the Cortex-M33 core is no longer clocked during low power mode
    so debugging features are disabled.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, Timer, Update interrupt, Sleep mode, Interrupt, Wakeup, External reset

### <b>Directory contents</b>

    - TIM/TIM_WakeUpFromSleep/Inc/stm32u3xx_nucleo_conf.h  BSP configuration file
    - TIM/TIM_WakeUpFromSleep/Inc/stm32u3xx_conf.h         HAL Configuration file
    - TIM/TIM_WakeUpFromSleep/Inc/stm32u3xx_it.h           Header for stm32u3xx_it.c
    - TIM/TIM_WakeUpFromSleep/Inc/main.h                   Header file for main.c
    - TIM/TIM_WakeUpFromSleep/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
    - TIM/TIM_WakeUpFromSleep/Src/stm32u3xx_it.c           Interrupt handlers
    - TIM/TIM_WakeUpFromSleep/Src/stm32u3xx_hal_msp.c      HAL MSP module
    - TIM/TIM_WakeUpFromSleep/Src/main.c                   Main program

### <b>Hardware and Software environment</b>

  - This example runs on STM32U345RCTxQ devices

  - This example has been tested with STMicroelectronics NUCLEO-U345RC-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U345RC-Q set-up:
    - LD2 connected to PA.05 pin

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
