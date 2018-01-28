#include "mbed.h"
#include "CAN.h"
#include "CAN_lib.h"

DigitalOut heartBeat(LED1);

CAN_Interrupt_Handler can1(PB_8, PB_9, 100000);     // Pins for Nucleo-F072RB
//CAN_Interrupt_Handler can2(PA_11, PA_12, 100000);
//CAN canBus(PD_0, PD_1);   // for F429ZI
//CANMessage canMailBox;

DigitalOut messageSignalLED(LED1);
InterruptIn sendButton(BUTTON1);
CAN testCANbus(PA_11, PA_12);       // Pins for Nucleo-F072RB

int CANaddress = 1337;
void sendTestMessage(); 

void setup()
{
    sendButton.rise(&sendTestMessage);
    testCANbus.frequency(100000);
}

int main() 
{
    setup();
    while(1)
    {
       ;
    }
    
}

void sendTestMessage()
{
    messageSignalLED = 1;
    wait(3);
    char data[] = "Hello";
    CANMessage testMessage(CANaddress, data, sizeof(data));
    testCANbus.write(testMessage);
    messageSignalLED = 0;   
}


/*
void canInterupptRoutine();

void setup()
{
    canBus.frequency(100000);
    canBus.attach(&canInterupptRoutine, CAN::RxIrq);
}

int main() 
{
    setup();    
    while(1) 
    {
        ;
    }
}

void canInterupptRoutine()
{
    canBus.read(canMailBox);
    if(canMailBox.data[0] == '1')
    {
        heartBeat = 1;
    }
    else
    {
        heartBeat = 0;
    }
}*/
