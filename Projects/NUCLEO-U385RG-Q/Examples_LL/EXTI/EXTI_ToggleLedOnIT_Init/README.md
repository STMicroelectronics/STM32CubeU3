## <b>EXTI_ToggleLedOnIT_Init Example Description</b>

This example describes how to configure the EXTI and use
GPIOs to toggle the user LEDs available on the board when
a user button is pressed. This example is based on the
STM32U3xx LL API. Peripheral initialization is done using LL
initialization function to demonstrate LL init usage.

In this example, one EXTI line (External line 13) is configured to generate
an interrupt on each rising edge.

In the interrupt routine a led connected to a specific GPIO pin is toggled.

In this example:

    - External line 13 is connected to PC.13 pin
      - when rising edge is detected on External line 13 by pressing User push-button, LED2 toggles

On NUCLEO-U385RG-Q:

    - External line 13 is connected to User push-button


### <b>Keywords</b>

System, GPIO, Output, Alternate function, EXTI, Toggle

### <b>Directory contents</b>

  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/main.h                  Header for main.c module
  - EXTI/EXTI_ToggleLedOnIT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/stm32u3xx_it.c          Interrupt handlers
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/main.c                  Main program
  - EXTI/EXTI_ToggleLedOnIT_Init/Src/system_stm32u3xx.c      STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

