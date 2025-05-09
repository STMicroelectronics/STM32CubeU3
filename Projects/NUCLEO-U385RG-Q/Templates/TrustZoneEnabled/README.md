## <b>Templates_TrustZoneEnabled Example Description</b>

- This project provides a reference template based on the STM32Cube HAL API that can be used
to build any firmware application when TrustZone security is activated (Option bit TZEN=1).

- This project is targeted to run on STM32U385RGU device on NUCLEO-U385RG-Q board from STMicroelectronics.

- The reference template project configures the maximum system clock frequency at 96Mhz in secure
application.

#### <b>Description</b>

This project is composed of two sub-projects:

  - one for the secure application part (Project_s)
  - one for the non-secure application part (Project_ns).

Please remember that on system with security enabled, the system always boots in secure and
the secure application is responsible for launching the non-secure application.

This project mainly shows how to switch from secure application to non-secure application
thanks to the system isolation performed to split the internal Flash and internal SRAM memories
into two halves:

  - the first half for the secure application and
  - the second half for the non-secure application.

User Option Bytes configuration:

Please note the internal Flash is fully secure by default in TZEN=1 and User Option Bytes
Security Watermarks should be set according to the application configuration.
Here the proper User Option Bytes setup in line with the project linker/scatter
file is as follows:

    - TZEN=1
      NSBOOTADD0 = 0x08080000
    - SECWM1_PSTRT=0x0  SECWM1_PEND=0x7F  meaning all 128 pages of Bank1 set as secure
    - SECWM2_PSTRT=0x7F SECWM2_PEND=0x0   meaning no page of Bank2 set as secure, hence Bank2 non-secure

Any attempt by the non-secure application to access unauthorized code, memory or
peripheral generates a fault.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.
 3. When deleting MDK-ARM project folder and regenerating this application with STM32CubeMx tool, be sure to place the Veneer$$CMSE section into IROM2 “ROM_NSC_region”.
 4. When deleting the MDK-ARM project folder and regenerating the application with the STM32CubeMX tool, be sure to keep the Debug.ini file.
 5. The instruction cache (ICACHE) must be enabled by software to get a 0 wait-state execution
    from Flash memory and external memories, and reach the maximum performance.

### <b>Keywords</b>

Reference, Template, TrustZone

### <b>Directory contents</b>

	- Templates/TrustZoneEnabled/Secure/Src/main.c                      Secure Main program
	- Templates/TrustZoneEnabled/Secure/Src/secure_nsc.c                Secure Non-Secure Callable (NSC) module
	- Templates/TrustZoneEnabled/Secure/Src/stm32u3xx_hal_msp.c         Secure HAL MSP module
	- Templates/TrustZoneEnabled/Secure/Src/stm32u3xx_it.c              Secure Interrupt handlers
	- Templates/TrustZoneEnabled/Secure/Src/system_stm32u3xx_s.c        Secure STM32U3xx system init file
	- Templates/TrustZoneEnabled/Secure/Inc/main.h                      Secure Main program header file
	- Templates/TrustZoneEnabled/Secure/Inc/partition_stm32u385xx.h     Secure core configuration file
	- Templates/TrustZoneEnabled/Secure/Inc/stm32_assert.h    	 	      Secure assert file
	- Templates/TrustZoneEnabled/Secure/Inc/stm32u3xx_hal_conf.h        Secure HAL configuration file
	- Templates/TrustZoneEnabled/Secure/Inc/stm32u3xx_it.h              Secure Interrupt handlers header file
	- Templates/TrustZoneEnabled/Secure/Inc/stm32u3xx_nucleo_conf.h     Secure BSP configuration file
	- Templates/TrustZoneEnabled/Secure_nsclib/secure_nsc.h             Secure Non-Secure Callable (NSC) header file
	- Templates/TrustZoneEnabled/NonSecure/Src/main.c                   Non-secure Main program
	- Templates/TrustZoneEnabled/NonSecure/Src/stm32u3xx_hal_msp.c      Non-secure HAL MSP module
	- Templates/TrustZoneEnabled/NonSecure/Src/stm32u3xx_it.c           Non-secure Interrupt handlers
	- Templates/TrustZoneEnabled/NonSecure/Src/system_stm32u3xx_ns.c    Non-secure STM32U3xx system init file
	- Templates/TrustZoneEnabled/NonSecure/Inc/main.h                   Non-secure Main program header file
	- Templates/TrustZoneEnabled/NonSecure/Inc/stm32_assert.h    	    	Non-secure assert file
	- Templates/TrustZoneEnabled/NonSecure/Inc/stm32u3xx_hal_conf.h     Non-secure HAL Configuration file
	- Templates/TrustZoneEnabled/NonSecure/Inc/stm32u3xx_it.h           Non-secure Interrupt handlers header file
	- Templates/TrustZoneEnabled/NonSecure/Inc/stm32u3xx_nucleo_conf.h  Non-secure BSP configuration file


### <b>Hardware and Software environment</b>

  - This template runs on STM32U385CG devices with security enabled (TZEN=1).
  - This template has been tested with STMicroelectronics NUCLEO-U385RG-Q (MB1841)
    board and can be easily tailored to any other supported device
    and development board.

  - User Option Bytes requirement (with STM32CubeProgrammer tool)
        - TZEN = 1                            System with TrustZone-M enabled
        - SBOOT0 address : SBOOT0R = 0x0C000000
        - BOOT0 address for non-secure automatic VTOR update : NSBOOT0R = 0x08080000
        - Bank 1 watermark : SECWM1R1 : STRT=0x00  END=0x7F  meaning all pages of Flash bank 1 set as secured
        - Bank 2 watermark : SECWM2R1 : STRT=0x7F  END=0x00  meaning all pages of Flash bank 2 set as non-secured


### <b>How to use it?</b>

In order to make the program work, you must do the following :

 - Make sure that the system is configured with the security enable (TZEN=1) (option byte)


<b>IAR</b>

- Open your toolchain
- Open Multi-projects workspace file Project.eww
- Rebuild xxxxx_S project
- Rebuild xxxxx_NS project
- Set the "xxxxx_S" as active application (Set as Active)
- Flash the non-seure and secure binaries with Project->Download->Download active application
  (this shall download the \Secure_nsclib\xxxxx_S.out and \EWARM\xxxxx_NS\Exe\xxxxx_NS.out to flash memory)
- Run the example


<b>MDK-ARM</b>

- Open your toolchain
- Open Multi-projects workspace file Project.uvmpw
- Select the xxxxx_S project as Active Project (Set as Active Project)
- Build xxxxx_S project
- Select the xxxxx_NS project as Active Project (Set as Active Project)
- Build xxxxx_NS project
- Select the xxxxx_S project as Active Project (Set as Active Project)
- Load the secure binary (F8) (this shall download the \MDK-ARM\xxxxx_s\Exe\Project_s.axf and \MDK-ARM\xxxxx_ns\Exe\Project_ns.axf to flash memory)
- Run the example


<b>STM32CubeIDE</b>

 - Open STM32CubeIDE
 - File > Import. Point to the STM32CubeIDE folder of the example project. Click Finish.
 - Select and build the xxxxx_NS project, this will automatically trigger build of xxxxx_S project
 - Select the xxxxx_S project and select “Debug configuration”
   - Double click on “STM32 Cortex-M C/C++ Application”
   - Click on "Debugger" and select "ST-LINK (OpenOCD)" as Debug probe
   - Select  “Startup” >  “Add” >
 - Select the xxxxx_NS project
 - Build configuration : Select Release/Debug
   - Click Debug to debug the example


