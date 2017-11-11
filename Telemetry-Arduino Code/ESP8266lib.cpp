#include "ESP8266lib.h"
#include <SoftwareSerial.h>

//
//look in .h file for more descriptions of ESP8266 methods
//

ESP8266::ESP8266(int rxPin = 2, int txPin = 3, long int baudRate = 9600)  
{
  SoftwareSerial newSerialLine(rxPin, txPin);
  newSerialLine.begin(baudRate);
  this->serialLine = &newSerialLine;
}



String ESP8266::getResponseFromESP(int delayTime = 0)   //just keeps reading characters into a string until nothing is left
{
  String response;
  while(serialLine->available())
  {
    response += serialLine->read();
    delay(delayTime);               //optional delay if the esp is running slow
  }
  return response;
}

void ESP8266::serialDump()    //clears out the serial data buffer
{
 getResponseFromESP(.2); 
}




String ESP8266::sendCommand(String command)
{
  command = "AT+" + command;
  serialLine->println(command);
  return getResponseFromESP();
}

boolean ESP8266::checkIfMessageSentCorrectly(String response)   //all responses indiciating success contain OK somewhere in them, we have failed if we didn't find it
{
  return response.indexOf("OK") != -1; 
}





String ESP8266::getFirmwareVersion()
{
  String response = sendCommand("GMR");
  return response;
}




boolean ESP8266::resetESP()
{
  String response = sendCommand("RST");
  return checkIfMessageSentCorrectly(response);
}




boolean ESP8266::changeESPWifiMode(int newMode)
{
  String command = "CWMODE=" + String(newMode);
  String response = sendCommand(command);
  return checkIfMessageSentCorrectly(response);
}

String ESP8266::getESPWifiMode()
{
 String response = sendCommand("CWMODE?");
 return response; 
}

boolean ESP8266::changeESPConnectionMode(int newMode)
{
  String response = sendCommand("CIPMUX=" + String(newMode));
  return checkIfMessageSentCorrectly(response);
}

boolean ESP8266::setServerMode(int serverMode, int port)
{
  String response = sendCommand("CIPSERVER=" + String(serverMode) + "," + String(port));
  return checkIfMessageSentCorrectly(response);
}




String ESP8266::getBaudRate()
{
  String response = sendCommand("CIOBAUD");
  return response;
}




String ESP8266::scanAccessPoints()
{
  String response = sendCommand("CWLAP");
  return response;
}

boolean ESP8266::resetDefaultAccessPoint()
{
  String response = sendCommand("CWQAP");
  return checkIfMessageSentCorrectly(response);
}

boolean ESP8266::connectToNetwork(String networkName, String password)
{
  String command = "CWJAP=\"" + networkName + "\",\"" + password + "\"";    //format of this command looks like: CWJAP="networkname","password"
  String response = sendCommand(command);
  return checkIfMessageSentCorrectly(response);
}

String ESP8266::getIPaddress()
{
  String ipAddr = sendCommand("CIFSR"); 
  return ipAddr;
}

boolean ESP8266::sendDataPacket(String server, String dataToSend, int port= 80)
{
  sendCommand("CIPSTART=\"TCP\",\"" + server + "\",\"" + String(port) + "\""); //format of this command looks like: CIPSTART="TCP", "server name", "port"
  sendCommand("CIPSEND=" + String(dataToSend.length()));
  serialLine->println(dataToSend);
  String response = sendCommand("CIPCLOSE");
  return checkIfMessageSentCorrectly(response);
}

boolean ESP8266::sendDataPacketUDP(String server, String dataToSend, int port= 80)
{
  sendCommand("CIPSTART=\"UDP\",\"" + server + "\",\"80\"");  //format of this command looks like: CIPSTART= "UDP", "server name", "port"
  sendCommand("CIPSEND=" + String(dataToSend.length()));
  serialLine->println(dataToSend);
  String response = sendCommand("CIPCLOSE");
  return checkIfMessageSentCorrectly(response);
}
