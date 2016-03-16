/*
This firmware does the following:
1) receives SLIP-encoded messages from Max
2) inverts the data
3) sends the data back to Max

Written by Ian Hattwick
Feb 24, 2016
for computermusicdesign.com

*/
const byte END_MESSAGE = 175;
const byte ESC_MESSAGE = 236;
byte messageLength=0;

byte inputData[32];
byte inputDataIndex=0;

byte outputData[32];
byte outputDataIndex=0;

/****************************************************************
Setup and Loop functions
****************************************************************/
void setup() {
  Serial.begin(57600);
}

void loop() {
  
  while(Serial.available()) removeSlipEncoding();

}

/****************************************************************
removeSlipEncoding()

removes slip encoding from incoming serial messages, 
stores the data in an array,
and then triggers a processing function when we are done.
****************************************************************/
void removeSlipEncoding(){
    //store incoming byte
    byte temp = Serial.read();
    
    //check for escape message.
    //if we find one the next byte is stored in our inputData array.
    if( temp == ESC_MESSAGE ) {
      inputData[ inputDataIndex ] = Serial.read();
      inputDataIndex++;
    }
    //check for the end message
    else if (temp == END_MESSAGE ) {
       messageLength=inputDataIndex; //keep track of the amount of incoming data
      processSerialData(); //process data however you want
      slipEncodeData(); //here we will send back the processed data
      inputDataIndex=0; //get ready for the next incoming packet
    }
    //store data in inputData array
    else {
      inputData[ inputDataIndex ] = temp;
      inputDataIndex++;
    }
}

/***************************************************************
processSerialData()
Inverts the incoming data (just for fun)

****************************************************************/

void processSerialData(){
    for (byte i=0;i<messageLength;i++){
    outputData[i] = 255 - inputData[i]; //invert the data
    }
}

/****************************************************************
slipEncodeData()

, and then SLIP encodes it and
sends it out the serial port
****************************************************************/
void slipEncodeData(){
  
  for (byte i=0;i<messageLength;i++){
    
    //check for, and escape, values equal to the end message
    if (outputData[i] == END_MESSAGE){
      Serial.write(ESC_MESSAGE);
      Serial.write(END_MESSAGE);
    } 
    //check for, and escape, values equal to the esc message
    else if (outputData[i] == ESC_MESSAGE){
      Serial.write(ESC_MESSAGE);
      Serial.write(ESC_MESSAGE);
    } 
    //otherwise send out data
    else Serial.write(outputData[i]);
  }
  //when we are doing sending out data array, send an end message
  Serial.write(END_MESSAGE);
}

