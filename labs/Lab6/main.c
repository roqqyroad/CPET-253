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
        // Reflectance_Read(1500); //for first signoff

        isNewState = (state != prevState);
        prevState = state; //save state for next time
        switch (state)
        {
        case FORWARD:
            Motor_Forward(1275, 1305); //call driving function
            if (result != 0b00011000)
            { //if it is not centered,
                if ((result = 0b10000000) | (result = 0b11000000 | result =
                        0b0100000) | (result = 0b01100000) | (result =
                        0b00100000) | (result = 0b00110000) | (result =
                        0b00010000))
                {

                    state = TURNLEFT;
                }
                if ((result = 0b00000001) | (result = 0b00000011 | result =
                        0b0000010) | (result = 0b00000110) | (result =
                        0b00000100) | (result = 0b00001100) | (result =
                        0b00001000))
                {

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
                Motor_Right(1275, 50); //turning slowly
            }
            stateTimer++;
            if (result = 0b00011000)
            {
                stateTimer = 0;
                state = FORWARD;
                break;
                case TURNLEFT:
                if (isNewState)
                {
                    stateTimer = 0; //reset timer
                    Motor_Left(50, 1305); //turning slowly
                }
                stateTimer++;
                if (result = 0b00011000)
                {
                    stateTimer = 0;
                    state = FORWARD;
                }
                break;
                state = FORWARD; //default state of switch statement
            } //end switch
            DelayTenMilliseconds(); //Delay 10ms to pace state machine
            // P2OUT ^= 0b00000100;//Toggle LED to demonstrate 10ms delay
        }

    }
