#include "msp.h"
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\Init_Ports.h"


#define PB1 BIT0
#define PB2 BIT1
#define PB3 BIT2
#define PBSTART BIT3

#define LED1 BIT7
#define LED2 BIT6
#define LED3 BIT5
#define LEDTO BIT4

volatile bool wasInterrupt = 0;
volatile uint8_t LED = 0b00000000;



void PORT4_IRQHandler(void){
    uint16_t status = P4IV;


    if (status == 0x02){ //P4.0
        LED &= ~LED1;
        LED |= LED1;

    }
    else if (status == 0x04){ //P4.1
         LED &= ~LED2;
         LED |= LED2;

     }
    else if (status == 0x06){ //P4.2
        LED &= ~LED3;
        LED |= LED3;

    }
    else if (status == 0x08){ //P4.3 start pb
        //start timer

    }
    wasInterrupt = true;
    P4IFG &= 0x00;//clear flag

}
/*
void Timer_Init(void){
    //Timer A2 interrupts to tell when 1 second has passed
    //1sec = counts 1/12MHz; too many counts

}
*/



/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	DisableInterrupts();
    Clock_Init48MHz();  // makes bus clock 48 MHz
    Port2_Init();
    Port4_Init();
    //Timer_Init();
    EnableInterrupts();

	while(1){
	    if(wasInterrupt){
	        wasInterrupt = false;
	        P2OUT &= ~LED;
	        P2OUT |= LED;
	    }
	}
}
