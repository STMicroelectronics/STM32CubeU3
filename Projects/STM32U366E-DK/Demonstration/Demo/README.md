## <b>Demo Description</b>

The STM32Cube demonstration platform comes on top of STM32Cube as a firmware
package that offers a full set of software components based on a modular
architecture. All modules can be reused separately in standalone applications.

This demonstration is based on the STM32Cube HAL, BSP, and Touch Sensing
middleware components and runs on the STM32U366E-DK board.

### <b>Demonstration Overview</b>

At startup, once the "STM32U366E-DK DEMO" intro scroll completes, the supported
demos are shown on the glass LCD.

Use the joystick keys UP and DOWN to browse the demos. Use SEL or RIGHT to
launch the selected demo.

### <b>Menu Navigation</b>

Joystick actions:

- RIGHT or SEL: launch the selected application or enter a sub-menu.
- LEFT: return to the upper menu level or exit the running demo.
- DOWN: next menu item, for example switch temperature display to Fahrenheit.
- UP: previous menu item, for example switch temperature display to Celsius.

Note: in the current code, returning to the main menu is detected by a short
LEFT hold of about 250 ms, not by a multi-second press.

### <b>LEDs</b>

- LED1 is the green LED, LED2 is the red LED, and LED3 is the blue LED.
- During board-level hardware initialization, all three LEDs are turned ON.
- Entering the ultra-low-power demo turns all three LEDs OFF.
- In the TouchWakeup demo, LED3 turns ON when a touch is detected.
- In the TouchWakeup demo, LED3 turns OFF before the demo prints the STOP2
  entry message and re-enters STOP2.
- In the ultra-low-power demo, LED3 is used as the activity indicator while the
  ULP mode selection screen is active and after wakeup processing resumes.

### <b>LCD Behavior</b>

- LCD scrolling and explicit delays are blocking in this application flow.
- While a message is being scrolled or a delay is running, touch and joystick
  handling are not serviced by the demo logic.
- This is most visible in the TouchWakeup demo and during startup or intro text.

========================================
APPLICATIONS LIST
========================================

+ Temperature and Humidity Sensor Demo
========================================

The Temperature and Humidity Sensor module measures temperature and humidity
through the on-board I2C SHT40 sensor.

At demo entry, the application performs a short blank LCD scroll and then waits
about 2 seconds before the first value is shown. This delay is part of the
current implementation and explains why temperature does not appear
immediately after entering the demo from the main menu.

After startup delay, the application continuously reads sensor values and shows
them on the glass LCD:

- Temperature is displayed in Celsius or Fahrenheit.
- Humidity is displayed as percentage.
- UP selects Celsius.
- DOWN selects Fahrenheit.
- SEL toggles between temperature and humidity display.
- LEFT returns to the main menu after the short hold described above.

+ TouchWakeup/Touchsensing Demo
========================================

The TouchWakeup demo detects a touch on touch sensor button TS1.

The current LCD sequence is:

- " TOUCH SETUP "
- " TOUCH SETUP COMPLETE "
- " RUN MODE"

Touch detection should be considered active only after setup has completed and
the " RUN MODE" message has been printed.

The touch flow is:

1. Touch, comparator, RTC, and Touch Sensing middleware are initialized.
2. The demo runs its setup and calibration loop.
3. After setup completes, the demo enters run mode and starts the RTC wakeup
  timer with a 250 ms interval.
4. On wakeup, touch state is checked.
5. If touch is detected:
  - LED3 turns ON.
  - The LCD scrolls "WAKEUP TOUCH DETECTED".
  - The demo waits 5 seconds.
6. If touch is released after a detected touch:
  - LED3 turns OFF.
  - The LCD scrolls " STOP2 MODE ENTERING".
7. The MCU then re-enters STOP2 and waits for the next RTC wakeup.

Because the LCD scroll calls and the 5-second delay are blocking, touch and
joystick input are effectively not handled while those messages are being
printed.

LEFT returns to the main menu after the short hold described above, but only
when the demo loop is actively polling the joystick. While LCD messages are
scrolling, that polling is paused.

+ Ultra Low Power Demo
========================================

Users can switch between ULP modes using the UP and DOWN joystick keys. RIGHT
or SEL selects the highlighted mode.

The current ULP mode list in the project is:

- STDBY
- SLEEP
- LP SLP
- STOP2
- STOP3

The LCD first shows the selected mode name. When a mode is entered, the glass
LCD displays the typical current value associated with that mode in the current
source code:

- STDBY: 2 uA
- SLEEP: 188 uA
- LP SLP: 185 uA
- STOP2: 15 uA
- STOP3: 14 uA

Before entering the selected ULP mode, the demo configures the RTC wakeup timer
to 0xFFFF with DIV16, which corresponds to about 33 seconds.

In the current implementation, wakeup for the ULP demo is therefore driven by
the RTC timer configured for about 33 seconds. Returning to the main menu is
checked only after execution returns to the ULP menu loop after wakeup; it is
not serviced while the MCU is still inside the low-power mode.

LEFT returns to the main menu after the short hold described above, once the
demo is back in the ULP mode selection loop.

#### <b>Notes</b>

1. Care must be taken when using HAL_Delay(). This function provides accurate
  delay in milliseconds based on a variable incremented in the SysTick ISR.
  If HAL_Delay() is called from a peripheral ISR process, then the SysTick
  interrupt must have higher priority (numerically lower) than the peripheral
  interrupt. Otherwise the caller ISR process will be blocked.

2. The application must keep the SysTick time base at 1 millisecond to ensure
  correct HAL operation.

### <b>Keywords</b>

LCD, BSP, RTC, low power modes, touch sensing, temperature, humidity

### <b>Directory Contents</b>

- Demo/Src/main.c main program entry point
- Demo/Src/system_stm32u3xx.c STM32U3xx system clock configuration file
- Demo/Src/stm32u3xx_it.c interrupt handlers
- Demo/Src/stm32u3xx_hal_msp.c HAL MSP module
- Demo/Inc/main.h main program header file
- Demo/Inc/stm32u3xx_hal_conf.h HAL configuration file
- Demo/Inc/stm32u3xx_it.h interrupt handlers header file
- main_app/app_main.c demo menu and application implementation
- main_app/touchsensing.c touch middleware initialization wrapper

### <b>Hardware And Software Environment</b>

- This demonstration runs on STM32U366xx devices.
- This demonstration has been compiled with EWARM 9.20.1.
- This demonstration has been tested with the STMicroelectronics
  STM32U366E-DK board.

### <b>How To Use It</b>

To run the demonstration:

- Open the project in your preferred toolchain.
- Rebuild all files and load the image into target memory.
- Run the application.