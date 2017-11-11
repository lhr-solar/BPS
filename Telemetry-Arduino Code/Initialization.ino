boolean Initialization(){//returns a '1' if successful
/*--------------------------------------------------------------------------------------------*/
//ESP Check  
  ESP8266.print("AT\r\n");// Send just 'AT' to make sure the ESP is responding
  if(Available(OK,sizeof(OK),5000,0))
    Serial.println("ESP IS RESPONSIVE");
  else
    Serial.println("ESP DOESN'T RESPOND");
  dump();
/*--------------------------------------------------------------------------------------------*/
//ESP Reset ****Only for debugging***
//   ESP8266.print("AT+RST\r\n");// Give it a reset
//  if(Available(OK,sizeof(OK),5000,0))
//    Serial.println("ESP RESET OK");
//   else
//    Serial.println("ESP DID NOT RESET"); 
//   dump();
/*--------------------------------------------------------------------------------------------*/
//CWMODE Setup
   ESP8266.print("AT+CWMODE=");// set the CWMODE
   ESP8266.print(CWMODE);
   ESP8266.print("\r\n");
  if(Available(OK,sizeof(OK),1000,0))
    Serial.println("ESP CWMODE SET");
  else
    Serial.println("ESP CWMODE DID NOT SET");
  dump();  
/*--------------------------------------------------------------------------------------------*/ 
//Wireless Connection
   ESP8266.print("AT+CWJAP=\"");// AT+CWJAP="SSID","Password"
   ESP8266.print(SSID_USERNAME);
   ESP8266.print("\",\"");
   ESP8266.print(SSID_PASSWORD);
   ESP8266.print("\"\r\n");
  if(Available(OK,sizeof(OK),10000,0))
    Serial.println("ESP CONNECTED TO WIFI");
  else
    Serial.println("ESP FAILED TO CONNECT");   
  dump();
/*--------------------------------------------------------------------------------------------*/
//Retrieve IP address
  Serial.println("CHECKING FOR AN IP ADDRESS");
  ESP8266.print("AT+CIFSR\r\n");
  while(!ESP8266.available());
  Serial.print(ESP8266.read());
  if(Available(Enter,sizeof(Enter),10000,0)){
  if(Available(Enter,sizeof(Enter),1000,1)){
    for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(Enter)+1); i++)
       ip_address[i] = scratch_data_from_ESP[i];
    ip_address[0] = (scratch_data_from_ESP[0]-sizeof(Enter)+1);
    Serial.print("");
    for(int i=1; i<=ip_address[0]; i++)
    Serial.print(ip_address[i]);
    Serial.println("");
  }}
  else
  Serial.print("IP ADDRESS DOES NOT EXIST");
  dump();
/*--------------------------------------------------------------------------------------------*/  
//CIPMUX Setup
   ESP8266.print("AT+CIPMUX=");
   ESP8266.print(CIPMUX);
   ESP8266.print("\r\n");
  if(Available(OK,sizeof(OK),5000,0))
    Serial.println("ESP MULTIPLEX MODE SET");
  else
    Serial.println("ESP CIPMUX SET FAILED"); 
  dump();
}
