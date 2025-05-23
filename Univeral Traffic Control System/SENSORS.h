/*
	pc9 NS STRIAGHT / RIGHT
	pc8 EW STRAIGHT / RIGHT
	pc11 NS LEFT TURN
	pc10 EW LEFT TURN
	
	pc13- MODE SWITCH URBAN / RURAL
	pc12- Crosswalk
*/
#include "stm32f4xx.h"

#define INPUT GPIOC->IDR
void sensors_init(void){
	__disable_irq();
	RCC->AHB1ENR |= 0x04; //C clock
	RCC->APB2ENR |= 0x4000;

	GPIOC->MODER &= ~0x0FFF0000; //pc 8 - 13 for input
	//PU resistor on switches
	GPIOC->PUPDR &= ~0x0FFF0000;
	GPIOC->PUPDR |= 0x05550000;
	
	//Interrupt on crosswalk and mode
	SYSCFG->EXTICR[3] &= ~0x00FF;
	SYSCFG->EXTICR[3] |= 0x0022; //select p12 and p13 to be listened by sysfg clock
	EXTI->IMR |= 0x3000; //unmask EXTI13 
	EXTI->FTSR |= 0x3000;         /* select falling edge trigger */
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	__enable_irq(); /* global enable IRQs */

}

int read_straights(void){
	int input = INPUT & 0x300;
	return (input >> 8);
}
	
int read_arrows(void){
	int input = INPUT & 0xC00;
	return (input >> 10);
}


	