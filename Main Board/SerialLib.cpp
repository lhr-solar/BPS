#include "SerialLib.h"
#include <string>

DigitalOut led1(LED1);
DigitalOut led2(LED2);

SerialInterruptHandler::SerialInterruptHandler(PinName tx, PinName rx) : serialConn(tx, rx)
{
    mailBoxGetIndex = 0;
    mailBoxPutIndex = 0;
    serialConn.baud(9600);
    serialConn.format(8, Serial::None, 1);
    serialConn.attach(callback(this, &SerialInterruptHandler::interruptRoutine), Serial::RxIrq);
    endMessageFlag = false;
}

void SerialInterruptHandler::interruptRoutine()
{
    led2 = 0;
    if(!isMailBoxFull())
    {
        string newMessage = ""; 
        serialConn.scanf("%s", newMessage.c_str());
//        serialConn.fgets(newMessage, 50);
        /*
        while(nextChar != '\n')
        {
                    led1 = 1;
                    nextChar = serialCon.getc();
                    //serialCon.printf("%c\n\r", nextChar);
                    
                    if (nextChar == '\r' && endMessageFlag){
                        break;
                    }
                    
                    if (nextChar != '\n') 
                    {
                        newMessage[i] = nextChar;
                    }else
                    {
                        endMessageFlag = true;
                    }
                    i++;
                    //serialCon.printf("%s\n", newMessage);
        }*/
        led2 = 1;
        mailBox[mailBoxPutIndex] = newMessage;
        mailBoxPutIndex = (mailBoxPutIndex + 1) % MAIL_BOX_SIZE;   
        lastMessage = newMessage;
    }
    led1 = 0;
}


string SerialInterruptHandler::getNextMessage()
{
    string newMessage = lastMessage;
    
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


void SerialInterruptHandler::sendMessage(string message)
{
    serialConn.printf("%s\n", message.c_str());
}

void SerialInterruptHandler::setMode(int mode)
{
    this->mode = mode;
}

