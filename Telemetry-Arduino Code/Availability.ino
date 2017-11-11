//Keywords received from the ESP
boolean Available(const char keyword1[], int key_size, int timeout_val, byte mode){
  timeout_start_val=millis();//for the timeout
  char data_in[20];//this is the buffer - if keyword is longer than 20, then increase this
  int scratch_length=1;//the length of the scratch data array
  key_size--;//we're going to get an extra character from the sizeof
/*--------------------------------------------------------------------------------------------*/ 
 //BUFFER FILLER
 for(byte i=0; i<key_size; i++){//buffer = keyword.length() 
            while(!ESP8266.available()){//wait until a new byte is sent down from the ESP
              if((millis()-timeout_start_val)>timeout_val){//if nothing happened, get out
                Serial.println("Hardware Reset is needed");
                return 0;
              }
            }   
    data_in[i]=ESP8266.read();// save the byte to the buffer 'data_in[]
  }
/*--------------------------------------------------------------------------------------------*/  
//THE BUFFER IS FULL, NEW DATA IN/OLD DATA OUT
  while(1){//stay in here until the keyword found or a timeout occurs
     for(byte i=0; i<key_size; i++){
       if(keyword1[i]!=data_in[i])//if it doesn't match, break out of the search now
       break;
       if(i==(key_size-1)){
       return 1; 
       }
     }
/*--------------------------------------------------------------------------------------------*/
//Buffer
    for(byte i=0; i<(key_size-1); i++){// keysize-1 because everthing is shifted over
      data_in[i]=data_in[i+1];// so the data at 0 becomes the data at 1
  }
 
/*--------------------------------------------------------------------------------------------*/         
//Time control
            while(!ESP8266.available()){
              if((millis()-timeout_start_val)>timeout_val){
               Serial.println("timeout");
                return 0;
              }
            }
/*--------------------------------------------------------------------------------------------*/            
    data_in[key_size-1]=ESP8266.read();//save the new data in the last position in the buffer
      if(mode==1){
      scratch_data_from_ESP[scratch_length]=data_in[key_size-1];
      scratch_data_from_ESP[0]=scratch_length;
      scratch_length++;
    }    
  }
}
