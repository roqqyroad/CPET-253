//Determine which header files to include
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include"..\inc\Clock.h"
#include"..\inc\CortexM.h"
#include"..\inc\Init_Ports.h"
#include"..\inc\Motor.h"

//Use lab 2 as a reference for syntax

void TimerInit(void)
{
//**********First initialize TimerA0 for PWM
//Since the motors are connected to P2.6 and P2.7, use TimerA0, compare blocks 3 & 4****
//stop the timer
    TA0CTL &= ~0x0030;
//choose SMCLK for the clock source
    TA0CTL |= 0x0200;
    TA0CTL &= ~0x0100;
//choose clock divider of 2
    TA0CTL |= 0x0040; //0b0000 0010 0100 0000
//Outmode 7: reset/set
    //TA0CCTL1 |= 0x00E0;
    TA0CCTL3 = 0x00E0;
    TA0CCTL4 = 0x00E0;

    TA0CCR0 = 59999;


//***********Now initialize TimerAx for the delay function************//

//stop the timer
    TA1CTL &= ~0x0030;
//choose SMCLK for the clock source
    TA1CTL |= 0x0200; TA1CTL &= ~0x0100; //bit 9-8 10 to choose SMCLK
//choose clock divider of 2
    TA1CTL |= 0x0040; //0b0000 0010 0100 0000
    //bit 7-6 01 to choose clock divider of 2
//configure compare register for 10ms delay
    TA1CCR0 = 59999; //60k counts with a clock divider of 2 is 10ms
    //dont need the 15k here since we dont care about 25%

}

void DelayTenMilliseconds(void)
{
//***********Start the delay and wait************//
    //stop the timer
    TA1CTL &= ~0x0030;
    //clear the timer
    TA1R = 0;
    //start timer in continuous mode
    TA1CTL |= 0x0020;

    while (!(TA1CCTL0 & 0x0001)){} //complete while loop, wait for the CCIFG to assert

    //clear the CCIFG
    TA1CCTL0 &= ~0x0001;
    //stop the timer
    TA1CTL &= ~0x0030;
}
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    Clock_Init48MHz(); // makes bus clock 48 MHz
    TimerInit();

    //call port initialization functions
    Port2_Init();
    Port3_Init();
    Port5_Init();

    //declare enumerated states, declare starting state, declare previous state, declare state timer
    enum motor_states
    {
        FORWARD, TURN
    } state, prevState;
    state = FORWARD;
    prevState = !FORWARD;
    uint16_t stateTimer; //used to stay in a state
    bool isNewState; //declare boolean to know if state has switched use lecture 3 for state machine reference

    while (1)
    {
        isNewState = (state != prevState);
        prevState = state;

        switch (state)
        {

        case FORWARD:
            if (isNewState)
            {
                stateTimer = 0;
            }

            Motor_Forward(14750, 15055);
            stateTimer++;

            if (stateTimer == 100)
            {
                state = TURN;

            }
            break;

        case TURN:
            if (isNewState)
            {
                stateTimer = 0;
            }
            Motor_Right(15050, 0);
            stateTimer++;

            if (stateTimer == 90)
            {
                state = FORWARD;

            }
            break;

//create default statement for switch
        }
        //end switch
        DelayTenMilliseconds(); //Delay 10ms to pace state machine
        P2OUT ^= 0b00000100; //toggle blue on LED2 //Toggle LED to demonstrate 10ms delay
    } //end while(1)
} //end main()
