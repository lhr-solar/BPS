void Data_Packet(){
/*--------------------------------------------------------------------------------------------*/
//Packet Preparation
HTTP_Packet = URL;//pull in the base URL
HTTP_Packet += String(unit_id);//unit id value
HTTP_Packet += "&sensor=";//unit id 1
HTTP_Packet += String(sensor_value);//sensor value
HTTP_Packet += "&sensor1=";
HTTP_Packet += String(sensor1_value);//sensor1 value
HTTP_Packet += Packet_Ending;
//Serial.print(HTTP_Packet); //prints out full packet, use to debug
/*--------------------------------------------------------------------------------------------*/
  counter=0;//keeps track of the Packet size
  Packet_size=0;
  for(int i=0; i<(HTTP_Packet.length()); i++){
    Packet[Packet_size+i] = HTTP_Packet[i];
    counter++;
  }//for int
  Packet_size = counter+Packet_size;//  
  if(connection()){ 
    if(Available(Time,sizeof(Time),5000,0)){
        if(Available(Data2,sizeof(Data2),5000,0)){
            if(Available(Unlink,sizeof(Unlink),5000,0)){
              dump();
                }
            }  
      }
  }
}
/*--------------------------------------------------------------------------------------------*/
