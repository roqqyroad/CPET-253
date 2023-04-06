#include "msp.h"
#include <msp432.h>
#include <stdint.h>
#include <stdbool.h>
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/motor.h"
#include "../inc/Init_Ports.h"

#define TRIGGER 0x04
#define ECHO 0x08

#define microsecondsToClockCycles(a) ( (a) * 1.5 )       //assume 12Mhz clock divided by 8
#define clockCyclesToMicroseconds(a) ( (a) / 1.5 )       // 1.5 clock cycles = 1us

void Servo(uint16_t angle);
uint32_t pulseIn(void);

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

void ServoInit(void)
{
    Servo(4000);     //call Servo() function to center servo
    Clock_Delay1us(1000000); //delay here to give servo time to move - can use built in timer function
    TA3CTL &= ~0x0030;     //stop the timer
    return;
}
void Servo(uint16_t angle_count)
{
    TA3CCR0 = 59999;       //set period for 20ms
    TA3CCR2 = angle_count; //set high time for the input angle using angle_count
    TA3CTL |= 0x0010;     //set timer for up mode
    TA3R = 0;
    return;
}
uint16_t distanceInCm(void)
{
    uint16_t distance;
    float t; //for ease of use

    P6OUT |= TRIGGER;      //drive trigger pin high
    Clock_Delay1us(10);      //wait 10 us - can use built-in timer function
    P6OUT &= ~TRIGGER;      //drive trigger pin low
    t = pulseIn(); //to get time
    distance = (t * 0.034 / 2); //calculate distance using s=t * 0.034/2. t comes from pulseIn() function
    if (distance == 0)
    {
        distance = 400; // if no echo (distance = 0), assume object is at farthest distance
    }      //400 is the farthest distance
    return distance;      //return the distance
}
uint32_t pulseIn(void)
{
    uint16_t width = 0;
    uint16_t time = 0;
    uint16_t maxcount = 56999;  //max count for 38 ms (timeout)
    TA2CTL |= 0x0020;           //set timer for continuous mode

    TA2R = 0;                    //reset the count
    while(!(P6IN & ECHO))//wait for the pulse to start (while Echo is low)
    if (TA2R >= maxcount)
    {
        return 0;                   //if count is greater than maxcount return 0
    }

    TA2R = 0;                   //reset the count
    while(P6IN & ECHO)//wait for the pulse to finish (while Echo is high)
    if (TA2R >= maxcount)
    {                 //if count is greater than maxcount return 0
        return 0;
    }

    width = TA2R;               //read the count (width of the return pulse)
    TA2CTL &= ~0x0030;               //stop the timer
    time = width * (1.5);               // convert the reading to microseconds.
    return time;               //return the microsecond reading
}

void main(void)
{

    uint16_t distance, right_wall, left_wall;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    Clock_Init48MHz();  // makes bus clock 48 MHz
    //call all the initialization functions
    TimerInit();
    ServoInit();
    Port2_Init();
    Port3_Init();
    Port5_Init();
    Port6_Init();
    //Port7_Init();
    Port8_Init();
    //Port9_Init();

    //These are the states of the state machine
    /*enum motor_states
    {
        FORWARD, BACKWARD, RIGHT, LEFT, SWEEP_RIGHT, SWEEP_LEFT
    } state, prevState;

    state = FORWARD;          //start in FORWARD state
    prevState = !FORWARD;   //used to know when the state has changed
    uint16_t stateTimer = 0;           //used to stay in a state
    bool isNewState;              //true when the state has switched
*/
    while (1)
    {

        //distance = distanceInCm();
        printf("Distance read is : %d \n", distanceInCm());
        Clock_Delay1ms(500);

        /*

        isNewState = (state != prevState);
        prevState = state;
        distance = distanceInCm(); //this needs to be moved to the states that use it

        switch (state)
        {

        case FORWARD:

        case BACKWARD:

        case SWEEP_RIGHT:
        }
        Clock_Delay1ms(20);

        */

    } //while
}
