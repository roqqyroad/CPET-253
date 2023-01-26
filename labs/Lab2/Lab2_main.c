/****************************************************************************************
         CPET253 Lab2 - State Machine Review and Motor Drivers

 Jeanne Christman
 original version 6/1/2022

 This program uses a state machine to control the TI-RSLK robot to drive
 in a square

 It uses several function provided by TI as well as the Cortex SysTick
 peripheral
  •inc/Clock.h and inc/Cortex.h must be included and Clock.c and Cortex.c need to be in the project
  •the function Clock_Init48MHz() makes the system clock 48MHz (period = 20.83 ns)
  •To use SysTick to create a delay, do the following
      -Set SysTick -> LOAD = the delay you want to create. 
      -Set SysTick -> VAL = 0 to start the count at 0
      -Set SysTick -> CTRL = 0x00000005 to enable the clock
      -Wait for 0x00010000 to be true to indicate time is up
      -See MSP432 datasheet for more information on SysTick

 To control the motors on the TI-RSLK robot, there are three outputs that need
 to be driven.
    :Pin    :Description            :Notes
    :=======:=======================:=========================
    : P5.5  : Right motor direction : 0=forwards, 1=backwards
    : P3.6  : Right motor sleep     : 0=sleep, 1=awake
    : P2.6  : Right motor PWM       : 0=stop, PWM signal = go
    : P5.4  : Left motor direction  : 0=forwards, 1=backwards
    : P3.7  : Left motor sleep      : 0=sleep, 1= awake
    : P2.7  : Left motor PWM        : 0=stop, PWM signal = go

 Functions in this code:
     -MotorForward(void) - set both motors to forward, use SysTick to create PWM
      with 10 ms period and 25% duty cycle
     -MotorBackward(void) - set both motors to backward, use SysTick to create PWM
      with 10 ms period and 25% duty cycle
     -MotorTurnRight(void) - set left motor to forward and right motor to sleep,
      use SysTick to create PWM with 10 ms period and 25% duty cycle
     -MotorTurnLeft(void) - set right motor to forward and left motor to sleep,
      use SysTick to create PWM with 10 ms period and 25% duty cycle
     -MotorStop(void) - nice but not required, used to stop the motors at end of square

The state machine has 4 states; forward, right, left, backward
use FSM to make a pattern: Forward, right turn 90 degrees, backwards, left turn 90, forward...
*******************************************************************************************/

#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\Init_Ports.h"


#define RIGHT_MOT_DIR    0x20  //0b0010 0000  P5.5
#define RIGHT_MOT_SLEEP  0x40  //0b0100 0000  P3.6
#define RIGHT_MOT_PWM    0x40  //0b0100 0000  P2.6
#define LEFT_MOT_DIR     0x10  //0b0001 0000  P5.4
#define LEFT_MOT_SLEEP   0x80  //0b1000 0000  P3.7
#define LEFT_MOT_PWM     0x80  //0b1000 0000  P2.7


void MotorStop (void)
//This function stops the motors by putting 0 on PWM pins and then puts
//motors to sleep
{
    P2OUT &= ~RIGHT_MOT_PWM & ~LEFT_MOT_PWM;       //stop motors
    P3OUT &= ~RIGHT_MOT_SLEEP & ~LEFT_MOT_SLEEP;   //put motors to sleep
    return;
}
void MotorForward (void)
//This function is used to drive both motors in the forward direction
//It uses SysTick to create a PWM wave with a period of 10 ms and 25% duty cycle
//The signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the EN pins of the motors
{
     P3OUT |= RIGHT_MOT_SLEEP | LEFT_MOT_SLEEP;  //wake up motors
     P5OUT &= ~RIGHT_MOT_DIR & ~LEFT_MOT_DIR;  //motors forward

     P2OUT |= RIGHT_MOT_PWM | LEFT_MOT_PWM;  //drive EN pins high for PWM
     // wait high time
     // The system clock is 48 Mhz,
     // for 25% duty cycle, high time is 2500 us
     // (2500us * 48MHz) = 120000
     SysTick -> LOAD = 120000;  //load the high time count into the timer
     SysTick -> VAL = 0;        //clear the timer to start at
     SysTick -> CTRL = 0x00000005;  //start the timer
     while (!(SysTick -> CTRL & 0x00010000));  //wait until the timer counts down to 0
     P2OUT &= ~RIGHT_MOT_PWM & ~LEFT_MOT_PWM;  //drive pins low for PWM
     // low time for 10ms period with 25% duty cycle is 7500us
     // (7500us * 48MHz) = 360000
     SysTick -> LOAD = 360000;  //load the low time count into the timer
     SysTick -> VAL = 0;
     SysTick -> CTRL = 0x00000005;
     while (!(SysTick -> CTRL & 0x00010000));
     return;
}
void MotorBackward (void)
//This function is used to drive both motors in the backward direction.
//It uses SysTick to create a PWM wave with a period of 10ms and 25% duty cycle
//The PWM signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the PWM pin

void MotorTurnRight (void)
//This function is used to drive left motor forward and right motor to sleep.
//It uses SysTick to create a PWM wave with a period of 10ms and 25% duty cycle
//The PWM signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the PWM pin

void MotorTurnLeft (void)
//This function is used to drive right motor forward and left motor to sleep.
//It uses SysTick to create a PWM wave with a period of 10ms and 25% duty cycle
//The PWM signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the PWM pin


void main(void)
{


       WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
       Clock_Init48MHz();  // makes bus clock 48 MHz
       //Call the appropriate functions from Init_Ports.c

       //These are the four states of the state machine
       enum motor_states {

       state =                    //start state
       prevState =                //used to know when the state has changed
       uint16_t stateTimer;       //used to stay in a state
       bool isNewState;           //true when the state has switched

       //through this while loop, every time one of the motor functions is called
       //it takes 10ms. Assume that the delay in each state is 10ms
       //time spent in any direction = stateTimer * 10ms
       while(1)
       {
           isNewState = (state != prevState);
           prevState = state;  //save state for next time

          switch (state) {
          //each case below should have entry housekeeping, state business and exit housekeeping
          //remember to reset the stateTimer each time you enter a new state
          //you must assign a new state when stateTimer reaches the correct value
          case
                  break;
          case
                  break;

          } //switch
       } //while(1)
   } //main()
