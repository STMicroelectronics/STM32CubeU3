## <b>Initial_Attestation_KWE Application Description</b>

This application describes how to use the PSA Crypto opaque driver based on STM32
Key Wrape Engine to sign an entity attestation token (ETA) for initial attestation
using the device unique authentication key (DUA) in wrapped form.

This application is configured to use the Hardware library.

Two keys are supported, DUA_USER_FU for free usage and DUA_USER_LU for licensed usage.

Prerequisite:

 - Select which key to use DUA_USER_FU or DUA_USER_LU for the initial attestation.
 - Use STM32CubeProgrammer and the root security services (RSS) to export the
   device unique authentication key (DUA_FU or DUA_LU) in wrapped form.
 - Read protection (RDP) level and security level (secure, non secure) should be
   set up during the key export using STM32CubeProgrammer and the root security services (RSS).

This application is scheduled as follow:

  - Initialize the PSA Crypto Library.
  - Generate a challenge and fill the attestation token. The challenge could be replaced by one
    from the server.
  - Collect device information and fill the attestation token. The user should update information
    depending on their application.
  - Compute measurements by hashing the firmware data and fill the attestation token.
  - Compute the hash of the token, excluding the signature field.
  - Import the device user key (DUA_FU or DUA_LU) to sign the token. The DUA user key is already
    wrapped using the root security services (RSS).
  - Sign the token using the STM32 Key Wrap Engine and the device user key (DUA_FU or DUA_LU).
  - Optional: Export the public key from the device user key (DUA_FU or DUA_LU) to verify the
    token signature.
  - Optional: Import the public key and verify the token signature
  - Convert the public Key to PEM format.
  - Convert the entity attestation token (ETA) to PEM format.
  - Get DUA USER Certificate : DUA_USER_FU for free usage and DUA_USER_LU for licensed usage.
  - Optional: Compare the public key from the certificate with the exported public key from
    the wrapped key to avoid using DUA_USER_LU key with DUA_USER_FU certificate or DUA_USER_FU
    key with DUA_USER_LU certificate.
  - Convert the certificate to PEM format.
  - Destroy the keys to free up resources.

####  <b>Expected success behavior</b>

- The green led will be turned on
- The global variable glob_status will be set to PASSED

#### <b>Error behaviors</b>

- If the green led is off or green led will be toggled each 250 milliseconds in an infinity loop.
- The global variable glob_status will be set to FAILED

### <b>Keywords</b>

Cryptography, authentication, ECDSA, wrap, Cryptographic

### <b>Directory contents</b>

  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file

  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/stm32u3xx_hal_conf.h        HAL configuration file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/stm32u3xx_it.h              Interrupt handlers header file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/main.h                      Header for main.c module
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/storage_interface.h         Storage header file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/stm32_cert.h                STM32 certificate header file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/mbedtls_config.h            Mbed TLS configuration file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/mbedtls_alt_config.h        Mbed TLS Alt configuration file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Inc/kwe_config.h                Key Wrap Engine configuration file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Src/stm32u3xx_it.c              Interrupt handlers
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Src/stm32u3xx_hal_msp.c         HAL MSP module
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Src/main.c                      Main program
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Src/stm32_cert.c                STM32 certificate source file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Src/storage_interface.c         Storage source file
  - MbedTLS_HW_KWE/Initial_Attestation_KWE/Src/system_stm32u3xx.c          STM32U3xx system source file

### <b>Hardware and Software environment</b>

  - This application runs on STM32U3xx devices.

  - This application has been tested with NUCLEO-U385RG-Q board and can be
    easily tailored to any other supported device and development board.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Download the Rsse package from st.com : X-CUBE-RSSe (STM32CubeExpansion_RSSe_V2.0.0 or greater and update the path accordingly in export_DUA_key.bat).
 - Download the tool STM32CubeProgrammer software for all STM32 from st.com (V2.19.0 or greater and update the path accordingly in export_DUA_key.bat).

 - Run the script export_DUA_key.bat (to export the DUA wrapped private key and save it under Wrapped_Keys directory)

 - Reset the board.

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory

 - Set the board in RDP level 1 using the STM32CubeProgrammer.
 - Unplug and plug the board
 - Reset the board and verify the status of the Green LED.

 - To reopen the debug RDP must be set to level 0 using the STM32CubeProgrammer.

