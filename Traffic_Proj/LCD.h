/* p3_1.c: Initialize and display "hello" on the LCD using 8-bit data mode.
 *
 * Initialize LCD controller and flash hello on the LCD.
 * This program does not poll the status of the LCD.
 * It uses delay to wait out the time LCD controller is busy.
 * For simplicity, all delay below 1 ms uses 1 ms. You may
 * want to adjust the amount of delay for your LCD controller
 * to enhance the performance.
 *
 * The LCD controller is connected to the Nucleo-F446RE
 * board as follows:
 *
 * PC0-PC7 for LCD D0-D7, respectively.
 * PA0 for LCD R/S
 * PB6 for LCD R/W ----- N/A
 * PA1 for LCD EN
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#include "stm32f4xx.h"
#include "DELAY.h"

#define RS 0x01    /* PB5 mask for reg select */
#define RW 0x40     /* PB6 mask for read/write */
#define EN 0x02     /* PB7 mask for enable */

void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void PORTS_init(void);

int rural_lcd(void) {

    while(1) {
				/* clear LCD display */
        LCD_command(1);
        /* Write "RURAL" on LCD */
        LCD_data('R');
        LCD_data('U');
        LCD_data('R');
        LCD_data('A');
        LCD_data('L');
        delayMs(1000);

        
        delayMs(500);
    }
}
int urban_lcd(void) {
    /* initialize LCD controller */
    LCD_init();

    while(1) {
			/* clear LCD display */
        LCD_command(1);
        /* Write "URBAN" on LCD */
        LCD_data('U');
        LCD_data('R');
        LCD_data('B');
        LCD_data('A');
        LCD_data('N');
        delayMs(1000);

        
    }
}

/* initialize port pins then initialize LCD controller */
void LCD_init(void) {
    PORTS_init();

    delayMs(30);            /* initialization sequence */
    LCD_command(0x30);
    delayMs(10);
    LCD_command(0x30);
    delayMs(1);
    LCD_command(0x30);

    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

void PORTS_init(void) {
    RCC->AHB1ENR |=  0x05;          /* enable GPIOA/C clock */

    /* PA0 for LCD R/S */
    /* N/A for LCD R/W */
    /* PA1 for LCD EN */
    GPIOA->MODER &= ~0x0000000F;    /* clear pin mode */
    GPIOA->MODER |=  0x00000005;    /* set pin output mode */
    GPIOA->BSRR   =  0x00000002;       /* turn off EN and R/W */

    /* PC0-PC7 for LCD D0-D7, respectively. */
    GPIOC->MODER &= ~0x0000FFFF;    /* clear pin mode */
    GPIOC->MODER |=  0x00005555;    /* set pin output mode */
}

void LCD_command(unsigned char command) {
    GPIOA->BSRR = (RS | 0) << 16;  /* RS = 0, R/W = 0 */
    GPIOC->ODR = command;           /* put command on data bus */
    GPIOA->BSRR = EN;               /* pulse E high */
    delayMs(0);
    GPIOA->BSRR = EN << 16;         /* clear E */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(char data) {
    GPIOA->BSRR = RS;               /* RS = 1 */
    //GPIOA->BSRR = RW << 16;         /* R/W = 0 */
    GPIOA->ODR = data;              /* put data on data bus */
    GPIOA->BSRR = EN;               /* pulse E high */
    delayMs(0);
    GPIOA->BSRR = EN << 16;         /* clear E */

    delayMs(1);
}

