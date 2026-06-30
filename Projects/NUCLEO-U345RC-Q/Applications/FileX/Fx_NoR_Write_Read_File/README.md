
## <b>Fx_Nor_Write_Read_File Application Description</b>

This application provides an example of Azure RTOS FileX and LevelX stacks usage on custom
board. By default, it does not run on the NUCLEO-U345RC-Q board, the user must add a custom board with NOR Flash support.
It demonstrates how to create a Fat File system
on the NOR flash using FileX alongside LevelX. The application is designed to execute file operations on the Winbond W25Q128JV
NOR flash device, the code provides all required software code for properly managing it.

 - This application has been tested using a MIKROE Flash 6 Click board embedding a Winbond W25Q128JV NOR flash device configured as below:

    - MemoryDataWidth = 1 bit
    - PageSize = 256 bytes
    - BlockSize = 64 KB
    - BlockNbr = 256
    - Sector Size: = 4 KB
    - Number of Sectors = 4096

  - User can design his own hardware according to the following SPI1 GPIO configuration:

    - PA0    ------> W25Q128J_CS
    - PA7    ------> SPI1_MOSI
    - PA1    ------> SPI1_SCK
    - PA6    ------> SPI1_MISO

The application starts by calling the ThreadX's initialization routine which executes the main thread that handles file operations.
At this stage, all FileX resources are created, the NOR flash is initialized and a single thread is created:

  - fx_app_thread (Prio : 10; PreemptionPrio : 10) used for file operations.

The fx_app_thread will start by formatting the NOR Flash using FileX services. The resulting file system is a FAT32 compatible, with 512 bytes per sector and 8 sectors per cluster.

Upon successful opening of the flash media, FileX continues with creating a file called "STM32.TXT" into the root directory, then writes into it
some dummy data. Then file is re-opened in read only mode and content is checked.

Through all the steps, FileX/LevelX services are called to print the flash size available before and after the example file is written into the flash.
The number of occupied sectors is also shown.

#### <b>Expected success behavior</b>

Successful operation is marked by a toggling green LED light.
Also, information regarding the total and available size of the flash media is printed to the serial port.

#### <b> Error behaviors</b>

On failure, the green LED toggles at an accelerated rate of every 100 ms, and a failure message is displayed on the serial port.

#### <b>Assumptions if any</b>

None

#### <b>Known limitations</b>

None

### <b>Notes</b>

None

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL).
 - Using dynamic memory allocation requires to apply some changes to the linker file.
   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
   using the "first_unused_memory" argument.
   This requires changes in the linker files to expose this memory location.
    + For EWARM add the following section into the .icf file:
     ```
     place in RAM_region    { last section FREE_MEM };
     ```
    + For MDK-ARM:
    ```
    either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in "tx_initialize_low_level.S to match the memory region being used
        LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|
    ```
    + For STM32CubeIDE add the following section into the .ld file:
    ```
    ._threadx_heap :
      {
         . = ALIGN(8);
         __RAM_segment_used_end__ = .;
         . = . + 64K;
         . = ALIGN(8);
       } >RAM AT> RAM
    ```

       The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
       In the example above the ThreadX heap size is set to 64KBytes.
       The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
       Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
       Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

#### <b>FileX/LevelX usage hints</b>

- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.
- FileX uses data buffers passed to fx_media_open(), fx_media_read(), and fx_media_write(). It is recommended that these buffers are a multiple of the sector size.

### <b>Keywords</b>

RTOS, ThreadX, FileX, LevelX, File System, NOR, W25Q128JV, FAT32

### <b>Hardware and Software environment</b>

  - This application runs on STM32U34xx devices.
  - This application has been tested using MIKROE Flash 6 Click board embedding a Winbond W25Q128JV NOR flash device and plugged on
    NUCLEO-U345RC-Q (MB1841) board and can be easily tailored to any other supported device and development board.

  - This application uses USART1 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application