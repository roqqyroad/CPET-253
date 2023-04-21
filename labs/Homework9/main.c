#include "msp.h"
#include <stdint.h>

volatile bool C1 = false;
volatile bool C2 = false;
volatile bool C3 = false;
volatile bool timerInterrupt = false;

void Port_Init(void){
    //push buttons
    P4DIR &= 0b11110000; //set as input
    P4REN |= 0b00001111;
    P4OUT |= 0b00001111; //active low push buttons
    P4SEL0 &= ~0b00001111;
    P4SEL1 &= ~0b00001111;
    P4IE |= 0b00000111; //enable interrupts on pins 0, 1, 2
    P4IES |= 0b00000111; //Falling edge interrupt
    P4IFG &= ~0b00000111; //clear interrupt flag
    NVIC -> ISER[0] |= 0x0001000;

    //LEDs
    P2DIR |= 0b11110000; //set as output
    P2OUT &= 0b00000000; //all LED off
    return;
}//end Port init

void PORT4_IRQHANDLER(void){
    uint16_t status = P4IV; //see which button was pressed
    if (status == 0x02) {
        C1 = true;
    }
    else if (status == 0x04) {
        C2 = true;
    }
    else if (status == 0x06){
        C3 = true;
    }
    P4IFG &= 0x00; //clear interrupt flag
    return;
} //end IRQHandler

void TimerInterruptInit(void){
    TA2CTL = 0b0000000011000010; //TAxCLK, div of 8, stop mode, interrupt enabled
    TA2EX0 = 0x0005; //div of 6
    TA2CCR0 = 62499;
    TA2CCR3 = 62499;
    TA2CCTL3 |= BIT4; //enable interrupt for TimerA2
    NVIC -> ISER[0] |= 0x0002000;
    TA2CCTL3 &= 0x00; //clear any flags
    TA2R = 0; //reset timer
    return;
}//end TimerInterruptInit

void TA2_N_IRQHandler(void){
     timerInterrupt = true;
     TA2CCTL3 &= 0x00; //clear flags
}//end IRQHandler

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Clock_Init48MHz();
	Port_Init();
	TimerInterruptInit();
	EnableInterrupts();
	while(1){
	    if(P4IN & 0b00001000){
	        P2OUT &= 0x00; //Turn all LED off
	        TA2R = 0; //reset timer to 0
	        TA2CTL |= 0b0000000011010010; //timer in UP mode, starting timer
	        } //end start button if
	    if(C1){
	        P2OUT |= 0b10000000; //turn P2.7 on
	        TA2CTL &= ~0x0030; //stop timer
	        C1 = false;
	    }//end contestant 1 if
	    if(C2){
	        P2OUT |= 0b01000000; //turn P2.6 on
	        TA2CTL &= ~0x0030; //stop timer
	        C2 = false;
	    } //end contestant 2 if
	    if(C3){
	        P2OUT |= 0b00100000; //turn P2.5 on
	        TA2CTL &= ~0x0030; //stop timer
	        C3 = false;
	    }//end contestant 3 if
	    if(timerInterrupt){
	        P2OUT |= 0b00010000;  //turn P2.4 on
	        TA2CTL &= ~0x0030; //stop timer
	        timerInterrupt = false;
	    } //end timerInterrupt if

	}//end while

}//end main
