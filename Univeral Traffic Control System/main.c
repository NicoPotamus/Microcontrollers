
#include "LIGHTS.h"
#include "7SEGMENT.h"
#include "stdbool.h"
#include "SENSORS.h"
#include "LCD.h"
//delay is defined in 7Segment
//#include "DELAY.h"

struct State{
	int NS;
	int EW;
	int delay;
	int canCross;
	int arrow_precidence;
	int nextState[4];
};

typedef struct State UrbanTrafficLight;
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define S4 4
#define S5 5
#define S6 6
#define S7 7
#define S8 8
#define S9 9
#define S10 10
#define S11 11

typedef struct State TrafficLight;
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define S4 4
#define S5 5
#define S6 6
#define S7 7
#define S8 8
#define S9 9
#define S10 10
#define S11 11

void EXTI15_10_IRQHandler(void);

bool crossInterrupt= false;
bool modeInterrupt = false;

int main(void){
	
	lights_init();
	sensors_init();
	sevenSegment_init();
	LCD_init();
	
	
	TrafficLight FSM[12]= {
		{NS_green(), EW_red(), 3000, 0, 3, {S0, S1, S0, S1}}, //S0
		{NS_yellow(),EW_red(), 1000, 0, 0, {S2, S2, S2, S2}},
		{NS_red(), EW_red(), 		100, 1, 2, {S4, S8, S4, S8}},
		{NS_red(), EW_red(),	 9000, 1, 2, {S4, S8, S4, S8}}, //S3 should be unreachable
		{NS_red(), EW_green(), 3000, 0, 3, {S4, S4, S5, S5}},
		{NS_red(), EW_yellow(),1000, 0, 0, {S6, S6, S6, S6}},
		{NS_red(), EW_red(), 		100, 1, 2, {S0, S0, S10, S10}},//S6
		{NS_red(), EW_red(),	 9000, 1, 2, {S0, S0, S10, S10}},// should b unreachable
		{NS_red(), EW_arrow_green(), 3000, 0, 0, {S9, S9, S9, S9}},
		{NS_red(), EW_arrow_yellow(),1000, 0, 0, {S4, S4, S4, S4}},//S9
		{NS_arrow_green(), EW_red(), 3000, 0, 0, {S11, S11, S11, S11}},
		{NS_arrow_yellow(), EW_red(),  1000, 0, 0,  {S0, S0, S0, S0}}//S11
	};
	
	UrbanTrafficLight UFSM[12]= {
		{NS_green(), EW_red(), 3000, 0, 3, {S1, S1, S0, S1}}, //S0
		{NS_yellow(),EW_red(), 1000, 0, 0, {S2, S2, S2, S2}},
		{NS_red(), EW_red(), 		100, 1, 2, {S8, S8, S4, S8}},
		{NS_red(), EW_red(),	 9000, 1, 2, {S4, S8, S4, S8}}, //S3 should be unreachable
		{NS_red(), EW_green(), 3000, 0, 3, {S5, S4, S5, S5}},
		{NS_red(), EW_yellow(),1000, 0, 0, {S6, S6, S6, S6}},
		{NS_red(), EW_red(), 		100, 1, 2, {S10, S0, S10, S10}},//S6
		{NS_red(), EW_red(),	 9000, 1, 2, {S0, S0, S10, S10}},// should b unreachable
		{NS_red(), EW_arrow_green(), 3000, 0, 0, {S9, S9, S9, S9}},
		{NS_red(), EW_arrow_yellow(),1000, 0, 0, {S4, S4, S4, S4}},//S9
		{NS_arrow_green(), EW_red(), 3000, 0, 0, {S11, S11, S11, S11}},
		{NS_arrow_yellow(), EW_red(),  1000, 0, 0,  {S0, S0, S0, S0}}//S11
	};
	
	while(1){
		rural_lcd();
		
		int currState = 0;
		while(1){
			output_lights(FSM[currState].NS, FSM[currState].EW);
			delayMs(FSM[currState].delay);
			
			if(FSM[currState].canCross == 1){
				//check button and cross if necessary
				if(crossInterrupt == true){
					//crosswalk
					countdown_cross();
					crossInterrupt = false;
				}
				if(modeInterrupt == true){
					modeInterrupt = false;
					break;
				}
				
			}
			if(FSM[currState].arrow_precidence == 0){
				currState = FSM[currState].nextState[0];
			}
			else if(FSM[currState].arrow_precidence == 1){
				//use striaght sensors
				int sensors = read_straights();
				currState = FSM[currState].nextState[sensors]; 
			}
			else if(FSM[currState].arrow_precidence == 2){
				//use left sensors
				int sensors =read_arrows();
				currState = FSM[currState].nextState[sensors];
			}
			else{
				//and the 2 sets of sensors
				int sensors =(read_arrows() | read_straights());
				currState = FSM[currState].nextState[sensors];		}
		}
		
		//LCD OUTPUT URBAN
		urban_lcd();
		int currUrban = 0;
		while(1){
			output_lights(UFSM[currUrban].NS, UFSM[currUrban].EW);
			delayMs(UFSM[currUrban].delay);
			
			if(UFSM[currUrban].canCross == 1){
				//check button and cross if necessary
				if(crossInterrupt == true){
					//crosswalk
					countdown_cross();
					crossInterrupt = false;
				}
				if(modeInterrupt == true){
					modeInterrupt = false;
					break;
				}
				
			}
				currUrban = UFSM[currUrban].nextState[0];
			
		}
	}
	
	
}


// A function that runs when the button is pressed
void EXTI15_10_IRQHandler(void)
{
	
		int flags = (((EXTI->PR)) & ~0xFFFFCFFF);
		if (EXTI->PR & (1 << 12)){
			crossInterrupt = true;
			EXTI->PR = 0x1000; /* clear interrupt pending flag */
		}
		if(EXTI->PR & (1 << 13)){
			modeInterrupt = true;
			EXTI->PR = 0x2000; /* clear interrupt pending flag */
		}
		if (EXTI->PR & (1 << 15)) {  // Check if EXTI15 is set
        EXTI->PR = (1 << 15);  // Clear interrupt pending bit for EXTI15
    }

}
