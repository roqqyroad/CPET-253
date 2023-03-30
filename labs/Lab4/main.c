#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\Init_Ports.h"
#include "..\inc\Motor.h"
#include "..\inc\BumpInt.h"

extern bool wasInterrupt;
extern bool wasRight;

void TimerInit(void)
{
    //**********First initialize TimerA0 for PWM
    //Since the motors are connected to P2.6 and P2.7, use TimerA0, compare blocks 3 & 4****
    TA0CCR3 = 14999;
    TA0CCR4 = 14999;
    //***********Now initialize TimerAx for the delay function************//
    TA0CTL &= ~0x0030; //stop the timer
    TA0CTL |= 0x0200;
    TA0CTL &= ~0x0100; //choose SMCLK for the clock source
    TA0CTL |= 0x0040; //choose clock divider of 2 - ID: 01
    //TA0CTL &= 0x0080;
    //set the period timer
    TA0CCR0 = 59999;
    //outmode 7
    TA0CCTL3 |= 0x00E0; //Outmode 7: reset/set
    TA0CCTL4 |= 0x00E0; //Outmde 7: reset/set
    //TimerA2
    TA2CTL &= ~0x0030;    //stop the timer
    TA2CTL |= 0x0200;
    TA2CTL &= ~0x0100;    //choose SMCLK for the clock source
    TA2CTL |= 0x0040; //choose clock divider of 2 - ID: 01
    //TA2CTL &= 0x0000;
    TA2CCR0 = 59999; //configure compare register for 10ms delay
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

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    DisableInterrupts();
    Clock_Init48MHz(); // makes bus clock 48 MHz
    TimerInit();
    Port2_Init();
    Port3_Init();
    Port5_Init(); //port initialization functions
    BumpInt_Init(); //Initialize interrupts
    EnableInterrupts();

    enum motor_states
    {
        FORWARD, BACKWARD, TURNLEFT, TURNRIGHT
    } state, prevState; //declare enumerated states, declare starting state, declare previous state, declare state timer
    state = FORWARD;
    prevState = !FORWARD;
    bool isNewState; //declare boolean to know if state has switched use lecture 3 for state machine reference
    uint16_t stateTimer;

    while (1)
    {
        isNewState = (state != prevState);
        prevState = state; //save state for next time
        uint16_t status = P4IV;

        switch (state)
        {
        case FORWARD:
            Motor_Forward(12750, 13055); //call driving function
            if (wasInterrupt)
            { //if interrupt go backward
                wasInterrupt = false;
                state = BACKWARD;
            }
            break;
        case BACKWARD:
            if (isNewState)
            {
                stateTimer = 0; //reset timer
                Motor_Backward(12750, 13055); //call driving function
            }
            else if (stateTimer >= 46)
            {
                if (wasRight)
                {
                    wasRight = false;
                    state = TURNLEFT;
                }
                else if (!wasRight)
                {
                    wasRight = false;
                    state = TURNRIGHT;
                }
            }
            else
            {
                stateTimer++;
            }
            break;
        case TURNRIGHT:
            if (isNewState)
            {
                stateTimer = 0; //reset timer
                Motor_Right(12750, 0); //turning
            }
            stateTimer++;
            if (stateTimer >= 91)
            {
                P4IFG &= 0x00;
                wasInterrupt = false;
                stateTimer = 0;
                state = FORWARD;
            }
            break;
        case TURNLEFT:
            if (isNewState)
            {
                stateTimer = 0; //reset timer
                Motor_Left(0, 13055); //turning
            }
            stateTimer++;
            if (stateTimer >= 91)
            {
                P4IFG &= 0x00;
                wasInterrupt = false;
                stateTimer = 0;
                state = FORWARD;
            }
            break;
            state = FORWARD; //default state of switch statement
        } //end switch
        DelayTenMilliseconds(); //Delay 10ms to pace state machine
        // P2OUT ^= 0b00000100;//Toggle LED to demonstrate 10ms delay
    } //end while(1)
}
