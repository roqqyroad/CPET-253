#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "../inc/Clock.h"
#include "../inc/CortexM.h"


/** This program enables pin P1.1 as an interrupt source
 *  The main program does the following:
 *  If a P1.1 interrupt is detected, a count is incremented
 *  The count is then displayed to the console
 **/

// global variables
volatile uint32_t count = 0;

void PinInit (void) {
    P1DIR  &= ~BIT1;          //put a 0 on P1.1 to make input
    P1REN  |=  BIT1;          //enable resistor on P1.1
    P1OUT  |=  BIT1;          // make it a pull-up resistor
    P1SEL0 &= ~BIT1;          //use P1.1 as GPIO
    P1SEL1 &= ~BIT1;
    return;
}

void InterruptInit(void) {
    P1IE  |= BIT1;            //enable interrupt on P1.1
    P1IES |= BIT1;            //make it falling edge triggered
    NVIC -> ISER[1] |= 0x00000008;   //enable the port 1 interrupt
    //port1 interrupt is number 35, so bit 3 is set (interrupt 32 is bit 0)

    P1IFG &= ~BIT1;           //clear the flag in case there is a stored button push
    EnableInterrupts();       //global interrupt enable
    return;
}

void TimerA0Init(void) {
    //set up timer for a delay in up mode
    //taxccr0 = period
    //1s = 65536 * N/3MHz (default SMCLK)
    //N = 45.78 => 48   ID = 8, IDEX = 6
    //1s = counts * 48/3MHz
    //counts = 62,500
    TA0CTL   = 0x02D0;
    TA0R     = 0x0;
    TA0CCR0  = 62499;
    TA0EX0   = 0x5;
    return;
}

void TimerA0Delay(void) {
    //delay for 1 second then restart
    TA0R     = 0;            //restart timer
    TA0CCTL0 &= ~0x01;       //clear flag
    while (!(TA0CCTL0 & 1));
    TA0CCTL0 &= ~0x01;       //clear flag
    return;
}

void PORT1_IRQHandler(void) {
    //count the number of times this IRQ Handler is entered
    count++;
    uint16_t status = P1IV;
    P1IFG &= ~BIT1;           //clear the interrupt flag
    return;
}

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    PinInit();               //initialize input and output pins
    TimerA0Init();
    InterruptInit();

    while(1)
    {
        TimerA0Delay();// 1 second delay
        printf("count = %d \n", count);  // print to console window
    }

}
