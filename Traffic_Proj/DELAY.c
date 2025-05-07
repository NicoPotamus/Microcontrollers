#include "DELAY.h"

void delayMs(int n) {
    int i;
    SysTick->LOAD = 16000;
    SysTick->VAL = 0;
    SysTick->CTRL = 0x5;

    for (i = 0; i < n; i++)
    {
        while ((SysTick->CTRL & 0x10000) == 0)
        {
        }
    }
    SysTick->CTRL = 0;
}