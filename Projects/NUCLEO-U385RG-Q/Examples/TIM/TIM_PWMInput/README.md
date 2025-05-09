## <b>TIM_PWMInput Example Description</b>

How to use the TIM peripheral to measure the frequency and 
duty cycle of an external signal.

The TIM2CLK frequency is set to SystemCoreClock (Hz), the Prescaler is 0 so the 
counter clock is SystemCoreClock (Hz).

SystemCoreClock is set to 96 MHz for STM32U385RGTxQ Devices.

TIM2 is configured in PWM Input Mode: the external signal is connected to 
TIM2 Channel2 used as input pin.
To measure the frequency and the duty cycle, we use the TIM2 CC2 interrupt request,
so in the timer IRQ routine (via call to function HAL_TIM_IC_CaptureCallback() ),
the frequency and the duty cycle of the external signal are computed.

"uwFrequency" variable contains the external signal frequency:

TIM2 counter clock = SystemCoreClock,

uwFrequency = TIM2 counter clock / TIM2_CCR2 in Hz, 

"uwDutyCycle" variable contains the external signal duty cycle:

uwDutyCycle = (TIM2_CCR1*100)/(TIM2_CCR2) in %.

The minimum frequency value to measure is (TIM2 counter clock / CCR MAX)

                                         = (96 MHz)/ 65535

In case of error, LD2 is turned ON.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Timer, TIM, Input, signals, PWM, External signal, Frequency, Duty cycle, Measure

### <b>Directory contents</b>

  - TIM/TIM_PWMInput/Inc/stm32u3xx_nucleo_conf.h    BSP configuration file
  - TIM/TIM_PWMInput/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMInput/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMInput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMInput/Src/stm32u3xx_it.c          Interrupt handlers
  - TIM/TIM_PWMInput/Src/main.c                  Main program
  - TIM/TIM_PWMInput/Src/stm32u3xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMInput/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U385RG-Q Set-up
    - Connect the external signal to measure to the TIM2 CH2 pin (PA1) (pin 30 in CN7 connector).

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

