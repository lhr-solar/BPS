#ifndef SERIALLIB_H
#define SERIALLIB_H


#include "mbed.h"
#include<string>

#define MAIL_BOX_SIZE 50
class SerialInterruptHandler
{
    
    public:
        SerialInterruptHandler(PinName tx, PinName rx);
        void sendMessage(string message);
        string getNextMessage();
        bool isMailBoxEmpty();
        bool isMailBoxFull();
        
        void setMode(int mode);
    
    private:
        void interruptRoutine();
        
        string mailBox[MAIL_BOX_SIZE + 1];
        
//        char *mailBox[MAIL_BOX_SIZE + 1];
        int mailBoxGetIndex;
        int mailBoxPutIndex;
        bool endMessageFlag;
        int mode; 
        string lastMessage;
        Serial serialConn;
        //  0 : Ignore all messages
        //  1 : Accept all messages

};


#endif
