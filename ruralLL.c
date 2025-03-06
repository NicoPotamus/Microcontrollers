/*
* rural traffic light
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

#define SWITCHES GPIOC -> IDR
#define LIGHTS GPIOC -> ODR

int currState = 0;
int input = 0;

void delayMs(int n);
void setup();

// Linked data structure: defines each FSM row in order
struct State {
int outputs;
int delay;
int nextState[4];
};

// Defines each state, row by row, 4 states total
typedef struct State TrafficLight;
#define S0 0
#define S1 1
#define S2 2
#define S3 3

// FSM based on State Transition Graph and Table
TrafficLight FSM[4] = {
  {0x0C,5000,{S0,S0,S1,S1}},         // state 0: NS green EW red 5 sec
  {0x0A,1000,{S2,S2,S2,S2}},      	// state 1: NS yellow EW red 1 sec
  {0x21,5000,{S2,S3,S2,S3}},         // state 2: NS red EW green 5 sec
  {0x11,1000,{S0,S0,S0,S0}},      	// state 3: NS red EW yellow 1 sec
};

// FSM Engine - Main routine
int main() {
	setup();
	while(1){
		LIGHTS = (FSM[currState].outputs);
		delayMs(FSM[currState].delay);
		input = (SWITCHES & 0b11000000) >> 6; // pins 6 & 7
		currState = FSM[currState].nextState[input];
	}
}

void setup() {
  RCC -> AHB1ENR |= 5; // setup clock pA & pc
	
	GPIOC -> MODER &= 0xffff0000;
	GPIOC -> MODER |= 0x00000555; // set pc 0 - 6 enabled 6 & 7 are input
	GPIOC -> ODR &= 0;// clear pins
	GPIOC -> PUPDR = 0x5000;//put pull up resistors on out switches
	GPIOC -> ODR = 0B001100;// defaulting NSG EWR
}

//countdown timer function
void delayMs(int n) {
	int i;
	SysTick -> LOAD = 16000;
	SysTick -> VAL = 0;
	SysTick -> CTRL = 0x5;
	
	for (i = 0; i < n; i++){
		while((SysTick -> CTRL & 0x10000) == 0){}
	}
	SysTick -> CTRL = 0;

}
