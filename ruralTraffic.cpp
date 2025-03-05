/*
* urban traffic light
* pc0- NS red
* pc1- NS Yell
* pc2- NS Green
* pc3- EW Red 
* pc4- EW Yell 
* pc5- EW Green 
* pc6- NS Sensor
* pc7- EW sensor
*/


#include "stm32f4xx.h"


void delayMs(int n);
int main(void) {
	
	RCC -> AHB1ENR |= 5; // setup clock pA & pc
	
	GPIOC -> MODER &= 0xffff0000;
	GPIOC -> MODER |= 0x00000555; // set pc 0 - 6 enabled 6 & 7 are input
	GPIOC -> ODR &= 0;// clear pins
	GPIOC -> PUPDR = 0x5000;//put pull up resistors on out switches
	GPIOC -> ODR = 0B001100;// defaulting NSG EWR
	
	//Start cycle
	while(1){
		delayMs(3000); // hold the initial 3 seconds
		while((GPIOC -> IDR & 0x80) != 0 ){} //read EW sensor
		GPIOC -> ODR = 0B001010; // NSY EWR
		delayMs(1000);
		
		GPIOC -> ODR = 0B100001;//NSR EWG
		delayMs(3000);
		while((GPIOC -> IDR & 0x40) != 0 ){}//read NS sensor
		
		GPIOC -> ODR = 0B010001;//NSR EWY
		delayMs(1000);
		
		GPIOC -> ODR = 0B001100;// defaulting NSG EWR
	}
	
}
void delayMs(int n) {
int i;
for (; n > 0; n--)
	for (i = 0; i < 3195; i++) ; // Increase the loop count to slow down the delay
}


