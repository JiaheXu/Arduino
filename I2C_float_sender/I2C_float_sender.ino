#include <Wire.h>
#define SERIAL_BAUD 115200
#define N_FLOATS 4

volatile byte* arrayPointer;

byte SLAVE_ADDRESS = 1;
volatile byte lastMasterCommand = 99;
volatile float array[N_FLOATS] = {12.3456, 11.111111, 333.33333, 1};

volatile boolean sendStuff;


void setup() {
   Serial.begin(SERIAL_BAUD);
   Wire.begin(SLAVE_ADDRESS);
   Wire.onRequest(requestEvent); 
}

void loop() 
{
  // fillArray();
  // sendStuff = true;
}

void requestEvent()
{  
  if(sendStuff)
  {
    // Wire.write((byte*)leArray, 6);
  }
  sendStuff= false;
  writeI2C();
}

void writeI2C() {

  arrayPointer = (byte*) &array;
  byte buffer[4*N_FLOATS];
  for(byte i = 0; i < 4*N_FLOATS; i++) 
    buffer[i] = arrayPointer[i];
  Wire.write(buffer,4*N_FLOATS);
}