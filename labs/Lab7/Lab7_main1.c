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
uint32_t pulseIn (void);

void TimerInit(void)
{
    //**********First initialize TimerA0 for PWM
    //Since the motors are connected to P2.6 and P2.7, use TimerA0, compare blocks 3 & 4****
    TA0CCR3 = 14999;
    TA0CCR4 = 14999;


    //***********Now initialize TimerAx for the delay function************//
    TA0CTL &= ~0x0030; //stop the timer
    TA0CTL |= 0x0200; TA0CTL &= ~0x0100; //choose SMCLK for the clock source
    TA0CTL |= 0x0040;//choose clock divider of 2 - ID: 01
    //TA0CTL &= 0x0080;
    //set the period timer
    TA0CCR0 = 59999;
    //outmode 7
    TA0CCTL3 |= 0x00E0; //Outmode 7: reset/set
    TA0CCTL4 |= 0x00E0; //Outmde 7: reset/set


    //TimerA2
    TA2CTL &= ~0x0030;//stop the timer
    TA2CTL |= 0x0200; TA2CTL &= ~0x0100;//choose SMCLK for the clock source
    TA2CTL |= 0x00C0; //choose clock divider of 8 - ID: 11
    //TA2CTL &= 0x0000;
    //TA2CCR0 = 59999; //configure compare register for 10ms delay


    //TimerA3
    TA3CCTL2 |= 0x00E0; // set  reset
    TA3CTL &= ~0x0030; //stop the timer
    TA3CTL |= 0x0200; TA3CTL &= ~0x0100; //choose SMCLK for clock source
    TA3CTL |= 0x0080; //clock divider of 4 - ID: 10 on pins 7-6
    TA3CCR2 = 59999; //configure compare register for 20ms delay
}

void ServoInit(void)
{
    Servo(4400);     //call Servo() function to center servo
    Clock_Delay1us(100000);     //delay here to give servo time to move - can use built in timer function
    TA3CTL &= ~0x0030;     //stop the timer
    return;
}
void Servo(uint16_t angle_count)
{
    TA3CCR0 = 59999;      //set period for 20ms
    TA3CCR2 = angle_count;      //set high time for the input angle using angle_count
    TA3CTL |= 0x0010;      //set timer for up mode
    return;
}
uint16_t distanceInCm(void) {
    uint16_t distance;
    uint16_t time;
    P6OUT |= TRIGGER;      //drive trigger pin high
    Clock_Delay1us(10);      //wait 10 us - can use built-in timer function
    P6OUT &= ~TRIGGER;      //drive trigger pin low
    time = pulseIn();

    //distance = ((time*0.034)/2);
    distance = ((time*34)/2000);
    //distance = ((time*0.0155)/2);      //calculate distance using s=t * 0.034/2. t comes from pulseIn() function


    if(distance == 0){
        distance = 400;      // if no echo (distance = 0), assume object is at farthest distance
    }
    return distance;      //return the distance
}
uint32_t pulseIn (void)
{
    uint16_t width = 0;
    uint16_t time = 0;
    uint16_t maxcount = 56999;  //max count for 38 ms (timeout)
    TA2CTL |= 0x0020;           //set timer for continuous mode
//    TA2CCR0 = maxcount;
    TA2R = 0; //reset timer count
    //int i = 0;

   // for(i = 0; i< 8; i++){


  //  }
    while((P6IN & ECHO)==0){                   //wait for the pulse to finish (while Echo is high)
         if(TA2R >= maxcount){//if count is greater than maxcount return 0
             return 0;
         }
    }
     TA2R=0;
    while(P6IN & ECHO){                    //wait for the pulse to finish (while Echo is high)
        if(TA2R >= maxcount){//if count is greater than maxcount return 0
            return 0;
        }
    }
    
    width = TA2R;               //read the count (width of the return pulse)
    TA2CTL &= ~0x0030;               //stop the timer
    time = width*(1.5);               // convert the reading to microseconds.
    return time;               //return the microsecond reading
}

void main(void)
{

    uint16_t distance;
    uint16_t right_wall;
    uint16_t left_wall;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    Clock_Init48MHz();  // makes bus clock 48 MHz
    //call all the port initialization functions
    TimerInit();
    ServoInit();
    Port6_Init();
    Port8_Init();
    Port2_Init();
    Port3_Init();
    Port5_Init();

    //These are the states of the state machine
    enum motor_states {FORWARD, BACKWARD, RIGHT, LEFT, SWEEP_RIGHT, SWEEP_LEFT} state, prevState;

    state = FORWARD;          //start in FORWARD state
    prevState = !FORWARD;   //used to know when the state has changed
    uint16_t stateTimer = 0;           //used to stay in a state
    bool isNewState;              //true when the state has switched


    while(1) {

      /*  printf("%u \n", distance);
        printf("Distance read is : %d \n", distanceInCm());
        Clock_Delay1ms(500);*/


        isNewState = (state != prevState);
            prevState = state;
        distance = distanceInCm();  //this needs to be moved to the states that use it

        switch (state) {
        case FORWARD:
            if(isNewState){
                Servo(4400);
                distance = distanceInCm(); //find distance
                Motor_Forward(12750, 13055); //call motor function
            }
            else if(distance <= 10){
                state = BACKWARD; //move back
                                }
            break;
        case BACKWARD:
            if(isNewState){
                stateTimer = 0; //reset timer
                Motor_Backward(12750, 13055); //call motor function
            }
            stateTimer++; //increment timer
            if(stateTimer >= 40){
                Motor_Stop(); //stop motors
                state = SWEEP_RIGHT; //switch state
            }
            break;
        case SWEEP_RIGHT:
            Servo(4400); //move servo to middle
            Clock_Delay1us(1000000); //delay
            Servo(2000); //move servo right
            Clock_Delay1us(1000000); //delay
            distance = distanceInCm(); //find distance
            if(distance <= 150){
                right_wall = distance; //set the distance to right wall if close enough

            }
            else if(distance > 150){
                right_wall = 400; //set right to 400 if too far

            }
           // Servo(4400); //move servo to middle
            Clock_Delay1us(1000000); //delay
            state = SWEEP_LEFT; //switch state
            break;
        case SWEEP_LEFT:
            Servo(4400); //move servo to middle
            Clock_Delay1us(1000000); //delay
            Servo(7500); //move servo left
            Clock_Delay1us(1000000); //delay
            distance = distanceInCm(); //find distance
            if(distance <= 150){
                left_wall = distance; //set left wall to distance if close enough
            }
            else if(distance > 150){
                left_wall = 400; //if too far, set left wall 400
            }
            Servo(4400); //move servo to middle
            Clock_Delay1us(1000000); //delay
            if(left_wall > right_wall){ //move based on how far each wall is
                state = LEFT; //switch state
            }
            else if(right_wall >= left_wall){
                state = RIGHT; //switch state
            }
            break;
        case RIGHT:
            if(isNewState){
                stateTimer = 0; //reset timer
                Motor_Right(12750, 0); //call motor function
            }

            stateTimer++; //increment timer
            if(stateTimer >= 45){
                state = FORWARD; //switch state
            }
            break;
        case LEFT:
            if(isNewState){
                stateTimer = 0; //reset timer
                Motor_Left(0, 13499); //call motor function
            }

            stateTimer++; //increment timer
            if(stateTimer >= 45){
                state = FORWARD; //switch timer
            }
            break;
        default: state = FORWARD;
        } //end switch state
        Clock_Delay1ms(20); //delay state machine
        printf("%u", distance);


    } //end while
    return;
} //end main
