/** Name: Rachel DuBois | Section: Lab 02 */

//Determine which header files to include
#include "msp.h"
#include <stdint.h>
#include"..\inc\Clock.h"
#include"..\inc\CortexM.h"
#include"..\inc\Init_Ports.h"

//main.c

//TIMERINIT FUNCTION
void TimerInit(void)
{
    //stop the timer
    TA1CTL &= ~0x0030;
    //choose SMCLK for the clock source
    TA1CTL |= 0x0200;
    TA1CTL &= ~0x0100;
    //choose clock divider of 2
    TA1CTL |= 0x0040; //0b0000 0010 0100 0000
    //Outmode 7: reset/set
    TA1CCTL3 = 0x00E0;
    TA1CCTL4 = 0x00E0;

    TA1CCR0 = 39999; //Total counts given a divider of 2 is 40k counts
    TA1CCR3 = 11999;  //Duty cycle of 30% --30% of 40k is 12k so 12k counts

    //---- THIS IS HERE FOR TESTING PURPOSES WITH THE OSCOPE
    //TA1CTL |= 0x0010; //Starts the counting in up mode for PWM
}

//MAIN FUNCTION
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    Clock_Init48MHz(); // makes bus clock 48 MHz

    //call port initialization functions
    Port1_Init();
    Port7_Init();

    TimerInit();    //call TimerInit function

    while (1)
    {

        if ((P1IN &= 0b00010000) == 0) //If the pushbutton is pressed, it will be zero since active low. If it is being pressed....
        {
            //Do not set TA1R because it will mess up the output waveform and only show it when debugging pauses
            TA1CTL |= 0x0010; //Starts the counting in up mode for PWM
        }
        else //If the pushbutton is not being pressed....
        {
            TA1CTL &= ~0x0030; //Stop the timer since the button is not being pressed at the moment
        }
    }

}

