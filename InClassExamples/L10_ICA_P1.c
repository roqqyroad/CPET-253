#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"

/** This program enables TimerA to cause an interrupt
 *  The main program does the following:
 *  If the TimerA ISR is entered, LED1 is toggled
 **/

#define LED1 BIT0    //Port 1 bit 0

// global variables
volatile bool wasInterrupt = false;

void PortInit (void) {
    P1DIR  |=  LED1;    //Create LED1 output
    P1OUT  &= ~LED1;    //Turn off LED to start
    P1SEL0 &= ~LED1;    //Select GPIO output
    P1SEL1 &= ~LED1;

    return;
}

void InterruptInit(void) {
    TA2CTL     = 0x0290;      //enable timer SMCLK/4 and UP mode
    TA2EX0     = 0x0005;      //divide by 6 (total /24)
    TA2CCR0    = 62499;       //configured for 500ms
    TA2CCTL0  |= BIT4;        //enable interrupt for TA2 CCR0
    NVIC -> ISER[0] |= 0x0001000; //enable the TA2N interrupt (12)
    TA2CCTL0  &= ~ BIT0;      //clear any pending flag (CCIFG)
    TA2R = 0;                 //reset timer to zero
    EnableInterrupts();       //global interrupt enable
    return;
}

void TA2_0_IRQHandler(void) {
    //set flag only if correct interrupt
    wasInterrupt = true;
    //Clear the interrupt before leaving ISR
    TA2CCTL0  &= ~ BIT0;      //clear any pending flag (CCIFG)
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;// stop watchdog timer
    DisableInterrupts();  //disable interrupts before configuration
    PortInit();           //initialize input and output pins
    InterruptInit();

    while(1)
    {
      if (wasInterrupt)
      {
          P1OUT ^= LED1;           //Toggle LED1
          wasInterrupt = false;    //Clear flag
      }
    }
}
