#include "mbed.h"
#include "SerialLib.h"
SerialInterruptHandler handler(SERIAL_TX, SERIAL_RX); // tx, rx
DigitalOut led(LED2);

int main() {
    while(1) 
    {
        if(!handler.isMailBoxEmpty())
        {
            led = !led;
            while(!handler.isMailBoxEmpty())
            {
                handler.sendMessage(handler.getNextMessage());
            }
        }
        wait(5);
    }
}
