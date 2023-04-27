// Motor.c
// Runs on MSP432
// Provides functions that control robot motion
// using built in TimerA module 
// Prof. Dickens
// 1/26/2023

/* These functions will be used in all of the labs for CPET253
// Left motor direction connected to P5.4
// Left motor PWM connected to P2.7/TA0CCP4
// Left motor enable connected to P3.7
// Right motor direction connected to P5.5
// Right motor PWM connected to P2.6/TA0CCP3
// Right motor enable connected to P3.6 */

#include <stdint.h>
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"

#define RIGHT_MOT_DIR    0x20  //0b0010 0000  P5.5
#define RIGHT_MOT_SLEEP  0x40  //0b0100 0000  P3.6
#define RIGHT_MOT_PWM    0x40  //0b0100 0000  P2.6
#define LEFT_MOT_DIR     0x10  //0b0001 0000  P5.4
#define LEFT_MOT_SLEEP   0x80  //0b1000 0000  P3.7
#define LEFT_MOT_PWM     0x80  //0b1000 0000  P2.7

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(volatile uint16_t leftDuty, volatile uint16_t rightDuty){
    P3OUT |= ~LEFT_MOT_SLEEP; //left sleep
    P3OUT |= ~RIGHT_MOT_SLEEP; //right sleep
    leftDuty = 0;
    rightDuty = 0;

    TA0R = 0; //counter, start at 0 once on
    TA0CCR3 = rightDuty; //right side high time goes capture/compare unit 3
    TA0CCR4 = leftDuty; //left side high time goes capture/compare unit 4

    TA0CTL |= 0x0010; //start counting by setting mode to UP
    return;
}

// ------------Motor_Forward------------
// Drive the robot forward by running left and
// right wheels forward with the provided input parameters
// Input: leftDuty  duty cycle of left wheel 
//        rightDuty duty cycle of right wheel 
// Output: none
// Assumes: IO ports and Timers have been initialized.
void Motor_Forward(volatile uint16_t leftDuty, volatile uint16_t rightDuty ){
   // This function sets the motors to drive forward with a PWM signal fixed at 10ms
   // having a duty cycle input when the function is called for each motor independently

    P3OUT |=  RIGHT_MOT_SLEEP; //wake up right motor
    P3OUT |=  LEFT_MOT_SLEEP;  //wake up left motor

    P5OUT &= ~RIGHT_MOT_DIR;   //set right motor in forward direction
    P5OUT &= ~LEFT_MOT_DIR;    //set left motor in forward direction

    //No need to drive P2OUT since Timer will drive it directly
    //This is configured via the PxSEL bits

    TA0R = 10;                  //counter, start at zero once turned on
    TA0CCR3  = rightDuty;      //right side high time goes in Capture/compare unit 3
    TA0CCR4  = leftDuty;       //left side high time goes in Capture/compare unit 4

    TA0CTL  |= 0x0010;         // start counting by setting mode to UP
    return;
  }

// ------------Motor_Right------------
// Turn the robot to the right by running the
// left wheel forward and stopping the right wheel
// Input: leftDuty  duty cycle of left wheel 
//        rightDuty duty cycle of right wheel 
// Output: none
// Assumes: IO ports and Timers have been initialized.
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty){ 
    P3OUT &= ~RIGHT_MOT_SLEEP; //right sleep
    P3OUT |= LEFT_MOT_SLEEP; //wake up left

    P5OUT &= ~LEFT_MOT_DIR; //left set forward

    TA0R = 0; //counter, start at 0 once on
    TA0CCR3 = rightDuty; //right side high time goes capture/compare unit 3
    TA0CCR4 = leftDuty; //left side high time goes capture/compare unit 4

    TA0CTL |= 0x0010; //start counting by setting mode to UP
        return;
}

// ------------Motor_Left------------
// Turn the robot to the left by stopping the
// left wheel and running the right wheel
// forward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel 
//        rightDuty duty cycle of right wheel 
// Output: none
// Assumes: IO ports and Timers have been initialized.
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty){ 
    P3OUT &= ~LEFT_MOT_SLEEP; //left sleep
    P3OUT |= RIGHT_MOT_SLEEP; //wake up right

    P5OUT &= ~RIGHT_MOT_DIR; //right set forward

    TA0R = 0; //counter, start 0 once on
    TA0CCR3 = rightDuty; //right side high time goes capture/compare unit 3

    TA0CTL |= 0x0010; //start counting by setting mode to UP
    return;
}

// ------------Motor_Backward------------
// Drive the robot backward by running left and
// right wheels backward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel
//        rightDuty duty cycle of right wheel
// Output: none
// Assumes: IO ports and Timers have been initialized.
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty){ 
    P3OUT |= LEFT_MOT_SLEEP; //wake up left
    P3OUT |= RIGHT_MOT_SLEEP; //wake up right

    P5OUT |= LEFT_MOT_DIR; //left set backwards
    P5OUT |= RIGHT_MOT_DIR; //right set backwards

    TA0R = 0; //counter, start at 0 once on
    TA0CCR3 = rightDuty; //right side high time goes capture/compare unit 3
    TA0CCR4 = leftDuty; //left side high time goes capture/compare unit 4

    TA0CTL |= 0x0010; //start counting by setting mode to UP
    return;
}
