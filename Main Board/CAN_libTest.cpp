#include "mbed.h"
#include "CAN.h"
#include "CAN_lib.h"

DigitalOut led4(LED4);

//Serial pcConnection(SERIAL_TX, SERIAL_RX);

CAN_Interrupt_Handler can1(PD_0, PD_1, 3, 100000);     // Address 0x21, priority 3
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

void ledBlink(){
    led4 != led4;
}
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

void setup()
{
    //canBus.frequency(100000);
    //canBus.attach(&ledBlink, CAN::RxIrq);
    //pcConnection.baud(9600);
    //pcConnection.format(8, Serial::None, 1);
    //sendButton.rise(&sendTestMessage);
    //testCANbus.frequency(100000);
    //canBus.frequency(100000);
    //anBus.attach(&canInterupptRoutine, CAN::RxIrq);
}

int main() 
{
    //setup();    
    while(1) 
    {
        ;
    }
}

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
