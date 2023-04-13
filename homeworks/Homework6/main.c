#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\CortexM.h"
#include "..\inc\Clock.h"
//#include "..\inc\Init_Ports.h"

void Timer_Init(void)
{

    TA2CTL &= ~0x0030; //stop timer
    TA2CTL |= 0x0200; //SMCLK chosen
    TA2CTL |= 0x80; //divider 4
    TA2CCTL3 = 0x00E0; //compare block 3 in use as reset/set
    TA2CCR0 = 59999; //60k counts; max count -1 for pwm usage

    //TA2CCR3 = 5999; //duty cycle 10%
    //TA2CTL |= 0x0010; //mode up

}

void Port_Init(void)
{

    P6DIR |= 0b01000000; //P6.6 input
    P6SEL1 &= ~0b01000000;
    P6SEL0 |= 0b01000000; //P6.6 set sel bits for pwm output

}

/**
 * main.c
 */
void main(void)
{
    bool switchDir = false;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;        // stop watchdog timer
    Clock_Init48MHz();
    Port_Init();
    Timer_Init();

    TA2CCR3 = 2999; //1ms
    TA2R = 0;
    TA2CTL |= 0x0010; //UP Mode set

    while (1)
    {
        if( TA2R < 100 ) //dollar store interrupt, don't judge
        {
            if ( switchDir == true ) TA2CCR3 -= 15; //increment the servo target
            else if( TA2CCR3 < 5999 ) TA2CCR3 += 15;
            else switchDir = true;  //once it hits 90, heads back
        }
    }
}
