#include "stm32f446xx.h"

/*
    NS red - pb4
    NS yellow - pb5
    NS green - pb6
	NS Arrow yellow - pb7
    NS Arrow green - pb8
		
    EW red - pb0
    EW yellow - pb2
    EW green - pb9
    EW Arrow yellow - pb10
    EW Arrow green - pb12
*/

void lights_init(){
	RCC->AHB1ENR |= 0x02;
	GPIOB->MODER &= ~0x033FFF33;
	GPIOB->MODER |=  0x01155511; //lights to output
	
}
void output_lights(int num1,int num2){
	GPIOB->ODR = (num1 | num2);
}

int NS_red(){
	return 0x10;
}
int NS_yellow(){
	return 0x20;
}
int NS_green(){
	return 0x40;
}
int NS_arrow_yellow(){
	return 0x90;
}
int NS_arrow_green(){
	return 0x110;
}
int EW_red(){
	return 0x01;
}
int EW_yellow(){
	return 0x04;
}
int EW_green(){
	return 0x200;
}
int EW_arrow_yellow(){
	return (EW_red() | 0x400);
}
int EW_arrow_green(){
	return (EW_red() | 0x1000);
}