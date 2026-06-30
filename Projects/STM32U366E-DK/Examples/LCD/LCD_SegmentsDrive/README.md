# <b>LCD_SegmentsDrive Example Description</b>

## Overview

This example demonstrates how to use the embedded LCD glass controller to drive the on-board LCD glass by Pacific Display Devices on the STM32U366VET6Q-DK board.

The main goal is to display the string **"U366-DK"** on the LCD glass in scrolling mode after waking up from STOP3 low-power mode.

---

## Features
- Displays **"STOP3"** on the LCD 1 second after startup.
- Automatically enters **STOP3 mode** while maintaining the "STOP3" display.
- Wakes up upon an external event on pin **PC.13** (configured with EXTI interrupt on falling edge).
- After wake-up, scrolls the **"U366-DK"** string on the LCD glass.
- Scroll repeats **5 times** at a fixed speed defined by `SCROLL_SPEED_LOW` (configured in `stm32u366e_discovery_glass_lcd.h`).
- Returns to RUN mode for 1 second before re-entering STOP3 mode.
- Uses the LCD display to indicate system states:
  - **"STOP3"** — system is in STOP3 mode.
  - **Scrolling "U366-DK"** — system is in RUN mode.

---

## System Configuration

- System clock is set to **48 MHz**.
- SysTick configured to generate an interrupt every **1 ms**.
- External interrupt configured on **PC.13** for wake-up.
- HAL_Delay() is used for timing delays based on SysTick.

---

## Important Notes

1. **HAL_Delay() Usage:**  
   HAL_Delay() depends on a variable incremented in the SysTick ISR. If HAL_Delay() is called inside a peripheral ISR, ensure the SysTick interrupt priority is higher (numerically lower) than the peripheral interrupt to avoid blocking. Use `HAL_NVIC_SetPriority()` to configure interrupt priorities accordingly.

2. **SysTick Time Base:**  
   The application must ensure that the SysTick time base is always set to 1 ms for correct HAL operation.

---

## <b>Keywords</b>

LCD, STOP3 mode, scrolling display, Low-power mode, STM32U366

---

## Directory Contents

| Path                                            | Description                      |
|-------------------------------------------------|--------------------------------|
| `LCD/LCD_SegmentsDrive/Inc/stm32u3xx_hal_conf.h` | HAL configuration file          |
| `LCD/LCD_SegmentsDrive/Inc/stm32u3xx_it.h`       | Interrupt handlers header file  |
| `LCD/LCD_SegmentsDrive/Inc/main.h`               | Header file for main.c          |
| `LCD/LCD_SegmentsDrive/Src/stm32u3xx_it.c`       | Interrupt handlers implementation |
| `LCD/LCD_SegmentsDrive/Src/main.c`               | Main program                   |
| `LCD/LCD_SegmentsDrive/Src/stm32u3xx_hal_msp.c`  | HAL MSP initialization          |
| `LCD/LCD_SegmentsDrive/Src/system_stm32u3xx.c`   | STM32U3xx system source file    |

---

## Hardware and Software Environment

- Target device: **STM32U366VET6Q**
- Tested on: **STMicroelectronics STM32U366VET6Q-DK** development board
- Easily adaptable to other supported STM32U3 series devices and boards.

---

## How to Use

1. Open your preferred IDE or toolchain.
2. Build the project to compile all source files.
3. Flash the compiled image onto the target device.
4. Run the example to observe the LCD behavior as described.
