## <b>PWR_STANDBY Example Description</b>

How to enter the Standby mode and wake up from this mode by using an external reset or the WKUP pin.

In the associated software, the system clock is set to 96 MHz, an EXTI line
is connected to the pin PC.13 and configured to generate an
interrupt on rising edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LD2 is toggled in order to indicate whether the MCU is in Standby or Run mode.
Ultralow-power mode (ULPMEN) is also enabled to further reduce power consumption.

When a rising edge is detected on the EXTI line, an interrupt is generated and the system wakes up
the program then checks and clears the standby flag.
After clearing the standby flag, the software enables wake-up pin PWR_WAKEUP_LINE2 connected to PC.13, then
the corresponding flag indicating that a wakeup event was received from the PWR_WAKEUP_LINE2 is cleared.
Finally, the system enters again Standby mode causing LD2 to stop toggling.

Next, the user can wake-up the system in pressing the User push-button which is connected
to the wake-up pin PWR_WAKEUP_LINE2.The software then comes back in run mode for 5 sec.
A rising edge on WKUP pin will wake-up the system from Standby.
Alternatively, an external RESET of the board will lead to a system wake-up as well.The application needs to ensure that the SysTick

#### <b>Current Consumption(SMPS Enabled(@3.3V))</b>
- Current Consumption during STANDBY Mode - 0.265 uA
- Current Consumption during RUN Mode - 1890 uA

After wake-up from Standby mode, program execution restarts in the same way as after
a RESET and LD2 restarts toggling.

LD2 is used to monitor the system state as follows:

 - LD2 toggling: system in Run mode
 - LD2 off : system in Standby mode
 - LD2 ON: configuration failed (system will go to an infinite loop)

#### <b>Notes</b>
   1. To measure the current consumption in Standby mode, remove JP4 jumper
      and connect an amperemeter to JP4 to measure IDD current.

   2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

   3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset

### <b>Directory contents</b>

  - PWR/PWR_STANDBY/Inc/stm32u3xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_STANDBY/Inc/stm32u3xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32u3xx_it.h           Header for stm32u3xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                         Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32u3xx.c       STM32U3xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32u3xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                         Main program
  - PWR/PWR_STANDBY/Src/stm32u3xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b> 

  - This example runs on STM32U385RGTxQ devices

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U385RG-Q Set-up
    - LD2 connected to PA.05 pin
    - User push-button connected to pin PC.13 (External line 13)
    - WakeUp Pin PWR_WAKEUP_LINE2 connected to PC.13

### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
