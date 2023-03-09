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


    P1DIR &= 0b11101111; //Sets P1.4 as an input
    P1REN |= 0b00010000; //Enables resistor on P1.4
    P1OUT |= 0b00010000; //Pullup resistor on P1.4


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

   // P5DIR |= BIT5;
    //P5DIR |= BIT4; //set buts 5 and 4 to outputs
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


    P7DIR |= 0b00100000;// 7.5 should be an output so it is useable for the sel bits
    P7SEL1 &= ~0b00100000; //7.5 sel1 should be = 0x00
    P7SEL0 |= 0b00100000; //7.5 sel 0 should be = 0x20
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
