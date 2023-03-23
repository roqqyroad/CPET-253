//------------See AP.c for details of hardware connections to CC2650--------------------
//------------See LaunchPad.c for details of switches and LEDs--------------------------

#include <stdint.h>
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/AP.h"
#include "../inc/UART0.h"
#include "../inc/UART1.h"
#include "../inc/SSD1306.h"
#include "../inc/Motor.h"
#include"..\inc\Init_Ports.h"
#include "msp.h"


uint8_t  BT_ByteData;      // 8-bit user data from the phone


// ********OutValue**********
// Debugging dump of a data value to virtual serial port to PC
// data shown as 1 to 8 hexadecimal characters
// Inputs:  response (number returned by last AP call)
// Outputs: none
void ValueOut(char *label,uint32_t value){
  UART0_OutString(label);
  UART0_OutUHex(value);
}

void MoveRobot (uint8_t command) {
// this function calls the appropriate functions to stop, move forward, move backward, turn right,
// or turn left according to the command received from the BLE
    if(command == 1){

        Motor_Forward(12750, 13055);
        SSD1306_Clear();
        SSD1306_SetCursor(0,0);
        SSD1306_OutString("Going Forward");
        SSD1306_OutChar(CR);
    }

    if(command == 2){


        Motor_Backward(12750, 13055);
        SSD1306_Clear();
        SSD1306_SetCursor(0,0);
        SSD1306_OutString("Going Backwards");
        SSD1306_OutChar(CR);
    }

    if(command == 3){
        Motor_Right(12050, 0);
        SSD1306_Clear();
        SSD1306_SetCursor(0,0);
        SSD1306_OutString("Turning Right");
        SSD1306_OutChar(CR);
    }

    if(command == 4){
        Motor_Left(0, 12050);
        SSD1306_Clear();
        SSD1306_SetCursor(0,0);
        SSD1306_OutString("Turning Left");
        SSD1306_OutChar(CR);
    }

    if(command == 5){
        Motor_Stop();
        SSD1306_Clear();
        SSD1306_SetCursor(0,0);
        SSD1306_OutString("Stopped");
        SSD1306_OutChar(CR);

    }

}

void WriteByteData(void){ // called on a SNP Characteristic Write Indication on characteristic ByteData
  MoveRobot(BT_ByteData);   // send command to robot
  ValueOut("\n\rWrite BLE_ByteData=",BT_ByteData);
}

void Timer_Init(void)
{
//**********First initialize TimerA0 for PWM
//Since the motors are connected to P2.6 and P2.7, use TimerA0, compare blocks 3 & 4****
//stop the timer
    TA0CTL &= ~0x0030;
//choose SMCLK for the clock source
    TA0CTL |= 0x0200;
    TA0CTL &= ~0x0100;
//choose clock divider of 2
    TA0CTL |= 0x0040; //0b0000 0010 0100 0000
//Outmode 7: reset/set
    //TA0CCTL1 |= 0x00E0;
    TA0CCTL3 = 0x00E0;
    TA0CCTL4 = 0x00E0;

    TA0CCR0 = 59999;

}

int main(void){
  volatile int r;

  DisableInterrupts();
  Clock_Init48MHz();
  UART0_Init();
  Timer_Init();
  Port2_Init();
  Port3_Init();
  Port5_Init();
  SSD1306_Init(SSD1306_SWITCHCAPVCC);
  EnableInterrupts();
  UART0_OutString("\n\rApplication Processor - MSP432-CC2650\n\r");
  r = AP_Init();
  AP_GetStatus();  // optional
  AP_GetVersion(); // optional
  AP_AddService(0xFFF0);
  //------------------------
  BT_ByteData = 0;  // write parameter from the phone will be used to control direction
  AP_AddCharacteristic(0xFFF1,1,&BT_ByteData,0x02,0x08,"DirectionData",0,&WriteByteData);

  //------------------------

  AP_RegisterService();
  AP_StartAdvertisementJacki();
  AP_GetStatus(); // optional
  while(1){
    AP_BackgroundProcess();  // handle incoming SNP frames

  }
}
