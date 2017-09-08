boolean connection(){//returns 1 if successful or 0 if not
/*--------------------------------------------------------------------------------------------*/
//Success
  Serial.println("CONNECTING");
  ESP8266.print("AT+CIPSTART=0,\"TCP\",\"192.168.173.1\",8012\r\n");//connect to the web server
  if(Available(OK,sizeof(OK),5000,0)){//go look for 'OK'
  dump();//get rid of extra data
  Serial.println("CONNECTED");// connection made
  ESP8266.print("AT+CIPSEND=0,");//send AT+CIPSEND=0, size of Packet
  ESP8266.print(Packet_size);//the Packet size
  dump();
  ESP8266.print("\r\n");
  
  if(Available(Chevron,sizeof(Chevron),5000,0)){//go wait for the '>' character, ESP ready for the Packet
    Serial.println("PREPARING TO SEND");
    for(int i=0; i<Packet_size; i++)//print the Packet to the ESP
    ESP8266.print(Packet[i]);
    
    if(Available(SendOK,sizeof(SendOK),5000,0)){//go wait for 'SEND OK'
    Serial.println("SENT");
    return 1;//get out of here, data is about to fly out of the ESP
  }
/*--------------------------------------------------------------------------------------------*/
//FAIL
    else
  Serial.println("FAILED TO SEND");
  }//got the back chevron >
  else
  Serial.println("FAILED TO GET >");  
  }//First OK
  else{
  Serial.println("FAILED TO CONNECT");//DID NOT CONNECT
  Initialization();
  }  
}
