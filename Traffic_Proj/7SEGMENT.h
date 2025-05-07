/**
* pa4- a
* pa5- b
* pa6- c
* pa7- d
* pa8- e
* pa9- f
* pa10- g
* pa11- h
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
  int ledNums [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    
    //Start cycle
		for(int i = 0; i < 10; i++){
				//bit shift left to account for pin offeset
				GPIOA -> ODR = (ledNums[i] << 4);
				delayMs(1000);
		}
		GPIOA -> ODR = 0;
		
}