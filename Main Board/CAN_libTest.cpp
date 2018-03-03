#include "mbed.h"
#include "CAN.h"
#include "CAN_lib.h"

DigitalOut led3(LED3);
//DigitalOut led2(PC_8);
//DigitalOut led1(LED1);

Serial pcConnection(SERIAL_TX, SERIAL_RX);

CAN_Interrupt_Handler can1(PD_0, PD_1, 3);     // Address 0x21, priority 3
//CAN_Interrupt_Handler can2(PA_11, PA_12, 100000);
//CAN canBus(PD_0, PD_1);   // for F429ZI
CANMessage canMailBox;
CANMessage receiverMailBox;

//DigitalOut messageSignalLED(LED1);
//InterruptIn sendButton(BUTTON1);
//CAN testCANbus(PA_11, PA_12);       // Pins for Nucleo-F072RB

// transmitter prototypes
int CANaddress = 1337;
void sendTestMessage(); 
void setup();
void canInterupptRoutine();
/*
// for transmitter
void sendTestMessage()
{
    //messageSignalLED = 1;
    wait(3);
    char data[] = "Hello";
    CANMessage testMessage(CANaddress, data, sizeof(data));
    testCANbus.write(testMessage);
    //messageSignalLED = 0;   
}*/

//******************************************************************************
// TEST
//******************************************************************************
void setup()
{
    pcConnection.baud(9600);
    pcConnection.format(8, Serial::None, 1);
    //sendButton.rise(&sendTestMessage);
    //testCANbus.frequency(100000);
    //canBus.frequency(100000);
    //anBus.attach(&canInterupptRoutine, CAN::RxIrq);
}

//------------------------------------------------------------------------------
// Filtering Test Code (Change to main to test)
int main(){
    setup();
    //can1.setMode(0);
    //can1.setMode(1);
    can1.setMode(2, 0x5);
    while(1){
        if(!can1.isMailBoxEmpty()){
            can1.getNextMessage(receiverMailBox);
            pcConnection.printf("%s\n\r", receiverMailBox.data);
            led3 = !led3;
        }
    }
}

//------------------------------------------------------------------------------
// Priority Test Code (Change to main to test)
/*
//Ticker tick;    // Uncomment this line if testing
//InterruptIn in(BUTTON1);
void long_event(){
    //led3 = !led3;
    led3 = !led3;
    wait_ms(950);
    led3 = !led3;
}

void short_event(){
    led2 = !led2;
    //wait(.5);
}

int mainPriorityTest(){
    setup();
    can1.setPriority(0);
    tick.attach(&long_event, 1);
    //in.rise(&long_event);
    NVIC_SetPriority(TIM3_IRQn, 4);
    while(1){
        if(!can1.isMailBoxEmpty()){
            can1.getNextMessage(receiverMailBox);
            //pcConnection.printf("%s\n\r", receiverMailBox.data);
            led2 = !led2;
        }
    }    
    return 0;
}
*/
/*
//------------------------------------------------------------------------------
// FIFO Test Code (Change to main to test)
int mainFIFOTest(){
    setup();
    //wait(20);
    while(1){
        //led3 = 1;
        led2 = 1;
        if(can1.isMailBoxFull()){
            break;
        }
        
        //pcConnection.printf("Working");
    }
    led2 = 0;
    while(!can1.isMailBoxEmpty()){
        led3 = 1;
        can1.getNextMessage(receiverMailBox);
        pcConnection.printf("%s\n\r", receiverMailBox.data);
        //led3 = !led3;
    }
    return 0;
}
*/
//******************************************************************************
//******************************************************************************

/*
void canInterupptRoutine()
{
    canBus.read(canMailBox);
    if(canMailBox.data[0] == '1')
    {
        led3 = 1;
    }
    else
    {
        led3 = 0;
    }
}

void receiveMessage()
{
 canBus.read(receiverMailBox);
 if(receiverMailBox.data[0] == '1')
 {
    myLed = 1;
 }
 else
 {
    myLed = 0;
 }
}
*/
