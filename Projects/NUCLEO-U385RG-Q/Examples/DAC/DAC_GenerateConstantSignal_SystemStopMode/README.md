## <b>DAC_GenerateConstantSignal_SystemStopMode Example Description</b>

How to use the DAC peripheral to do a simple conversion with the MCU in STOP1 mode.
This example is based on the STM32U3xx DAC HAL API.

Example configuration:
The example uses the DAC for a simple conversion in 8 bits right
alignment of 0xFF value, the result of conversion can be seen by
connecting PA4(A2 Arduino CN8 pin 3, Morpho CN7 pin32) to an oscilloscope.

Example execution:
From the main program execution, DAC  is enabled and do a constant voltage.
- After 5 seconds, the MCU enter in STOP1 mode.
- The observed value remains at 3.3V
- Press user button to generate an interrupt and exit STOP1 mode.
- Repeat the example execution. 

LD2 is used to monitor program execution status:
- Normal operation:
  - LD2 turned on if the MCU is in Normal mode
  - LD2 turned off if the MCU is in STOP1 mode
- Error: LD2 remaining turned on

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Analog, DAC, Digital to Analog, Single conversion

### <b>Directory contents</b>

  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Inc/main.h                  Header for main.c module
  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Src/stm32u3xx_it.c          Interrupt handlers
  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Src/main.c                  Main program
  - DAC/DAC_GenerateConstantSignal_SystemStopMode/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.
    
  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
