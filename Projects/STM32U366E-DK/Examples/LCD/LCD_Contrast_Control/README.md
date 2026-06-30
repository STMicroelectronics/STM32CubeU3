## <b>LCD_Contrast_Control Example Description</b>

How to use the embedded LCD glass controller and how to set the LCD
Contrast level.

After Reset the Contrast level is equal to LCD_CONTRASTLEVEL_0 and LCD Glass display "DK0016" string.
By pressing on the Joystick Sel push-button (PC.03) the Contrast level is incremented
and the message displayed on the Pacific Display LCD glass is proportional
to the contrast level.

In this example, 8 different contrasts are used:

  - LCD_CONTRASTLEVEL_0                               Maximum Voltage = 2.60V
  - LCD_CONTRASTLEVEL_1                               Maximum Voltage = 2.73V
  - LCD_CONTRASTLEVEL_2                               Maximum Voltage = 2.86V
  - LCD_CONTRASTLEVEL_3                               Maximum Voltage = 2.99V
  - LCD_CONTRASTLEVEL_4                               Maximum Voltage = 3.12V
  - LCD_CONTRASTLEVEL_5                               Maximum Voltage = 3.26V
  - LCD_CONTRASTLEVEL_6                               Maximum Voltage = 3.40V
  - LCD_CONTRASTLEVEL_7                               Maximum Voltage = 3.55V

### <b>Note</b>
   Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

### <b>Note</b>
The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>
LCD, Joystick, contrast level, STM32U366

### <b>Directory contents</b>

  - LCD/LCD_Contrast_Control/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - LCD/LCD_Contrast_Control/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - LCD/LCD_Contrast_Control/Inc/main.h                  Header for main.c module
  - LCD/LCD_Contrast_Control/Src/stm32u3xx_it.c          Interrupt handlers
  - LCD/LCD_Contrast_Control/Src/main.c                  Main program
  - LCD/LCD_Contrast_Control/Src/stm32u3xx_hal_msp.c     HAL MSP file
  - LCD/LCD_Contrast_Control/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U3xx devices.

  - This example has been tested with STMicroelectronics STM32U366E-DK
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
