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
myData_UNION myData;
myData_UNION * pMyData;
volatile float array[N_FLOATS] = {11, 22, 33, 44};
volatile boolean sendStuff;

void setup() {
   Wire.begin();
   Serial.begin(9600);
}

// byte x = 0;
unsigned long previousMillis = 0;
void loop() 
{
  unsigned long currentMillis = millis();
  if(previousMillis == 0)
  {
    previousMillis = currentMillis;
  }
  Serial.print("time diff: ");
  Serial.println( currentMillis - previousMillis);
  previousMillis = currentMillis;
  
  writeI2C();
  readI2C();
  Serial.print("master get: ");
  for (int i = 0; i < dataCount; i++)  {
    Serial.print(myData.floatData[i],2);
    Serial.print(" ");
  }
  Serial.println();
}
void readI2C(){ 
  if (Wire.requestFrom (SLAVE_ADDRESS, sizeof myData) == sizeof myData){
    Wire.readBytes( (byte *) &myData, sizeof(myData));
  }
}
void writeI2C() 
{
  arrayPointer = (byte*) &array;
  byte buffer[4*N_FLOATS];
  for(byte i = 0; i < 4*N_FLOATS; i++) 
    buffer[i] = arrayPointer[i];
  
  Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device #4
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