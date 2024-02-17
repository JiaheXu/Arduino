#include <Wire.h>
#define SERIAL_BAUD 9600
#define N_FLOATS 4

int DEBUG = 0;

volatile byte* arrayPointer;

byte SLAVE_ADDRESS = 2;
volatile byte lastMasterCommand = 99;
volatile float array[N_FLOATS] = {9.9, 10.10, 11.11, 12.12};
volatile boolean sendStuff;

const byte dataCount = 4;
union myData_UNION{
 float floatData[dataCount];
 byte rawData[dataCount*sizeof(float)];
};
myData_UNION myData;
myData_UNION * pMyData;

void setup() 
{
   Serial.begin(SERIAL_BAUD);
   Wire.begin(SLAVE_ADDRESS);
   Wire.onRequest(requestEvent); 
   Wire.onReceive(receiveEvent);
}

void loop() {}

void requestEvent()
{  
  writeI2C();
}
void receiveEvent(int howMany)
{
  int pos = 0;
  byte buffer[4*N_FLOATS];
  float f[4];
  for(byte i = 0; i < 4*N_FLOATS; i++) 
  {
    byte c = Wire.read(); // receive byte as a character
    buffer[i] = c;
  }
  memcpy(f, buffer, 4*N_FLOATS);

  if(DEBUG)
  {
    Serial.println ("slave recieved: ");
    for(int i=0;i<4;i++)
    {
      Serial.print(f[i],2);
      Serial.print(" ");
    }

    Serial.println();
  }
}

void writeI2C() 
{
  arrayPointer = (byte*) &array;
  byte buffer[4*N_FLOATS];
  for(byte i = 0; i < 4*N_FLOATS; i++) 
    buffer[i] = arrayPointer[i];
  Wire.write(buffer,4*N_FLOATS);
  if(DEBUG)
  {
    Serial.print("slave sent: ");
    for (int i = 0; i < dataCount; i++)  
    {
      Serial.print(array[i],2);
      Serial.print(" ");
    }
    Serial.println();
  }
}