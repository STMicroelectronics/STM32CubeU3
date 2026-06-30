## <b>OEMiROT_Appli Template Description</b>

This project provides a OEMiROT boot path Full Secure reference template. Boot is performed through OEMiROT boot path after authenticity and the integrity checks of the project firmware and data
images.

Please remember that on system with security enabled, the system always boots in secure. When the secure application is started the MPU
is already configured (by OEMiROT) to limit the execution area to the project firmware execution slot. This is done in order to avoid
any malicious code execution from an unauthorised area (RAM, out of execution slot in user flash ...). Once started, it is up to the secure
application to adapt the security configuration to its needs. In this template, the MPU is simply disabled.

### <b>Keywords</b>

TrustZone, OEMiROT, Boot path, Root Of Trust, Security, MPU

### <b>Directory contents</b>

  - OEMiROT_Appli/Secure_nsclib/appli_flash_layout.h                  Flash mapping for appli S
  - OEMiROT_Appli/Secure_nsclib/appli_region_defs.h                   RAM and FLASH regions definitions for appli S

  - OEMiROT_Appli/Inc/main.h                                          Header file for main.c
  - OEMiROT_Appli/Inc/stm32u3xx_hal_conf.h                            HAL configuration file
  - OEMiROT_Appli/Inc/stm32u3xx_it.h                                  Header file for stm32u3xx_it.h
  - OEMiROT_Appli/Inc/stm32u3xx_nucleo_conf.h                         Discovery board configuration file

  - OEMiROT_Appli/Src/main.c                                          Secure Main program
  - OEMiROT_Appli/Src/stm32u3xx_hal_msp.c                             Secure HAL MSP module
  - OEMiROT_Appli/Src/stm32u3xx_it.c                                  Secure Interrupt handlers
  - OEMiROT_Appli/Src/system_stm32u3xx.c                              Secure STM32U3xx system clock configuration file


### <b>Hardware and Software environment</b>

  - This template runs on STM32U366xx devices with security enabled (TZEN=B4).
  - This template has been tested with STMicroelectronics NUCLEO-U366RE-Q (MB1841)

### <b>How to use it ?</b>

<u>Before compiling the project, you should first start the provisioning process</u>. During the provisioning process, the linker files
will be automatically updated.

Before starting the provisioning process, select the application project to use (application example or template),
through oemirot_appli_path_project variable in ROT_Provisioning/env.bat or env.sh.
Then start provisioning process by running provisioning.bat (.sh) from ROT_Provisioning/OEMiROT or ROT_Provisioning/OEMiROT_OEMuROT,
and follow its instructions. Refer to ROT_Provisioning/OEMiROT or ROT_Provisioning/OEMiROT_OEMuROT readme for more information on
the provisioning process for OEMiROT boot path or OEMiROT_OEMuROT boot path.

After application startup, check if the LED is blinking.

For more details, refer to STM32U3 Wiki articles:

  - [STM32U3 security](https://wiki.st.com/stm32mcu/wiki/Category:STM32U3).
  - [OEMiRoT OEMuRoT for STM32U3](https://wiki.st.com/stm32mcu/wiki/Security:OEMiRoT_OEMuRoT_for_STM32U3).
  - [How to start with OEMiROT on STM32U3](https://wiki.st.com/stm32mcu/wiki/Security:How_to_start_with_OEMiRoT_on_STM32U3).

#### <b>Note:</b>

  1. The most efficient way to develop and debug an application is to boot directly on user flash in RDL level 0 by setting with
     STM32CubeProgrammer the RDP to 0xAA and the SECBOOTADD to (0x0C006000 + offset of the firmware execution slot).

  2. **AppliCfg.py** (located in Utilities/PC_Software/ROT_AppliConfig) is used during provisioning
     process. It is needed to have python and some python packages installed in your
     environment. Refer to Utilities/PC_Software/ROT_AppliConfig/README.md for more details.
