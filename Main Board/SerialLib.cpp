#include "SerialLib.h"
#include <string>

DigitalOut debugLed(LED1);

SerialInterruptHandler::SerialInterruptHandler(PinName tx, PinName rx) : serialCon(tx, rx)
{
    mailBoxGetIndex = 0;
    mailBoxPutIndex = 0;
    serialCon.baud(9600);
    serialCon.format(8, Serial::None, 1);
    serialCon.attach(callback(this, &SerialInterruptHandler::interruptRoutine), Serial::RxIrq);
}

void SerialInterruptHandler::interruptRoutine()
{
    debugLed = 1;
    if(!isMailBoxFull())
    {
        char newMessage[50] = "";
        char nextChar = 'a';
        for(int i = 0; nextChar != '\n' && i < 50; i++)    
        {
                    nextChar = serialCon.getc();
                    if (nextChar != '\n') 
                    {
                        newMessage[i] = nextChar;
                    }else
                    {
                        break;
                    }
        }
        mailBox[mailBoxPutIndex] = newMessage;
        mailBoxPutIndex = (mailBoxPutIndex + 1) % MAIL_BOX_SIZE;   
        lastMessage = newMessage;
    }
    debugLed = 0;
}


char *SerialInterruptHandler::getNextMessage()
{
    char *newMessage = lastMessage;
    
    if(!isMailBoxEmpty())
    {
        newMessage = mailBox[mailBoxGetIndex];
        mailBoxGetIndex = (mailBoxGetIndex + 1) % MAIL_BOX_SIZE;
    }
    return newMessage;
}

bool SerialInterruptHandler::isMailBoxEmpty()
{
    if(mailBoxPutIndex == mailBoxGetIndex) {return true;}
    else{return false;}
}

bool SerialInterruptHandler::isMailBoxFull()
{
    if((((mailBoxPutIndex + 1) % MAIL_BOX_SIZE)) == mailBoxGetIndex) {return true;}
    else{return false;}
}


void SerialInterruptHandler::sendMessage(char *message)
{
    serialCon.printf("%s\n", message);
}

void SerialInterruptHandler::setMode(int mode)
{
    this->mode = mode;
}

