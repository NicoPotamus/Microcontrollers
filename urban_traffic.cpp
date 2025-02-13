/*
* urban traffic light
* pc0- NS red
* pc1- NS Yell
* pc2- NS Green
* pc3- EW Red orange
* pc4- EW Yell purple
* pc5- EW Green blue
*/


#include "stm32f4xx.h"

void delayMs(int n);
int main(void) {
	
	RCC -> AHB1ENR |= 5; // setup clock
	
	
	GPIOC -> MODER &= 0xfffff000;
	GPIOC -> MODER |= 0x00000555; // set pc 0 - 6 enabled
	GPIOC -> ODR &= 0;// clear pins
	GPIOC -> ODR = 0B001100;// defaulting NSG EWR
	
	//Start cycle
	while(1){
		delayMs(3000); // hold the initial 3 seconds
		
		GPIOC -> ODR = 0B001010; // NSY EWR
		delayMs(1000);
		
		GPIOC -> ODR = 0B100001;//NSR EWG
		delayMs(3000);
		
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


