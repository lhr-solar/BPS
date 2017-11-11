
#ifndef ESP8266lib_h
#define ESP8266lib_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP8266
{
  
  public:
  ESP8266(int rxPin = 2, int txPin = 3, long int baudRate = 9600);

  void serialDump();  //clears out the serial buffer from the ESP
  boolean resetESP(); //resets just like a computer

  String sendCommand(String command);   
  String getResponseFromESP(int delayTime = 0);
  boolean checkIfMessageSentCorrectly(String response); 

  String getFirmwareVersion();
  String getBaudRate();
  
  String scanAccessPoints();
  boolean resetDefaultAccessPoint();    //resets the memory of the ESP so it won't try and autoconnect to the last connected point
  
  boolean changeESPWifiMode(int newMode); //changes receive/transmit mode, search online for mode types
  String getESPWifiMode();
  boolean changeESPConnectionMode(int newMode); //changes how many connections the ESP can handle, 0 = single, 1 = multi
  boolean setServerMode(int serverMode, int port);  //set whether the ESP is running a local server or not, look up table for modes
  
  boolean connectToNetwork(String networkName, String password);
  String getIPaddress();
  
  boolean sendDataPacket(String server, String dataToSend, int port= 80); //TCP
  boolean sendDataPacketUDP(String server, String dataToSend, int port= 80);  

  private:
  SoftwareSerial *serialLine; 
  
};


#endif
