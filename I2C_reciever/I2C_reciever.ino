#include <Wire.h>

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  Wire.requestFrom(100, 6);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    volatile byte c = Wire.read(); // receive a byte as character
    Serial.println(c, HEX);         // print the character
    Serial.println();
    
  }
  delay(2000);
}