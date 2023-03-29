// BumpInt.c
// Runs on MSP432, interrupt version
// Provide low-level functions that interface bump switches on the robot.
// Daniel Valvano and Jonathan Valvano
// July 11, 2019

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// Negative logic bump sensors
// P4.7 Bump5, left side of robot
// P4.6 Bump4
// P4.5 Bump3
// P4.3 Bump2
// P4.2 Bump1
// P4.0 Bump0, right side of robot

#include <stdint.h>
#include "msp.h"
#include "stdbool.h"
// Initialize Bump sensors
// Make six Port 4 pins inputs
// Activate interface pullup
// pins 7,6,5,3,2,0
// Interrupt on falling edge (on touch)
volatile uint16_t Bump0 = 3;
volatile uint16_t Bump1 = 2;
volatile uint16_t Bump2 = 1;
volatile uint16_t Bump3 = 1;
volatile uint16_t Bump4 = 2;
volatile uint16_t Bump5 = 3;
volatile uint16_t count = 0;
volatile bool wasRight;
volatile bool wasInterrupt = 0;

void BumpInt_Init(void){
    P4DIR &= 0b00010010; //Set port pins
    P4REN |= 0b11101101; //Enable resistors
    P4OUT |= 0b11101101; //Enable pull up resistors
    P4IE |= 0b11101101; //Enable interrupts on pins 7,6,5,3,2,0
    P4IES |= 0b11101101;  //Falling edge interrupt
    P4IFG &= 0b00000000; //clear interrupt flag
    NVIC -> ISER[1] |= 0x00000040;
}

// triggered on touch, falling edge
void PORT4_IRQHandler(void){
    uint16_t status = P4IV;
    volatile uint16_t count;


    if (status == 0x02){ //P4.0 BMP0
        count = count + Bump0;
        wasRight = true;
    }
    else if (status == 0x06){ //P4.2 BMP1
        count = count + Bump1;
        wasRight = true;
    }
    else if (status == 0x08){ //P4.3 BMP2
        count = count + Bump2;
        wasRight = true;
    }
    else if (status == 0x0C){ //P4.5 BMP3
        count = count - Bump3;
        wasRight = false;
    }
    else if (status == 0x0E){ //P4.6 BMP4
        count = count - Bump4;
        wasRight = false;
    }
    else if (status == 0x10){ //P4.7 BMP5
        count = count - Bump5;
        wasRight = false;
    }
    wasInterrupt = true;
    P4IFG &= 0x00; //clear interrupt flag
}
