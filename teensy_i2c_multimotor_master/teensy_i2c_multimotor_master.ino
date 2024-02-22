#include <Wire.h>

int DEBUG = 0;

byte SLAVE_ADDRESS = 1;
const byte dataCount = 4;
#define N_FLOATS 4
volatile byte* arrayPointer;
union myData_UNION{
 float floatData[dataCount];
 byte rawData[dataCount*sizeof(float)];
};
myData_UNION myData[10];
myData_UNION * pMyData[10];
volatile float array[N_FLOATS] = {11, 22, 33, 44};
volatile boolean sendStuff;

void setup() {
   Wire.begin();
   Serial.begin(9600);
}

// byte x = 0;
unsigned long previousMillis = 0;
void output(int address)
{
  if(DEBUG)
  {
    Serial.print("master get: ");
    for (int i = 0; i < dataCount; i++)  {
      Serial.print(myData[address].floatData[i],2);
      Serial.print(" ");
    }
    Serial.println();
  }
}
void loop() 
{
  unsigned long currentMillis = millis();
  if(previousMillis == 0)
  {
    previousMillis = currentMillis;
  }

  if(DEBUG)
  {
    Serial.print("time diff: ");
    Serial.println( currentMillis - previousMillis);
    Serial.print("currentMillis: ");
    Serial.println( currentMillis);
  }

  previousMillis = currentMillis;
  
  writeI2C(1);
  readI2C(1);
  output(1);

  writeI2C(2);
  readI2C(2);
  output(2);

  writeI2C(3);
  readI2C(3);
  output(3);

  writeI2C(4);
  readI2C(4);
  output(4);
}
void readI2C(int address)
{ 
  if (Wire.requestFrom (address, sizeof myData[0]) == sizeof myData[0])
  {
    Wire.readBytes( (byte *) &myData[address], sizeof(myData[0]));
  }
}
void writeI2C(int address) 
{
  arrayPointer = (byte*) &array;
  byte buffer[4*N_FLOATS];
  for(byte i = 0; i < 4*N_FLOATS; i++) 
    buffer[i] = arrayPointer[i];
  
  Wire.beginTransmission(address); // transmit to device #4
  Wire.write(buffer,4*N_FLOATS);
  Wire.endTransmission();
  if(DEBUG)
  {
    Serial.print("master sent: ");
    for (int i = 0; i < dataCount; i++)  
    {
      Serial.print(array[i],2);
      Serial.print(" ");
    }
    Serial.println();
  }
}