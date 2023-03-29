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
    TA2CTL     = 0x0252;      //enable timer SMCLK/2 and UP mode
                              //timerA2 interrupt enabled
    TA2EX0     = 0x0002;      //divide by 3 (total of /6)
    TA2CCR0    = 62499;       //configured for 125ms

    NVIC -> ISER[0] |= 0x0002000; //enable the TA2N interrupt (13)
    TA2CTL  &= ~ BIT0;        //clear any pending flag (TAIFG)
    TA2R = 0;                 //reset timer to zero
    EnableInterrupts();       //global interrupt enable
    return;
}

void TA2_N_IRQHandler(void) {
    //reading the TA2IV register clears the
    //highest priority interrupt flag
    uint16_t status = TA2IV;
    //set flag only if correct interrupt
    if (status == 0x000E) wasInterrupt = true;
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
