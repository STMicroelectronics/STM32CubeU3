;********************************************************************************
;* File Name          : startup_stm32u385xx.s
;* Author             : MCD Application Team
;* Description        : STM32U375xx Ultra Low Power Devices vector
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == _iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR
;*                        address.
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M33 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*******************************************************************************
;* @attention
;*
;* Copyright (c) 2023 STMicroelectronics.
;* All rights reserved.
;*
;* This software is licensed under terms that can be found in the LICENSE file
;* in the root directory of this software component.
;* If no LICENSE file comes with this software, it is provided AS-IS.
;*
;*******************************************************************************
;* <<< Use Configuration Wizard in Context Menu >>>
;
; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size		EQU     0x600

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size      EQU     0x200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                     ; Top of Stack
                DCD     Reset_Handler                    ; Reset Handler
                DCD     NMI_Handler                      ; NMI Handler
                DCD     HardFault_Handler                ; Hard Fault Handler
                DCD     MemManage_Handler                ; MPU Fault Handler
                DCD     BusFault_Handler                 ; Bus Fault Handler
                DCD     UsageFault_Handler               ; Usage Fault Handler
                DCD     SecureFault_Handler              ; Secure Fault Handler
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     SVC_Handler                      ; SVCall Handler
                DCD     DebugMon_Handler                 ; Debug Monitor Handler
                DCD     0                                ; Reserved
                DCD     PendSV_Handler                   ; PendSV Handler
                DCD     SysTick_Handler                  ; SysTick Handler
                ; External Interrupts
                DCD     WWDG_IRQHandler                  ; Window WatchDog
                DCD     PVD_PVM_IRQHandler               ; PVD/PVM through EXTI Line 16 detection Interrupt
                DCD     RTC_IRQHandler                   ; RTC non-secure interrupt
                DCD     RTC_S_IRQHandler                 ; RTC secure interrupt
                DCD     TAMP_IRQHandler                  ; Tamper interrupt
                DCD     RAMCFG_IRQHandler                ; RAMCFG global interrupt
                DCD     FLASH_IRQHandler                 ; FLASH non-secure global interrupt
                DCD     FLASH_S_IRQHandler               ; FLASH secure global interrupt
                DCD     GTZC_IRQHandler                  ; Global TrustZone Controller interrupt
                DCD     RCC_IRQHandler                   ; RCC non-secure global interrupt
                DCD     RCC_S_IRQHandler                 ; RCC secure global interrupt
                DCD     EXTI0_IRQHandler                 ; EXTI Line 0 interrupt
                DCD     EXTI1_IRQHandler                 ; EXTI Line 1 interrupt
                DCD     EXTI2_IRQHandler                 ; EXTI Line 2 interrupt
                DCD     EXTI3_IRQHandler                 ; EXTI Line 3 interrupt
                DCD     EXTI4_IRQHandler                 ; EXTI Line 4 interrupt
                DCD     EXTI5_IRQHandler                 ; EXTI Line 5 interrupt
                DCD     EXTI6_IRQHandler                 ; EXTI Line 6 interrupt
                DCD     EXTI7_IRQHandler                 ; EXTI Line 7 interrupt
                DCD     EXTI8_IRQHandler                 ; EXTI Line 8 interrupt
                DCD     EXTI9_IRQHandler                 ; EXTI Line 9 interrupt
                DCD     EXTI10_IRQHandler                ; EXTI Line 10 interrupt
                DCD     EXTI11_IRQHandler                ; EXTI Line 11 interrupt
                DCD     EXTI12_IRQHandler                ; EXTI Line 12 interrupt
                DCD     EXTI13_IRQHandler                ; EXTI Line 13 interrupt
                DCD     EXTI14_IRQHandler                ; EXTI Line 14 interrupt
                DCD     EXTI15_IRQHandler                ; EXTI Line 15 interrupt
                DCD     IWDG_IRQHandler                  ; IWDG global interrupt
                DCD     SAES_IRQHandler                  ; Secure AES global interrupt
                DCD     GPDMA1_Channel0_IRQHandler       ; GPDMA1 Channel 0 global interrupt
                DCD     GPDMA1_Channel1_IRQHandler       ; GPDMA1 Channel 1 global interrupt
                DCD     GPDMA1_Channel2_IRQHandler       ; GPDMA1 Channel 2 global interrupt
                DCD     GPDMA1_Channel3_IRQHandler       ; GPDMA1 Channel 3 global interrupt
                DCD     GPDMA1_Channel4_IRQHandler       ; GPDMA1 Channel 4 global interrupt
                DCD     GPDMA1_Channel5_IRQHandler       ; GPDMA1 Channel 5 global interrupt
                DCD     GPDMA1_Channel6_IRQHandler       ; GPDMA1 Channel 6 global interrupt
                DCD     GPDMA1_Channel7_IRQHandler       ; GPDMA1 Channel 7 global interrupt
                DCD     ADC1_IRQHandler                  ; ADC1 global interrupt
                DCD     DAC1_IRQHandler                  ; DAC1 global interrupt
                DCD     FDCAN1_IT0_IRQHandler            ; FDCAN1 interrupt 0
                DCD     FDCAN1_IT1_IRQHandler            ; FDCAN1 interrupt 1
                DCD     TIM1_BRK_TERR_IERR_IRQHandler    ; TIM1 Break, Transition error and Index error interrupt
                DCD     TIM1_UP_IRQHandler               ; TIM1 Update interrupt
                DCD     TIM1_TRG_COM_DIR_IDX_IRQHandler  ; TIM1 Trigger, Commutation, Direction change and Index interrupt
                DCD     TIM1_CC_IRQHandler               ; TIM1 Capture Compare interrupt
                DCD     TIM2_IRQHandler                  ; TIM2 global interrupt
                DCD     TIM3_IRQHandler                  ; TIM3 global interrupt
                DCD     TIM4_IRQHandler                  ; TIM4 global interrupt
                DCD     0                                ; Reserved
                DCD     TIM6_IRQHandler                  ; TIM6 global interrupt
                DCD     TIM7_IRQHandler                  ; TIM7 global interrupt
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     I3C1_EV_IRQHandler               ; I3C1 event interrupt
                DCD     I3C1_ER_IRQHandler               ; I3C1 error interrupt
                DCD     I2C1_EV_IRQHandler               ; I2C1 Event interrupt
                DCD     I2C1_ER_IRQHandler               ; I2C1 Error interrupt
                DCD     I2C2_EV_IRQHandler               ; I2C2 Event interrupt
                DCD     I2C2_ER_IRQHandler               ; I2C2 Error interrupt
                DCD     SPI1_IRQHandler                  ; SPI1 global interrupt
                DCD     SPI2_IRQHandler                  ; SPI2 global interrupt
                DCD     USART1_IRQHandler                ; USART1 global interrupt
                DCD     0                                ; Reserved
                DCD     USART3_IRQHandler                ; USART3 global interrupt
                DCD     UART4_IRQHandler                 ; UART4 global interrupt
                DCD     UART5_IRQHandler                 ; UART5 global interrupt
                DCD     LPUART1_IRQHandler               ; LPUART1 global interrupt
                DCD     LPTIM1_IRQHandler                ; LPTIM1 global interrupt
                DCD     LPTIM2_IRQHandler                ; LPTIM2 global interrupt
                DCD     TIM15_IRQHandler                 ; TIM15 global interrupt
                DCD     TIM16_IRQHandler                 ; TIM16 global interrupt
                DCD     TIM17_IRQHandler                 ; TIM17 global interrupt
                DCD     COMP_IRQHandler                  ; COMP1 and COMP2 through EXTI Lines interrupt
                DCD     USB_FS_IRQHandler                ; USB FS global interrupt
                DCD     CRS_IRQHandler                   ; CRS global interrupt
                DCD     0                                ; Reserved
                DCD     OCTOSPI1_IRQHandler              ; OctoSPI1 global interrupt
                DCD     0                                ; Reserved
                DCD     SDMMC1_IRQHandler                ; SDMMC1 global interrupt
                DCD     0                                ; Reserved
                DCD     GPDMA1_Channel8_IRQHandler       ; GPDMA1 Channel 8 global interrupt
                DCD     GPDMA1_Channel9_IRQHandler       ; GPDMA1 Channel 9 global interrupt
                DCD     GPDMA1_Channel10_IRQHandler      ; GPDMA1 Channel 10 global interrupt
                DCD     GPDMA1_Channel11_IRQHandler      ; GPDMA1 Channel 11 global interrupt
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     I2C3_EV_IRQHandler               ; I2C3 event interrupt
                DCD     I2C3_ER_IRQHandler               ; I2C3 error interrupt
                DCD     SAI1_IRQHandler                  ; Serial Audio Interface 1 global interrupt
                DCD     0                                ; Reserved
                DCD     TSC_IRQHandler                   ; Touch Sense Controller global interrupt
                DCD     AES_IRQHandler                   ; AES global interrupt
                DCD     RNG_IRQHandler                   ; RNG global interrupt
                DCD     FPU_IRQHandler                   ; FPU global interrupt
                DCD     HASH_IRQHandler                  ; HASH global interrupt
                DCD     PKA_IRQHandler                   ; PKA global interrupt
                DCD     LPTIM3_IRQHandler                ; LPTIM3 global interrupt
                DCD     SPI3_IRQHandler                  ; SPI3 global interrupt
                DCD     I3C2_EV_IRQHandler               ; I3C2 Event interrupt
                DCD     I3C2_ER_IRQHandler               ; I3C2 Error interrupt
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     ICACHE_IRQHandler                ; Instruction cache global interrupt
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     LPTIM4_IRQHandler                ; LPTIM4 global interrupt
                DCD     0                                ; Reserved
                DCD     ADF1_IRQHandler                  ; ADF interrupt
                DCD     ADC2_IRQHandler                  ; ADC2 global interrupt
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     0                                ; Reserved
                DCD     PWR_IRQHandler                   ; PWR non-secure interrupt
                DCD     PWR_S_IRQHandler                 ; PWR secure interrupt

__Vectors_End

__Vectors_Size  EQU    __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler\
                PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SecureFault_Handler\
                PROC
                EXPORT  SecureFault_Handler       [WEAK]
                B       .
                ENDP
SVC_Handler\
                PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT WWDG_IRQHandler                 [WEAK]
                EXPORT PVD_PVM_IRQHandler              [WEAK]
                EXPORT RTC_IRQHandler                  [WEAK]
                EXPORT RTC_S_IRQHandler                [WEAK]
                EXPORT TAMP_IRQHandler                 [WEAK]
                EXPORT RAMCFG_IRQHandler               [WEAK]
                EXPORT FLASH_IRQHandler                [WEAK]
                EXPORT FLASH_S_IRQHandler              [WEAK]
                EXPORT GTZC_IRQHandler                 [WEAK]
                EXPORT RCC_IRQHandler                  [WEAK]
                EXPORT RCC_S_IRQHandler                [WEAK]
                EXPORT EXTI0_IRQHandler                [WEAK]
                EXPORT EXTI1_IRQHandler                [WEAK]
                EXPORT EXTI2_IRQHandler                [WEAK]
                EXPORT EXTI3_IRQHandler                [WEAK]
                EXPORT EXTI4_IRQHandler                [WEAK]
                EXPORT EXTI5_IRQHandler                [WEAK]
                EXPORT EXTI6_IRQHandler                [WEAK]
                EXPORT EXTI7_IRQHandler                [WEAK]
                EXPORT EXTI8_IRQHandler                [WEAK]
                EXPORT EXTI9_IRQHandler                [WEAK]
                EXPORT EXTI10_IRQHandler               [WEAK]
                EXPORT EXTI11_IRQHandler               [WEAK]
                EXPORT EXTI12_IRQHandler               [WEAK]
                EXPORT EXTI13_IRQHandler               [WEAK]
                EXPORT EXTI14_IRQHandler               [WEAK]
                EXPORT EXTI15_IRQHandler               [WEAK]
                EXPORT IWDG_IRQHandler                 [WEAK]
                EXPORT SAES_IRQHandler                 [WEAK]
                EXPORT GPDMA1_Channel0_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel1_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel2_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel3_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel4_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel5_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel6_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel7_IRQHandler      [WEAK]
                EXPORT ADC1_IRQHandler                 [WEAK]
                EXPORT DAC1_IRQHandler                 [WEAK]
                EXPORT FDCAN1_IT0_IRQHandler           [WEAK]
                EXPORT FDCAN1_IT1_IRQHandler           [WEAK]
                EXPORT TIM1_BRK_TERR_IERR_IRQHandler   [WEAK]
                EXPORT TIM1_UP_IRQHandler              [WEAK]
                EXPORT TIM1_TRG_COM_DIR_IDX_IRQHandler [WEAK]
                EXPORT TIM1_CC_IRQHandler              [WEAK]
                EXPORT TIM2_IRQHandler                 [WEAK]
                EXPORT TIM3_IRQHandler                 [WEAK]
                EXPORT TIM4_IRQHandler                 [WEAK]
                EXPORT TIM6_IRQHandler                 [WEAK]
                EXPORT TIM7_IRQHandler                 [WEAK]
                EXPORT I3C1_EV_IRQHandler              [WEAK]
                EXPORT I3C1_ER_IRQHandler              [WEAK]
                EXPORT I2C1_EV_IRQHandler              [WEAK]
                EXPORT I2C1_ER_IRQHandler              [WEAK]
                EXPORT I2C2_EV_IRQHandler              [WEAK]
                EXPORT I2C2_ER_IRQHandler              [WEAK]
                EXPORT SPI1_IRQHandler                 [WEAK]
                EXPORT SPI2_IRQHandler                 [WEAK]
                EXPORT USART1_IRQHandler               [WEAK]
                EXPORT USART3_IRQHandler               [WEAK]
                EXPORT UART4_IRQHandler                [WEAK]
                EXPORT UART5_IRQHandler                [WEAK]
                EXPORT LPUART1_IRQHandler              [WEAK]
                EXPORT LPTIM1_IRQHandler               [WEAK]
                EXPORT LPTIM2_IRQHandler               [WEAK]
                EXPORT TIM15_IRQHandler                [WEAK]
                EXPORT TIM16_IRQHandler                [WEAK]
                EXPORT TIM17_IRQHandler                [WEAK]
                EXPORT COMP_IRQHandler                 [WEAK]
                EXPORT USB_FS_IRQHandler               [WEAK]
                EXPORT CRS_IRQHandler                  [WEAK]
                EXPORT OCTOSPI1_IRQHandler             [WEAK]
                EXPORT SDMMC1_IRQHandler               [WEAK]
                EXPORT GPDMA1_Channel8_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel9_IRQHandler      [WEAK]
                EXPORT GPDMA1_Channel10_IRQHandler     [WEAK]
                EXPORT GPDMA1_Channel11_IRQHandler     [WEAK]
                EXPORT I2C3_EV_IRQHandler              [WEAK]
                EXPORT I2C3_ER_IRQHandler              [WEAK]
                EXPORT SAI1_IRQHandler                 [WEAK]
                EXPORT TSC_IRQHandler                  [WEAK]
                EXPORT AES_IRQHandler                  [WEAK]
                EXPORT RNG_IRQHandler                  [WEAK]
                EXPORT FPU_IRQHandler                  [WEAK]
                EXPORT HASH_IRQHandler                 [WEAK]
                EXPORT PKA_IRQHandler                  [WEAK]
                EXPORT LPTIM3_IRQHandler               [WEAK]
                EXPORT SPI3_IRQHandler                 [WEAK]
                EXPORT I3C2_EV_IRQHandler              [WEAK]
                EXPORT I3C2_ER_IRQHandler              [WEAK]
                EXPORT ICACHE_IRQHandler               [WEAK]
                EXPORT LPTIM4_IRQHandler               [WEAK]
                EXPORT ADF1_IRQHandler                 [WEAK]
                EXPORT ADC2_IRQHandler                 [WEAK]
                EXPORT PWR_IRQHandler                  [WEAK]
                EXPORT PWR_S_IRQHandler                [WEAK]

WWDG_IRQHandler
PVD_PVM_IRQHandler
RTC_IRQHandler
RTC_S_IRQHandler
TAMP_IRQHandler
RAMCFG_IRQHandler
FLASH_IRQHandler
FLASH_S_IRQHandler
GTZC_IRQHandler
RCC_IRQHandler
RCC_S_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
EXTI5_IRQHandler
EXTI6_IRQHandler
EXTI7_IRQHandler
EXTI8_IRQHandler
EXTI9_IRQHandler
EXTI10_IRQHandler
EXTI11_IRQHandler
EXTI12_IRQHandler
EXTI13_IRQHandler
EXTI14_IRQHandler
EXTI15_IRQHandler
IWDG_IRQHandler
SAES_IRQHandler
GPDMA1_Channel0_IRQHandler
GPDMA1_Channel1_IRQHandler
GPDMA1_Channel2_IRQHandler
GPDMA1_Channel3_IRQHandler
GPDMA1_Channel4_IRQHandler
GPDMA1_Channel5_IRQHandler
GPDMA1_Channel6_IRQHandler
GPDMA1_Channel7_IRQHandler
ADC1_IRQHandler
DAC1_IRQHandler
FDCAN1_IT0_IRQHandler
FDCAN1_IT1_IRQHandler
TIM1_BRK_TERR_IERR_IRQHandler
TIM1_UP_IRQHandler
TIM1_TRG_COM_DIR_IDX_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
TIM6_IRQHandler
TIM7_IRQHandler
I3C1_EV_IRQHandler
I3C1_ER_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
LPUART1_IRQHandler
LPTIM1_IRQHandler
LPTIM2_IRQHandler
TIM15_IRQHandler
TIM16_IRQHandler
TIM17_IRQHandler
COMP_IRQHandler
USB_FS_IRQHandler
CRS_IRQHandler
OCTOSPI1_IRQHandler
SDMMC1_IRQHandler
GPDMA1_Channel8_IRQHandler
GPDMA1_Channel9_IRQHandler
GPDMA1_Channel10_IRQHandler
GPDMA1_Channel11_IRQHandler
I2C3_EV_IRQHandler
I2C3_ER_IRQHandler
SAI1_IRQHandler
TSC_IRQHandler
AES_IRQHandler
RNG_IRQHandler
FPU_IRQHandler
HASH_IRQHandler
PKA_IRQHandler
LPTIM3_IRQHandler
SPI3_IRQHandler
I3C2_EV_IRQHandler
I3C2_ER_IRQHandler
ICACHE_IRQHandler
LPTIM4_IRQHandler
ADF1_IRQHandler
ADC2_IRQHandler
PWR_IRQHandler
PWR_S_IRQHandler


                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF

                END
