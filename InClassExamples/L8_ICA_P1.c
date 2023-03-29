#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"


/** This program enables pins P1.1 and P1.4 as interrupt sources
 *  The main program does the following:
 *  If a P1.1 interrupt is detected, LED2 (on the launchpad board) is lit with green (P2.1)
 *  If a P1.4 interrupt is detected, LED2 is lit with blue (P2.2)
 **/

#define GREEN BIT1    //bit 1
#define BLUE  BIT2    //bit 2
// global variables
volatile bool isPin1 = false;
volatile bool isPin4 = true;

void PinInit1 (void) {
    P1DIR  &= ~BIT4 & ~BIT1;  //put a 0 on P1.4 and 1.1 to make them inputs
    P1REN  |= BIT4 | BIT1;    // enable resistors on P1.4 and P1.1
    P1OUT  |= BIT4 | BIT1;    // make it a pull-up resistor
    P1SEL0 &= ~BIT4 & ~BIT1;  //use P1.4 and P1.1 as GPIO
    P1SEL1 &= ~BIT4 & ~BIT1;

    P2DIR |= BIT1 | BIT2;     //make P2.1 and P2.2 outputs
    P2OUT &= ~BIT1 & ~BIT2;   //turn off the LEDs
    return;
}

void InterruptInit1(void) {
    P1IE |= BIT1 | BIT4;      //enable interrupts on P1.4 and P1.1
    P1IES |= BIT1 | BIT4;     //make them falling edge interrupts
    NVIC -> ISER[1] |= 0x00000008;      //enable the port 1 interrupt
                              //port1 interrupt is number 35, so bit 3 is set (interrupt 32 is bit 0)
    P1IFG &= ~BIT1; P1IFG &= ~BIT4;  //clear the flags in case there is a random button push
    EnableInterrupts();       //global interrupt enable
    return;
}

void PORT1_IRQHandler1(void) {
    //determine which interrupt triggered
    //P1IV = 2*(n+1) where n is the pin number
    uint16_t status = P1IV;
    if (status == 0x04) {
        isPin1 = true;  //pin1.1 triggered
        P1IFG &= ~BIT1;  //clear the flag
        return;
    }
    if (status == 0x0A) {
        isPin4 = true;  //pin1.4 triggered
        P1IFG &= ~BIT4; //clear the flag
        return;
    }
}

void main1(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Clock_Init48MHz();  // makes bus clock 48 MHz
	PinInit();          //initialize input and output pins
	InterruptInit();
	uint32_t i;

	while(1)
	{
	  if (isPin1)
	  {
	      P2OUT |= GREEN;
	      for (i=500000;i>0;i--);  //delay so LED is seen
	      P2OUT &= ~GREEN;        //turn it off
	      isPin1 = false;              //set it false again
	  }
	  if (isPin4)
	  {
	      P2OUT |= BLUE;
	      for (i=500000;i>0;i--);  //delay so LED is seen
	      P2OUT &= ~BLUE;        //turn it off
	      isPin4 = false;              //set it false again
	  }
	}
}
