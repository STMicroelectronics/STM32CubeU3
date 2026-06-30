## <b>PWR_RUN_SMPS Example Description</b>

How to use the SMPS PWR regulator.

Switching to the SMPS regulator provides lower consumption in particular at high VDD
voltage level.

In the associated software, the system clock is set to 96 MHz and the SysTick is
programmed to generate an interrupt each 1 ms.

After BOR0 power-on reset and system reset, the LDO regulator is enabled, in range 2.
The example switches first to SMPS before changing to the voltage range 1.

The JP4 jumper can be remove of the board to current consumption
measurement Idd with ammeter.

LD2 is used to monitor the system state as following:

 - LD2 ON: configuration failed (system will go to an infinite loop)
 - LD2 toggling: system in Run mode with SMPS

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
Current Consumption during RUN Mode - 1890 uA

### <b>Keywords</b>

Power, PWR, SMPS, LDO, Voltage range

### <b>Directory contents</b>

  - PWR/PWR_SMPS/Inc/stm32u3xx_nucleo_conf.h BSP configuration file
  - PWR/PWR_SMPS/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - PWR/PWR_SMPS/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - PWR/PWR_SMPS/Inc/main.h                  Header for main.c module
  - PWR/PWR_SMPS/Src/stm32u3xx_it.c          Interrupt handlers
  - PWR/PWR_SMPS/Src/stm32u3xx_hal_msp.c     HAL MSP file
  - PWR/PWR_SMPS/Src/main.c                  Main program
  - PWR/PWR_SMPS/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


