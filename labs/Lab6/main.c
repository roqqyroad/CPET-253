
#include "msp432.h"

#include <stdbool.h>
#include <stdint.h>

#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\Init_Ports.h"
#include "..\inc\Motor.h"
#include "..\inc\Reflectance.h"

void TimerInit(void)
{
    //**********First initialize TimerA0 for PWM
    //Since the motors are connected to P2.6 and P2.7, use TimerA0, compare blocks 3 & 4****
    TA0CCR3 = 14999;
    TA0CCR4 = 14999;

    TA0CTL &= ~0x0030; //stop the timer
    TA0CTL |= 0x0200; //choose SMCLK for the clock source
    TA0CTL &= ~0x0100; //choose SMCLK for the clock source
    TA0CTL |= 0x0040; //choose clock divider of 2 - ID: 01


    //Period of 60k
    TA0CCR0 = 59999;

    //Outmde 7: reset/set
    TA0CCTL3 |= 0x00E0;
    TA0CCTL4 |= 0x00E0;
    //END OF PWM

    //TimerA2 10ms Delay
    TA2CTL &= ~0x0030;    //stop the timer
    TA2CTL |= 0x0200;
    TA2CTL &= ~0x0100;    //choose SMCLK for the clock source
    TA2CTL |= 0x0040; //choose clock divider of 2 - ID: 01
    //TA2CTL &= 0x0000;
    TA2CCR0 = 59999; //configure compare register for 10ms delay
    //See DelayTenMilliseconds Function
}

void DelayTenMilliseconds(void)
{
    //***********Start the delay and wait************//
    TA2R = 0; //clear the timer
    TA2CTL |= 0x0020; //start timer in continuous mode
    while (!(TA2CCTL0 & 0x0001)){}; //complete while loop, wait for the CCIFG to assert
    TA2CCTL0 &= ~0x0001; //clear the CCIFG
    TA2CTL &= ~0x0030; //stop the timer
}


/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    Clock_Init48MHz(); // makes bus clock 48 MHz
    TimerInit(); //timer initialization

    //Port initialization functions
    Port2_Init();
    Port3_Init();
    Port5_Init();
    Port7_Init();
    Port9_Init();


}
