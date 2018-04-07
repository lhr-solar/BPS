#ifndef SERIALLIB_H
#define SERIALLIB_H


#include "mbed.h"
#include<string>

#define MAIL_BOX_SIZE 5
class SerialInterruptHandler
{
    
    public:
        SerialInterruptHandler(PinName tx, PinName rx);
        void sendMessage(char *message);
        char *getNextMessage();
        bool isMailBoxEmpty();
        bool isMailBoxFull();
        
        void setMode(int mode);
    
    private:
        void interruptRoutine();
        
        char *mailBox[MAIL_BOX_SIZE + 1];
        int mailBoxGetIndex;
        int mailBoxPutIndex;
        int mode; 
        char *lastMessage;
        Serial serialCon;
        //  0 : Ignore all messages
        //  1 : Accept all messages

};


#endif
