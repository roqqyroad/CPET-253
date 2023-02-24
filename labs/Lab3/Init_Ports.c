// Init_Ports.c
// Runs on MSP432
// Provide functions that initialize ports 
// Prof. Christman
// 1/8/2023

/* These functions will be used in all of the labs for CPET253

 There is an initialization function for each of the ports 1-10 and J.

 The initializations will be different for each lab.
 */

#include "msp.h"

// ------------Port1_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port1_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}

// ------------Port2_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port2_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state

     P2DIR |= 0b00000100; // Sets pin 2 to outputs.
     P2DIR |= BIT6;
     P2DIR |= BIT7; //set pins 6 and 7 to outputs
     P2DIR |= BIT4; //set bit4 as an output for TimerA0



    P2OUT &= 0b11111011; // Sets initial value to zero for LED.

    //P2DIR = 0b1101 0100
    P2SEL1 &= ~0xC0;
    P2SEL0 |= 0xC0; //sets for each pin, not for whole thing.

}

// ------------Port3_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port3_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state

    P3DIR |= BIT6;
    P3DIR |= BIT7;  //Set pins 6 and 7 to outputs
    P3OUT &= 0b00000000;
}

// ------------Port4_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port4_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}

// ------------Port5_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port5_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state

    P5DIR |= BIT5;
    P5DIR |= BIT4; //set buts 5 and 4 to outputs
}

// ------------Port6_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port6_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}

// ------------Port7_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port7_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}

// ------------Port8_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port8_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}

// ------------Port9_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port9_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}

// ------------Port10_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void Port10_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}

// ------------PortJ_Init------------
// First set the pin directions
// Enable resistors for inputs
// Set input resistors for pull-up or pull-down
// Enable alternate pin functions, if appliable
// Drive initial values on outputs
// Input: none
// Output: none
void PortJ_Init(void)
{
    // Use the port direction register to configure pins as inputs and output
    //    -A 1 sets a pin as an output. Use |= with a bit mask to set 1's.
    //    -A 0 sets a pin as an input. Use &= with a bit mask to set 0's.
    // Use the resistor enable register to enable resistors for inputs that need them
    // Use the port output register to configure the resistors as pull-ups or pull-downs
    // Use the PxSEL0 and PxSEL1 registers to enable alternate pin functions, if applicable
    // Use the port output register to put outputs to an initial state
}
