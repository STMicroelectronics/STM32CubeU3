## <b>PKA_ECDSA_Sign Example Description</b>

How to compute a signed message regarding the Elliptic curve digital signature algorithm
(ECDSA).

For this example, a test vector have been extracted from National Institute of Standards and Technology (NIST)
 
 - Cryptographic Algorithm Validation Program (CAVP) in order to demonstrate the usage of the hal.
This reference files can be found under:
"http://csrc.nist.gov/groups/STM/cavp/documents/dss/186-3ecdsatestvectors.zip (SigGen.txt)"

This test vector has been chosen to demonstrate how to sign a message. The inputs and outputs definitions are included 
in SigGen.c. The output signature is composed of two parts called "R" and "S". You can refer to this file for more information.

The selected curve for this example is P-256 (ECDSA-256) published by NIST in 
Federal Information Processing Standards Publication FIPS PUB 186-4. The description
of this curve is present in file Src/prime256v1.c.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 96 MHz.

In case of success, the LD2 (GREEN) is ON.
In case of any error, the LD2 (GREEN) is toggling slowly.

#### <b>Notes</b>
  1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
   based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
   a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
   than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
   To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
  2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
   to have correct HAL operation.

### <b>Keywords</b>

Security, PKA, NIST, CAVP, ECDSA verification

### <b>Directory contents</b>
  
  - PKA/PKA_ECDSA_Sign/Inc/stm32u3xx_hal_conf.h    HAL configuration file
  - PKA/PKA_ECDSA_Sign/Inc/stm32u3xx_it.h          Interrupt handlers header file
  - PKA/PKA_ECDSA_Sign/Inc/main.h                  Header for main.c module
  - PKA/PKA_ECDSA_Sign/Src/stm32u3xx_it.c          Interrupt handlers
  - PKA/PKA_ECDSA_Sign/Src/main.c                  Main program
  - PKA/PKA_ECDSA_Sign/Src/stm32u3xx_hal_msp.c     HAL MSP module 
  - PKA/PKA_ECDSA_Sign/Src/system_stm32u3xx.c      STM32U3xx system source file
  - PKA/PKA_ECDSA_Sign/Src/prime256v1.c            Description of P-256 (ECDSA-256)
  - PKA/PKA_ECDSA_Sign/Inc/prime256v1.h            Header for prime256v1.c
  - PKA/PKA_ECDSA_Sign/Src/SigGen.c                Reflect the content of the test vector from SigGen.txt
  - PKA/PKA_ECDSA_Sign/Src/SigGen.txt              Extract from NIST CAVP
  - PKA/PKA_ECDSA_Sign/Inc/SigGen.h                Header of SigGen.c

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U385RGTxQ devices.
  
  - This example has been tested with an STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device 
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

