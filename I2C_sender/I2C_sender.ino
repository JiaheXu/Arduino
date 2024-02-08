#include <Wire.h>

volatile byte leArray[6];
volatile boolean sendStuff;
volatile byte someByte1 = 0x2A;
volatile byte someByte2 = 0x2B;
volatile byte someByte3 = 0x2C;
volatile byte someByte4 = 0x1A;
volatile byte someByte5 = 0x1B;
volatile byte someByte6 = 0x1C;

void setup()
{
  Wire.begin(100);                // join i2c bus with address #2
  Wire.onRequest(requestEvent); // register event
}

void loop()
{
  fillArray();
  sendStuff = true;
}

void fillArray()
{
  leArray[0] = someByte1;
  leArray[1] = someByte2;
  leArray[2] = someByte3;
  leArray[3] = someByte4;
  leArray[4] = someByte5;
  leArray[5] = someByte6;
}
void requestEvent()
{  
  if(sendStuff)
  {
    Wire.write((byte*)leArray, 6);
  }
  sendStuff= false;
}
