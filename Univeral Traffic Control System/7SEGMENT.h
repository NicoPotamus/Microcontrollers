/**
* pa4- a - 7
* pa5- b - 6
* pa6- c - 4
* pa7- d - 2
* pa8- e - 1
* pa9- f - 9
* pa10- g - 10
* pa11- h - 5
* 
*/
#include "stm32f4xx.h"
#include "DELAY.h"


void sevenSegment_init(void) {
	
	RCC -> AHB1ENR |= 5; // setup clock

	GPIOA -> MODER &= 0xA8000000; // Reset Port A
  GPIOA -> MODER |= 0x00555500; // Enable pins 4 to 11 on Port A
}

void countdown_cross(void) {
	
	//number code for 0-9
  int ledNums [] = {0x6F, 0x7F, 0x07, 0x7D, 0x6D, 0x66, 0x4F, 0x5B, 0x06, 0x3F, 0x80, 0x00, 0x80, 0x00, 0x80};
    
    //Start cycle
		for(int i = 0; i < 16; i++){
				//bit shift left to account for pin offeset
				GPIOA -> ODR = (ledNums[i] << 4);
				delayMs(1000);
		}
		GPIOA -> ODR = 0;
		
}