#include "mbed.h"

using namespace std;

DigitalOut led_tx(LED1); // green
DigitalOut led_live(LED2); // blue
DigitalOut led_error(LED3); // red
DigitalIn  btn_ex_1(D7); // btn ext for red
DigitalOut led_ex_1(D6); // red ext
DigitalIn btn_ex_2(D5);  // btn ext for green
DigitalOut led_ex_2(D4); // green ext
DigitalIn btn_ex_3(D3);  // btn ext for yellow
DigitalOut led_ex_3(D2); // yellow ext
DigitalIn  btn_user(USER_BUTTON); // user button
Serial xbee(D1, D0);
//Serial xbee(SERIAL_TX, SERIAL_RX);
int num = 0;
int _tx = 0;
char _tx_str[20] = "";

void set_tx(bool signal){
        _tx = signal;
        if (signal) led_tx = 1;
        else {
            led_tx = 0;
            led_error = 0;
            }
    }
    
bool get_tx(){
        return _tx;
    }

int send_tx(char val[20]){
    if (!get_tx()){
        set_tx(true);
        for(int i = 0; i < 20; i++){
            _tx_str[i] = val[i];
        }
        return 0;
    }
    led_error = 1;
    return 1;
}



// main() runs in its own thread in the OS
int main() {
    xbee.baud(9600);
    led_tx = 0;
    led_live = 1;
    int count_tx=0;
    int count_live=0;
    int btn_test_p=0;
    while (true) {
        set_tx(false);
        if(btn_user && !btn_test_p){
            send_tx("\nHello World!\n");
            set_tx(true);
            btn_test_p = 1;
        }
        else if (btn_ex_1 && !btn_test_p){
            send_tx("RED");
            set_tx(true);
            led_ex_1 = 1;
            btn_test_p = 1;
        }
        else if (btn_ex_2 && !btn_test_p) {
            send_tx("GREEN");
            set_tx(true);
            led_ex_2 = 1;
            btn_test_p = 1;
        }
        else if (btn_ex_3 && !btn_test_p) {
            //xbee.printf("YELLOW");
            send_tx("YELLOW");
            set_tx(true);
            led_ex_3 = 1;
            btn_test_p = 1;
        }
        else if (!btn_user && !btn_ex_1 && !btn_ex_2 && !btn_ex_3)
        {
            btn_test_p = 0;
            //set_tx(false);
            led_ex_1 = 0;
            led_ex_2 = 0;
            led_ex_3 = 0;
        }
        if(count_tx>10){
            // 250000
            num++;
            if(num > 9){
                num = 0;
                }
            if(!get_tx()){
                //xbee.printf("%d", num);
                char tmp[1];
                sprintf(tmp, "%d", num);
                send_tx(tmp);
            }
            count_tx = 0;   
        }
        if(count_live > 20){
            // 5000000
            led_live = !led_live;
            count_live = 0;
        }
        
        count_live++;
        count_tx++;
        if (get_tx()){
            xbee.printf(_tx_str);
        }
        wait_ms(30);
    }
}

