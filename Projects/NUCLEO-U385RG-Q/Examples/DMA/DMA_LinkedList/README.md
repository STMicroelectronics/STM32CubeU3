## <b>DMA_LinkedList Example Description</b>

This example describes how to use the DMA to perform a list of transfers. The transfer list is organized as linked-list,
each time the current transfer ends the DMA automatically reload the next transfer parameters, and starts it (without CPU intervention).

This project is targeted to run on STM32U385RGTxQ devices on NUCLEO-U385RG-Q board from STMicroelectronics.

At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals and initialize the systick used as 1ms HAL timebase.

Then the *SystemClock_Config()* function is used to configure the system clock (SYSCLK) to run at 96 MHz.

The Linked List is configured using the "DMA_LinkedListConfig" function. This function configures the DMA for a linked-list transfer which contains 3 nodes.
All nodes are filled using HAL function "HAL_DMAEx_LinkedList_BuildNode".

Nodes are connected to the linked-list using the HAL function "HAL_DMAEx_LinkedList_InsertNode_Tail".

After setting all nodes parameters using HAL_DMAEx_LinkedList_BuildNode()/HAL_DMAEx_LinkedList_InsertNode_Tail(),
start the transfer in interrupt mode using HAL_DMAEx_LinkedList_Start_IT() function.

Note that using the DMA, the transfer data length is always expressed in bytes whatever
the source and destination data size (byte, half word or word).

At the end, the main program waits for the end of DMA transmission or error interrupts: the DMA transmission ends
when all transfer nodes are served.

NUCLEO-U385RG-Q board's LED can be used to monitor the transfer status:

 - LD2 toggles when no error detected.

 - LD2 is ON when any error occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

DMA, Data Transfer, Memory to Memory, channel, SRAM

### <b>Directory contents</b>

File | Description
 --- | ---
DMA/DMA_LinkedList/Inc/stm32u3xx_nucleo_conf.h   | BSP configuration file
DMA/DMA_LinkedList/Inc/stm32u3xx_hal_conf.h      | HAL configuration file
DMA/DMA_LinkedList/Inc/stm32u3xx_it.h            | Header for stm32u3xx_it.c
DMA/DMA_LinkedList/Inc/main.h                      | Header for main.c module
DMA/DMA_LinkedList/Src/stm32u3xx_it.c            | Interrupt handlers
DMA/DMA_LinkedList/Src/main.c                      | Main program
DMA/DMA_LinkedList/Src/stm32u3xx_hal_msp.c       | HAL MSP module
DMA/DMA_LinkedList/Src/system_stm32u3xx.c        | stm32u3xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U385RGTxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-U385RG-Q
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
