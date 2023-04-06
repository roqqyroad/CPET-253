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
    while (!(TA2CCTL0 & 0x0001))
    {
    }; //complete while loop, wait for the CCIFG to assert
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

    Reflectance_Init(); //reflecrance inintializaition

    //Port initialization functions
    Port2_Init();
    Port3_Init();
    Port5_Init();
    Port7_Init();
    Port9_Init();

    //create states for diagram
    enum motor_states
    {
        FORWARD, STOPPED, CORRECTLEFT, CORRECTRIGHT
    } state, prevState; //declare enumerated states, declare starting state, declare previous state, declare state timer
    state = FORWARD;
    prevState = !FORWARD;
    bool isNewState; //declare boolean to know if state has switched use lecture 3 for state machine reference
    uint16_t stateTimer;

    while (1)
    {
        // Reflectance_Read(1500); //for first signoff
        uint8_t data = Reflectance_Read(1500); //get data
        int32_t weight = Reflectance_Position(data); //get position

        isNewState = (state != prevState);
        prevState = state; //save state for next time
        switch (state)
        {
        case FORWARD:

            if (isNewState)
            {
                Motor_Forward(4000, 4100); //go forward
                data = Reflectance_Read(1500); //create more data
                weight = Reflectance_Position(data); //and get position
            }
            if (weight > 0)
            { //if the position isnt centered and is a bit too far left, go right
                state = CORRECTRIGHT;
            }
            if (weight < 0)
            { //if the position isnt centered and is too far right, go left
                state = CORRECTLEFT;
            }
            else
            { //otherwise continue as is
                state = FORWARD;
            }

            break;

            //Used in testing the lab
        case STOPPED:
            if (isNewState)
            {
                Motor_Stop();
            }
            break;



        case CORRECTRIGHT:
            if (isNewState){
                //Motor_Stop();//for testing
            Motor_Forward(4000, 2000);
            data = Reflectance_Read(1500);
            weight = Reflectance_Position(data);
        }
        if (weight < 0)
        {//went too far right
            state = CORRECTLEFT;//correct it by going left
        }
        if (weight == 0)
        {//centered
            state = FORWARD;//so continue forward
        }
        else
        {//not fully corrected yet
            state = CORRECTRIGHT;//so continue trying to correct it
        }

        break;



        case CORRECTLEFT:
        if (isNewState)
        {
            //Motor_Stop(); //for testing
            Motor_Forward(1900, 4100);
            data = Reflectance_Read(1500);
            weight = Reflectance_Position(data);
        }
        if (weight > 0)
        { //if you went too far to the left
            state = CORRECTRIGHT; //adjust by going more to the right
        }
        if (weight == 0)
        { //its centered
            state = FORWARD; //go forward
        }
        else
        { //otherwise stay correcting to the left
            state = CORRECTLEFT;
        }

        break;

        state = FORWARD; //default state of switch statement

    } //end switch
    DelayTenMilliseconds(); //Delay 10ms to pace state machine
// P2OUT ^= 0b00000100;//Toggle LED to demonstrate 10ms delay
}

}
