## <b>RAMCFG_WriteProtection Example Description</b>

How to configure and use the RAMCFG HAL API to configure RAMCFG SRAM write
protection page.

This project is targeted to run on STM32U385RGTxQ devices on NUCLEO-U385RG-Q board from STMicroelectronics.

The project configures the maximum system clock frequency at 96 MHz.

When no SRAM2 is write-protected, all DMA transaction are performed. When a page is protection any master
access "DMA" is ignored and DMA error is generated.

  - All SRAM2 pages are unprotected : This example section performs DMA accesses to all SRAM and verifies that
    all access are allowed.
  - Half of SRAM2 pages are unprotected : This example section performs DMA access to only unprotected
    SRAM2 pages and verifies that accesses are allowed for unprotected pages and ignored for protected ones.
  - No SRAM2 pages are unprotected : This example section performs DMA accesses and all are ignored for all
    SRAM pages.

NUCLEO-U385RG-Q board's LED can be used to monitor the project operation status:

 - LD2 toggles when no error detected.
 - LD2 is ON when any project error was occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

RAMCFG, Memory, SRAM, Write Protection, DMA

### <b>Directory contents</b>

  - RAMCFG/RAMCFG_WriteProtection/Src/main.c                  Main program
  - RAMCFG/RAMCFG_WriteProtection/Src/system_stm32u3xx.c      STM32U3xx system clock configuration file
  - RAMCFG/RAMCFG_WriteProtection/Src/stm32u3xx_it.c          Interrupt handlers
  - RAMCFG/RAMCFG_WriteProtection/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - RAMCFG/RAMCFG_WriteProtection/Inc/main.h                  Main program header file
  - RAMCFG/RAMCFG_WriteProtection/Inc/stm32u3xx_hal_conf.h    HAL Configuration file
  - RAMCFG/RAMCFG_WriteProtection/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - RAMCFG/RAMCFG_WriteProtection/Inc/stm32u3xx_nucleo_conf.h  HAL configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

