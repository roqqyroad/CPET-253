//------------See AP.c for details of hardware connections to CC2650--------------------
//------------See LaunchPad.c for details of switches and LEDs--------------------------

#include "msp.h"
#include <msp432.h>
#include <stdint.h>
#include <stdbool.h>
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/AP.h"
#include "../inc/UART0.h"
#include "../inc/UART1.h"
#include "../inc/SSD1306.h"
#include "../inc/Motor.h"
#include"../inc/Init_Ports.h"
#include "../inc/BumpInt.h"

volatile uint8_t BT_ByteData;      // 8-bit user data from the phone
extern bool wasInterrupt;

// ********OutValue**********
// Debugging dump of a data value to virtual serial port to PC
// data shown as 1 to 8 hexadecimal characters
// Inputs:  response (number returned by last AP call)
// Outputs: none
void ValueOut(char *label, uint32_t value)
{
    UART0_OutString(label);
    UART0_OutUHex(value);
}

// this function calls the appropriate functions to stop, move forward, move backward, turn right,
// or turn left according to the command received from the BLE
void MoveRobot(uint8_t command)
{

    //(left duty, right duty)
    if (command == 1)
    {

        Motor_Forward(6375, 6527);
        SSD1306_Clear();
        SSD1306_SetCursor(0, 0);
        SSD1306_OutString("Going forward from user input!");
        SSD1306_OutChar(CR);
    }

    if (command == 2)
    {

        Motor_Backward(6375, 6527);
        SSD1306_Clear();
        SSD1306_SetCursor(0, 0);
        SSD1306_OutString("Going Backwards from user input!");
        SSD1306_OutChar(CR);
    }

    if (command == 3)
    {
        Motor_Right(6375, 0);
        SSD1306_Clear();
        SSD1306_SetCursor(0, 0);
        SSD1306_OutString("Turning Right from user input!");
        SSD1306_OutChar(CR);
    }

    if (command == 4)
    {
        Motor_Left(0, 6527);
        SSD1306_Clear();
        SSD1306_SetCursor(0, 0);
        SSD1306_OutString("Turning Left from user input!");
        SSD1306_OutChar(CR);
    }

    if (command == 5)
    {
        Motor_Stop();
        SSD1306_Clear();
        SSD1306_SetCursor(0, 0);
        SSD1306_OutString("Stopped! Waiting for direction");
        SSD1306_OutChar(CR);

    }

}

void WriteByteData(void)
{ // called on a SNP Characteristic Write Indication on characteristic ByteData
    MoveRobot(BT_ByteData);   // send command to robot
    ValueOut("\n\rWrite BLE_ByteData=", BT_ByteData);
}

//SERVO RELATED
void Servo(uint16_t angle);

void ServoInit(void)
{
    Servo(4400);     //call Servo() function to center servo
    Clock_Delay1us(100000); //delay here to give servo time to move - can use built in timer function
    TA3CTL &= ~0x0030;     //stop the timer
    return;
}
void Servo(uint16_t angle_count)
{
    TA3CCR0 = 59999;      //set period for 20ms
    TA3CCR2 = angle_count; //set high time for the input angle using angle_count
    TA3CTL |= 0x0010;      //set timer for up mode
    return;
}
//END OF SERVO RELATED

//TIMER INIT
void Timer_Init(void)
{
    //**********First initialize TimerA0 for PWM
    //Since the motors are connected to P2.6 and P2.7, use TimerA0, compare blocks 3 & 4****
    TA0CCR3 = 14999;
    TA0CCR4 = 14999;

    TA0CTL &= ~0x0030; //stop the timer
    TA0CTL |= 0x0200; //choose SMCLK for the clock source
    TA0CTL &= ~0x0100; //choose SMCLK for the clock source
    TA0CTL |= 0x0040; //choose clock divider of 2 - ID: 01

    //Period of 60k
    TA0CCR0 = 59999;

    //Outmde 7: reset/set
    TA0CCTL3 |= 0x00E0;
    TA0CCTL4 |= 0x00E0;
    //END OF PWM

    //TimerA3 for servo
    TA3CCTL2 |= 0x00E0; // set  reset
    TA3CTL &= ~0x0030; //stop the timer
    TA3CTL |= 0x0200;
    TA3CTL &= ~0x0100; //choose SMCLK for clock source
    TA3CTL |= 0x0080; //clock divider of 4 - ID: 10 on pins 7-6
    TA3CCR2 = 59999; //configure compare register for 20ms delay

}    //END TIMER INIT

int main(void)
{
    volatile int r;
    //states USER for user control, SHAKE for shaking the crab head,
    //and BACKWARD to go backwards from hitting something.
    enum states
    {
        USER, SHAKE, BACKWARD
    } state, prevState;
    state = USER;
    prevState = !USER;
    uint16_t stateTimer;

    //how many times does the head of the crab shake
    volatile uint16_t shakes = 3;
    //how many ms between shake action
    volatile uint16_t shakesDelay = 500;
    //servo middle value
    volatile uint16_t servoMid = 4400;
    //servo right value
    volatile uint16_t servoRight = 2000;
    //servo left value
    volatile uint16_t servoLeft = 7500;
    //has the head been shook sufficiently
    volatile bool sufficientShakes = false;

    //delay for going backwards and stopping
    //before the user can control the crab again
    uint16_t backwardDelay = 1500;

    DisableInterrupts();
    Clock_Init48MHz();
    UART0_Init();
    Timer_Init();
    Port1_Init();
    Port2_Init();
    Port3_Init();
    Port4_Init();
    Port5_Init();
    Port6_Init();
    Port7_Init();
    Port8_Init();
    Port9_Init();
    ServoInit();    //init servos
    BumpInt_Init(); //Initialize interrupts
    Reflectance_Init();

    SSD1306_Init(SSD1306_SWITCHCAPVCC);
    EnableInterrupts();
    UART0_OutString("\n\rApplication Processor - MSP432-CC2650\n\r");
    r = AP_Init();
    AP_GetStatus();  // optional
    AP_GetVersion(); // optional
    AP_AddService(0xFFF0);
    //------------------------
    BT_ByteData = 0; // write parameter from the phone will be used to control direction
    AP_AddCharacteristic(0xFFF1, 1, &BT_ByteData, 0x02, 0x08, "DirectionData", 0, &WriteByteData);

    //------------------------

    AP_RegisterService();
    AP_StartAdvertisementJacki();
    AP_GetStatus(); // optional

    //WHILE
    while (1)
    {
        AP_BackgroundProcess();  // handle incoming SNP frames
        bool isNewState = (state != prevState);
        prevState = state;
        uint16_t status = P4IV;
        uint8_t data = Reflectance_Read(1500); //get data
        int32_t weight = Reflectance_Position(data); //get position

        //SWITCH FOR STATES
        switch (state)
        {

        case USER:
            if (isNewState)
            {
                stateTimer = 0;
                MoveRobot(BT_ByteData);  //move the robot with the user input
                Servo(4400); //move servo to middle
            }
            data = Reflectance_Read(1500);
            weight = Reflectance_Position(data);

            if ((weight > -25) && (weight < 25))
            {
                MoveRobot(BT_ByteData);
            }

            if (weight < -25)
            {
                if (BT_ByteData == 1)  //forward
                {
                    Motor_Forward(2500, 5000);

                }
                if (BT_ByteData == 2)  //backward
                {
                    Motor_Backward(2500, 5000);
                }

                /*
                 if (BT_ByteData = 3)//right mot
                 {

                 }
                 if (BT_ByteData = 4)//left mot
                 {

                 }
                 */

                data = Reflectance_Read(1500);
                weight = Reflectance_Position(data);
                Clock_Delay1ms(200);
            }

            if (weight > 25)
            {
                if (BT_ByteData == 1)  //forward
                {
                    Motor_Forward(4700, 3000);

                }
                if (BT_ByteData == 2)  //backward
                {
                    Motor_Backward(4700, 3000);
                }
                /*
                 if (BT_ByteData = 3)//right mot
                 {

                 }
                 if (BT_ByteData = 4)//left mot
                 {

                 }
                 */

                data = Reflectance_Read(1500);
                weight = Reflectance_Position(data);
                Clock_Delay1ms(200);

            }

            //if it gets interrupted, it hit something so go to shake
            if (wasInterrupt)
            {
                BT_ByteData = 0;  //stop the robot first
                wasInterrupt = false;  //clear flag
                state = SHAKE;  //go to shake

            }
            break;

////////////////////////////////////////////////////////////////////////////////////////////////////

        case SHAKE:
            if (isNewState)
            {
                Motor_Stop();
                sufficientShakes = false;
                stateTimer = 0;
                SSD1306_Clear();
                SSD1306_SetCursor(0, 0);
                SSD1306_OutString("OUCH! I ran into something!");
                SSD1306_OutChar(CR);
            }

            int i = 0;
            while (i < shakes)
            {

                Servo(4400); //move servo to middle
                Clock_Delay1ms(300); //delay
                Servo(2000); //move servo right
                Clock_Delay1ms(300); //delay
                Servo(7500); //move servo left
                Clock_Delay1ms(300); //delay
                i++;
                if (i == shakes)
                {
                    Servo(4400); //move servo to middle
                    sufficientShakes = true;
                }

            }
            //once the head is sufficiently shook, go backwards
            if (sufficientShakes)
            {
                state = BACKWARD;
            }

            break;

////////////////////////////////////////////////////////////////////////////////////////////////////

        case BACKWARD:
            if (isNewState)
            {
                sufficientShakes = false;
                SSD1306_Clear();
                SSD1306_SetCursor(0, 0);
                SSD1306_OutString("Going backwards since I hit something!");
                SSD1306_OutChar(CR);

                //(left duty, right duty)
                //pwm fast forward 12800, 13100 -> pwm slow backward (3500, 5000)
                Motor_Backward(4700, 5000);            //back up slowly a bit
                Clock_Delay1ms(backwardDelay); //for however many ms is specified
                Motor_Stop();            //stop robot
                BT_ByteData = 0;          //ensures that the user starts in stop
                state = USER;            //returns control to user
            }
            break;
            state = USER;            //default state
            Clock_Delay1ms(500);
        }            //end switch

    } //end while

} //end main

