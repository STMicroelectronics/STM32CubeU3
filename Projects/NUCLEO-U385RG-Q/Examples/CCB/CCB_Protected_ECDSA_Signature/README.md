## <b>CCB_Protected_ECDSA_Signature Example Description</b>

How to use the CCB peripheral to compute a signed message regarding the Elliptic curve digital signature algorithm using a special blob called "ECDSA key blob".

At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK).

The CCB peripheral configuration is ensured by the HAL_CCB_Init() function.
This function calls the HAL_CCB_MspInit() function, which implements the configuration
of the necessary CCB resources according to the hardware used (PKA, RNG and SAES) and enables the clock.

This example uses the Hardware unique key XOR software key(DHUK XOR BHK) as wrapping key.
The selected parameters for this example are an ECC 192 curve.
The related information is included in the ECDSAparam structure.
ECDSAKeyBlob will contain pointers to the reserved output buffers of IV, Tag, and the wrapped Private Key.
Calling HAL_CCB_ECDSA_WrapPrivateKey will wrap the user private key according to the ECDSA parameters.
Calling HAL_CCB_ECDSA_Sign will compute the signature.
The output signature is composed of two parts called "R" and "S".
Calling HAL_CCB_ECDSA_ComputePublicKey will compute the Public Key.
The output public is composed of two parts called "X" and "Y".
At last, the application will determine if a given signature is valid regarding the Elliptic curve digital signature algorithm.

STM32 board LED is used to monitor the example status:

  - LD2 is ON when the output signature is valid.
  - LD2 is slowly blinking when there is an error.

#### <b>Notes</b>

 1.  Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
     based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
     a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
     than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
     To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2.  The example needs to ensure that the SysTick time base is always set to 1 millisecond
     to have correct HAL operation.


### <b>Keywords</b>

Security, CCB, PKA, ECDSA Signature
### <b>Directory contents</b>

  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Inc/stm32u3xx_hal_conf.h        HAL configuration file
  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Inc/stm32u3xx_it.h              Interrupt handlers header file
  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Inc/main.h                      Header for main.c module
  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Src/stm32u3xx_it.c              Interrupt handlers
  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Src/main.c                      Main program
  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Src/stm32u3xx_hal_msp.c         HAL MSP module
  - CCB/CCB_Protected_ECDSA_PublicKeyComputation/Src/system_stm32u3xx.c          STM32U3xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.
  - This example has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
