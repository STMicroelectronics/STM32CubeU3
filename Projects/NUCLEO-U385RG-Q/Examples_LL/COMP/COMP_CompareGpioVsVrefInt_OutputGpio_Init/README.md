## <b>COMP_CompareGpioVsVrefInt_OutputGpio_Init Example Description</b>

How to use a comparator peripheral to compare a voltage level applied on
a GPIO pin to the internal voltage reference (VrefInt)
with comparator output connected to a GPIO pin.

This example is based on the STM32U3xx COMP LL API.
The peripheral initialization is done using LL unitary service functions
for optimization purposes (performance and size).

Example configuration:

Comparator instance COMP1 is configured to compare:

- input plus set to a GPIO pin (cf pin below) in analog mode.

- input minus set to internal voltage reference VrefInt (voltage level 1.2V,
  refer to device datasheet for min/typ/max values)
  
A GPIO pin a configured in alternate function to be connected directly
to comparator output (cf pin below).

Example execution:

From the main program execution, comparator is enabled.

Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing VrefInt voltage threshold (comparator input minus),
the comparator output set the logical level on its affected GPIO.

The program reads this GPIO state and reflects it on LED state.

LD2 is used to monitor program execution status:

- Normal operation:

  - LED turned on if comparator voltage level on input plus is higher than input minus
  - LED turned off if comparator voltage level on input plus is lower than input minus
  
- Error: LED remaining turned on

Connection needed:

Use an external power supply to generate a voltage in range [0V; 3.3V]
and connect it to analog input pin (cf pin below).

Note: If no power supply available, this pin can be let floating (in this case
      comparator output state will be undetermined).

Other peripherals used:

  1 GPIO for LD2
  
  1 GPIO for analog input: pin PA.02 (Arduino connector D1 pin 2 CN9)
  
  1 GPIO for digital output of comparator: PB.00 (Arduino connector A3 pin 4 CN8)

### <b>Keywords</b>

Comparator, COMP, analog, voltage, analog input, threshold, VrefInt

### <b>Directory contents</b>

  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/main.h                        Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Inc/stm32_assert.h                Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/stm32u3xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/main.c                        Main program
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio_Init/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.
    
  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
