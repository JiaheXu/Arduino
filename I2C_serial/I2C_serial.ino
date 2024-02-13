#include <Wire.h>
byte SLAVE_ADDRESS = 1;
const byte dataCount = 4;

union myData_UNION
{
 float floatData[dataCount];
};
myData_UNION myData;
myData_UNION * pMyData;

void setup() 
{
  Wire.begin();
  Serial.begin(9600);
}

void loop() 
{

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }


  readI2C();
  // Serial.println ("hello");
  for (int i = 0; i < dataCount; i++)  
  {
    // Serial.println ("hello");
    Serial.print("float ");
    Serial.println (i);
    Serial.println (" Value: ");
    Serial.println (myData.floatData[i],6);  
  } 
}

void readI2C()
{
 
  if (Wire.requestFrom (SLAVE_ADDRESS, sizeof myData) == sizeof myData)
  {
    Wire.readBytes( (byte *) &myData, sizeof(myData));
  }
  
}