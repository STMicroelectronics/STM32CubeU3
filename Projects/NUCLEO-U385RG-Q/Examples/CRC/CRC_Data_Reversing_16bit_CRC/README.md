## <b>CRC_Data_Reversing_16bit_CRC Example Description</b>

How to configure the CRC using the HAL API. The CRC (cyclic
redundancy check) calculation unit computes a 16-bit CRC code derived from a
buffer of 32-bit data (words). Input and output data reversal features are
enabled. The user-defined generating polynomial is manually set to 0x1021,
that is, X^16 + X^12 + X^5 + 1 which is the CRC-CCITT generating polynomial.

At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK).

The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK).
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

In this example, the user-defined generating polynomial is configured by
HAL_CRC_Init(). At the same time, input data reversal feature is set to bit
reversal on full word.
Output data reversal is enabled as well (only bit-level reversal option is available).
Additionally, the default init value is discarded and a user-defined one is used
instead.
It is specified finally that input data type is word.

The 16-bit long CRC is computed, stored in uwCRCValue variable then compared to the
CRC expected value stored in uwExpectedCRCValue_reversed variable.


For clarity's sake, a second CRC computation is then carried out with the input
and output data reversal options disabled, everything else being equal.

The input buffer used for this second CRC computation is derived from the
first input buffer by a bit-reversal operation carried out on full word (and vice versa).

It is checked that the same CRC is derived, taking into account that output data
reversal feature is disabled (i.e. the newly computed 16-bit CRC is equal to the
expected without reversal, whose value is stored in uwExpectedCRCValue variable).



STM32 board LED is used to monitor the example status:

  - LD2 is ON when the correct CRC value is calculated
  - LD2 is slowly blinking (1 sec. period) when there is an error in initialization or if an incorrect CRC value is calculated.

#### <b>Notes</b>

1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
   based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
   a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
   than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
   To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
   to have correct HAL operation.

### <b>Keywords</b>

Security, CRC, CRC Polynomial, CRC-CCITT, hardware CRC, Data reversing

### <b>Directory contents</b>

  - CRC/CRC_Data_Reversing_16bit_CRC/Inc/stm32u3xx_nucleo_conf.h           BSP configuration file
  - CRC/CRC_Data_Reversing_16bit_CRC/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - CRC/CRC_Data_Reversing_16bit_CRC/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - CRC/CRC_Data_Reversing_16bit_CRC/Inc/main.h                        Header for main.c module
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/stm32u3xx_it.c          Interrupt handlers
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/main.c                        Main program
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/stm32u3xx_hal_msp.c     HAL MSP module
  - CRC/CRC_Data_Reversing_16bit_CRC/Src/system_stm32u3xx.c      STM32U3xx system source file


### <b>Hardware and Software environment</b> 

  - This example runs on STM32U385RGTxQ devices.
  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b> 

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
