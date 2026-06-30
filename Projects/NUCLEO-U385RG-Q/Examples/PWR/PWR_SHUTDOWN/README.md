## <b>PWR_SHUTDOWN Example Description</b>

How to enter the system in SHUTDOWN mode and wake-up from this
mode using external RESET or WKUP line.

In the associated software, the system clock is set to 96 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an 
interrupt on rising edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LD2 is toggled in order to indicate whether the MCU is in SHUTDOWN or RUN mode.

When the User push-button is pressed a rising edge is detected on the EXTI line and
an interrupt is generated. In the EXTI handler routine, the wake-up line PWR_WAKEUP_LINE2 
is enabled and the corresponding wake-up flag cleared. 
Then, the system enters SHUTDOWN mode causing LD2 to stop toggling. 

Next, the user can wake-up the system in pressing the User push-button which is connected
to the wake-up line PWR_WAKEUP_LINE2.
A rising edge on the wake-up line wakes-up the system from SHUTDOWN.
Alternatively, an external RESET of the board leads to a system wake-up as well.

After wake-up from SHUTDOWN mode, program execution restarts from the beginning.
Exit from SHUTDOWN is detected through a flag set in an RTC back-up register; in
such a case, LD2 is kept on for about 5 sec. before toggling again.

User can re-enter SHUTDOWN in pressing again the User push-button.

LD2 is used to monitor the system state as follows:

 - LD2 toggling: system in RUN mode
 
 - LD2 is on for about 5 sec.: system is restarting and out-of-shutdown has been detected
 
 - LD2 off : system in SHUTDOWN mode
 
These steps are repeated in an infinite loop.

#### <b>Notes</b>

 1. To measure the current consumption in SHUTDOWN mode, remove JP4 jumper 
    and connect an amperemeter to JP4 to measure IDD current.     

 2. This example can not be used in DEBUG mode, this is due to the fact 
    that the Cortex-M0+ core is no longer clocked during low power mode 
    so debugging features are disabled

 3. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then 
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption during SHUTDOWN Mode - 0.343 uA
- Current Consumption during RUN Mode - 1890 uA

### <b>Keywords</b>

Power, PWR, EXTI, Shutdown mode, Interrupt, Wakeup, External reset, WKUP line

### <b>Directory contents</b>

  - PWR/PWR_SHUTDOWN/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_SHUTDOWN/Inc/stm32u3xx_conf.h         HAL Configuration file
  - PWR/PWR_SHUTDOWN/Inc/stm32u3xx_it.h           Header for stm32u3xx_it.c
  - PWR/PWR_SHUTDOWN/Inc/main.h                   Header file for main.c
  - PWR/PWR_SHUTDOWN/Src/system_stm32u3xx.c       STM32u3xx system clock configuration file
  - PWR/PWR_SHUTDOWN/Src/stm32u3xx_it.c           Interrupt handlers
  - PWR/PWR_SHUTDOWN/Src/main.c                   Main program

### <b>Hardware and Software environment</b>

  - NUCLEO-U385RG-Q set-up:
    - LD2 connected to PA.05 pin
    - User push-button connected to pin PC.13 (External line 13)
    - WakeUp line PWR_WAKEUP_LINE2 connected to PC.13

  - This example runs on STM32U3xx devices
      
  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example