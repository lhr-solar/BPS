#include <avr/power.h>
//#include <SoftwareSerial.h> //did not work with Arduino Mega, but this is for changing TX RX in software (same as AltSoftSerial)
#include <AltSoftSerial.h> //Same as SoftwareSerial, but it has fixed RX and TX for each Arduino
// AltSoftSerial always uses these pins:
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Teensy 3.0 & 3.1  21        20         22
// Teensy 2.0         9        10       (none)
// Teensy++ 2.0      25         4       26, 27
// Arduino Uno        9         8         10
// Arduino Leonardo   5        13       (none)
// Arduino Mega      46        48       44, 45
// Wiring-S           5         6          4
// Sanguino          13        14         12
/*--------------------------------------------------------------------------------------------*/
//DEFINE VALUES HERE
#define ESP8266_RX 46 //RX pin( unchangable while using <AltSoftSerial.h> )
#define ESP8266_TX 48 //TX pin( unchangable while using <AltSoftSerial.h> )
AltSoftSerial ESP8266; //DEFINE SERIAL SOFTWARE FOR ESP8266 AS ESP8266

/*--------------------------------------------------------------------------------------------*/
//wifi information//
const char SSID_USERNAME[] = "armtin"; 
const char SSID_PASSWORD[] = "arminartin1376";
/*--------------------------------------------------------------------------------------------*/
// URL
String URL = "GET http://192.168.173.1:8012/Microcontroller.php?unit="; //WEBHOST/PHP FILE FOR EACH MICROCONTROLLER
/*--------------------------------------------------------------------------------------------*/
//MODES
const char CWMODE = '1';//CWMODE 1=STATION, 2=APMODE, 3=BOTH
const char CIPMUX = '1';//CWMODE 0=Single Connection, 1=Multiple Connections
/*--------------------------------------------------------------------------------------------*/
//FUNCTIONS
boolean Initialization();
boolean Available(const char keyword1[], int key_size, int timeout_val, byte mode);
void timeout_start();
boolean timeout_check(int timeout_ms);
void dump();
boolean connection();
void Data_Packet();
/*--------------------------------------------------------------------------------------------*/
//GLOBAL VAARIABLES
int unit_id = 1;
int sensor_value = 0;
int sensor1_value = 0;
unsigned long timeout_start_val;
char scratch_data_from_ESP[20];//first byte is the length of bytes
char Packet[150];
byte Packet_size=0, counter=0;
char ip_address[16];
String HTTP_Packet = "                                                                                                    ";
String Packet_Ending = " HTTP/1.0\r\n\r\n"; // This ending is for HTTP type
/*--------------------------------------------------------------------------------------------*/
//KEYWORDS
const char OK[] = "OK";
const char Ready[] = "Ready";
const char Enter[] = "\r\n";
const char Chevron[] = ">";
const char SendOK[] = "SEND OK";
const char Unlink[] = "Unlink";
const char Time[] = "t1";
const char Data1[] = "d1";
const char Data2[] = "d2";
/*--------------------------------------------------------------------------------------------*/
//SETUP
void setup(){
  pinMode(ESP8266_RX, INPUT);
  pinMode(ESP8266_TX, OUTPUT);
  ESP8266.begin(9600);//default baudrate for ESP (can change with AT Commands)
  Serial.begin(9600); //for debug in Serial Monitor  
}
/*--------------------------------------------------------------------------------------------*/
void loop(){
/*--------------------------------------------------------------------------------------------*/
//SENSORS
  sensor_value = analogRead(0);
  sensor1_value = analogRead(3);
/*--------------------------------------------------------------------------------------------*/
  Data_Packet();
                //Set delay here if any information was lost 
}      
