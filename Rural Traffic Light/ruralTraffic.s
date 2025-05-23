; Assembly program using Assembly to toggle the green LED (LD2 -> PA5)
; on STM32F446RE Nucleo64 board at 1 Hz
;
    EXPORT Reset_Handler
		
    AREA vectors, CODE, READONLY
    DCD Reset_Handler ; Reset Handler

RCC_AHB1ENR equ 0x40023830
GPIOC_MODER equ 0x40020800    
GPIOC_ODR equ 0x40020814
GPIOC_IDR equ 0x40020810
GPIOC_PUPDR equ 0x4002080C


    AREA PROG, CODE, READONLY
Reset_Handler

		ldr r4, =RCC_AHB1ENR ; enable GPIOA clock
		mov r5, #5
		str r5, [r4]
		
		ldr r4, =GPIOC_MODER ; set pc 0 - 5 as output
		ldr r4, [r4]		; and 6 & 7 as input
		ldr r5, =0xffff0000 
		and r4, r4, r5
		ldr r5, =0x00000555
		orr r4, r4, r5
		ldr r5, =GPIOC_MODER
		str r4, [r5]
		
		ldr r4, =GPIOC_PUPDR
		ldr r5, =0x5000
		str r5, [r4]
		
		ldr r4, =GPIOC_ODR; default NSG EWR
		ldr r5, =0x0C
		str r5, [r4]

While1	mov r0, #3000 ; delay 3 sec
		bl delay
		
EWL		ldr r2, =GPIOC_IDR
		ldr r2, [r2]
		and r2, r2, #0x80
		cmp r2, #0
		bne EWL
		
		ldr r5, =0x0A; NSY EWR
		str r5, [r4]
		mov r0, #1000 ; delay 1 sec
		bl delay
		
		ldr r5, =0x21; NSY EWR
		str r5, [r4]
		mov r0, #3000 ; delay 3 sec
		bl delay
		
NSL		ldr r2, =GPIOC_IDR
		ldr r2, [r2]
		and r2, r2, #0x40
		cmp r2, #0
		bne NSL
		
		ldr r5, =0x11; NSY EWR
		str r5, [r4]
		mov r0, #1000 ; delay 1 sec
		bl delay
		
		ldr r5, =0x0C; NSY EWR
		str r5, [r4]
		
		b While1
		

; delay milliseconds in R0
delay 	ldr r1, =5325
DL1 	subs r1, r1, #1
		bne DL1
		subs r0, r0, #1
		bne delay
		bx lr
		end
