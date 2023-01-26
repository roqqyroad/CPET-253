#include <msp.h> 
#include <stdint.h>
#include <stdbool.h>

//This is an example of a SW state machine
//The state machine functions as the directional lights on a Mustang
//The pattern for both sides is similar:  
//• All three bulbs are off when not in use
//• When the driver raises (or lowers) the directional lever the following happens
//– The inside bulb goes on
//– The inside bulb stays on and the middle bulb goes on
//– The inside and middle bulb stay on and the outside bulb goes on
//• After the outside bulb goes on 
//– If the lever is still up (or down), the sequence above repeats 
//– If the lever is in neutral position, all lights go off
//• If the lever is returned to the neutral position prior to all 3 bulbs 
//  being on, it is ignored

//put the lights on PORT1 pins 7 -2
//put the levers on PORT2 pins 0-1 
#define Right_inner  BIT4                               
#define Right_middle BIT3
#define Right_outer  BIT2
#define Left_inner   BIT5
#define Left_middle  BIT6
#define Left_outer   BIT7
#define lever_up     BIT0     //if the lever is up, P2.0 is 1
#define lever_down   BIT1     //if the lever is down, P2.1 is 1

void main() {

   WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer - always include this   
    
    
//These are the seven states of the state machine
enum mustang_blinker {OFF, R_ONE_ON, R_TWO_ON, R_THREE_ON, L_ONE_ON, L_TWO_ON, L_THREE_ON} state, prevState;

state = OFF;          //start in OFF state
prevState = !OFF;   //used to know when the state has changed
uint8_t stateTimer;           //used to stay in a state
volatile bool isNewState;              //true when the state has switched
volatile bool isLEVERup;               //true when the lever is up
volatile bool isLEVERdown;             //true when lever is down
volatile uint16_t i;        // iteration counter for delay loop


    P1DIR |= 0b11111100;     //make P1.7, P1.6, P1.5, P1.4, P1.3, P1.2 outputs
    P1OUT &= 0b00000011;     //turn all the lights off
    P2DIR &= 0b11111100;     //make P2.0 and P2.1 inputs

    while(1) {
        // read inputs
        isLEVERup = ((P2IN & BIT0) == BIT0);
        isLEVERdown = ((P2IN & BIT1) == BIT1);
        // update state information
        // if state is not equal to prevState, then it's a new state
        isNewState = (state != prevState); 
        prevState = state;  //save state for next time

        switch (state) {       
            case OFF:
                //entry housekeeping
                P1OUT &= 0x03;  //turn off all outputs. Not necessary but doesn't hurt
                //state business - none
                if (isLEVERup) {          //exit condition 1 -right side
                    state = R_ONE_ON;
                }
                if (isLEVERdown) {        //exit condition 2 - left side
                    state = L_ONE_ON;
                }
            break;
            case R_ONE_ON:
                //entry housekeeping
                if (isNewState) {
                    stateTimer = 0;
                }
                P1OUT |= Right_inner;    //turn right inner on   
                P1OUT &= 0b11110011;    //turn off right middle and outer 
                //state business
                stateTimer++;  //increment timer, stay in state for 5
                //exit condition 1
                if (stateTimer >= 5) {
                    state = R_TWO_ON;
                }
            break;
            case R_TWO_ON:
                //entry housekeeping
                if (isNewState) {
                    stateTimer = 0;
                }
                P1OUT |= Right_middle;  //turn on middle light          
                                        //inner light is already on
                stateTimer++;  //increment timer, stay in state for 5
                //exit condition 1
                if (stateTimer >= 5) {
                    state = R_THREE_ON;
                }
            break;
            case R_THREE_ON:
                //entry housekeeping
                if (isNewState) {
                    stateTimer = 0;
                }
                P1OUT |= Right_outer;   //turn on outer light
                                     //innder and middle lights are already on
                stateTimer++;  //increment timer, stay in state for 5
                //exit condition 1 -lever still up
                if ((stateTimer >= 5) && (isLEVERup)){   
                    state = R_ONE_ON;
                }
                //exit condition 2 - lever returned to neutral
                if ((stateTimer >= 5) && !(isLEVERup)){   
                    state = OFF;
                    P1OUT &= 0b00000011;  //turn all lights off
                }
            break;
            case L_ONE_ON:
                //entry housekeeping
                if (isNewState) {
                    stateTimer = 0;
                }
                P1OUT |= Left_inner;    //turn left inner on     
                P1OUT &= 0b00111111;    //turn off left middle and outer 
                //state business
                stateTimer++;  //increment timer, stay in state for 5
                //exit condition 1
                if (stateTimer >= 5) {
                    state = L_TWO_ON;
                }
            break;
            case L_TWO_ON:
                //entry housekeeping
                if (isNewState) {
                    stateTimer = 0;
                }
                P1OUT |= Left_middle;  //turn on middle light
                                       //inner light is already on
                stateTimer++;  //increment timer, stay in state for 5
                //exit condition 1
                if (stateTimer >= 5) {
                    state = L_THREE_ON;
                }
            break;
            case L_THREE_ON:
                //entry housekeeping
                if (isNewState) {
                    stateTimer = 0;
                }
                P1OUT |= Left_outer;   //turn on outer light
                             //inner and middle lights already on
                stateTimer++;  //increment timer, stay in state for 5
                //exit condition 1 -lever still down
                if ((stateTimer >= 5) && (isLEVERdown)){    
                    state = L_ONE_ON;
                }
                //exit condition 2 - lever returned to neutral
                if ((stateTimer >= 5) && !(isLEVERdown)){   
                    state = OFF;
                    P1OUT &= 0b00000011;  //turn all lights off
                }
            break;
            default: state = OFF;
        } //end switch
    for(i=10000; i>0; i--);  //random delay loop
    } //end while
} //end main
