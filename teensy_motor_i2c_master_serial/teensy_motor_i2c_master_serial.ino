#include <Wire.h>

int DEBUG = 0;
volatile byte* arrayPointer;
int joints[6] = {0, 5, 4, 4, 4, 5};
// int joints[6] = {0, 4, 4, 4, 4, 5};
int start_joint[6] = {0, 0, 5, 9, 13, 17};

#define NUM_JOINTS  22
#define NUM_FINGER  5
const byte numBytes = NUM_JOINTS*4;

union joint5_UNION{
 float floatData[5];
 byte rawData[5*sizeof(float)];
};
union joint4_UNION{
 float floatData[4];
 byte rawData[4*sizeof(float)];
};

joint5_UNION joint5Data[10];
joint5_UNION * pJoint5Data[10];

joint4_UNION joint4Data[10];
joint4_UNION * pJoint4Data[10];

// volatile float state_array[NUM_JOINTS] = {0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16, 17.17, 18.18, 19.19, 20.20, 21.21};
// volatile float state_array[NUM_JOINTS] = {0.0};
// volatile float goal_array[NUM_JOINTS] = {0.0};
volatile boolean sendStuff;

boolean newData = false;

float goal_float_buffer[ NUM_JOINTS];
byte goal_byte_buffer[ NUM_JOINTS*4 ]; // buffer to get goal position

float state_float_buffer[ NUM_JOINTS ];
byte state_byte_buffer[ NUM_JOINTS*4 ]; // buffer to send state position

byte numReceived = 0;
byte jointCount = NUM_JOINTS;
byte byteCount = NUM_JOINTS*4;

void setup() 
{
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
    for (int i = 0; i < joints[address]; i++)  
    {
      Serial.print(state_float_buffer[start_joint[address] + i] , 2 );
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
      
  recvBytesWithStartEndMarkers();
  // for(int i=1;i<=NUM_FINGER;i++)
  // {
  //   writeI2C(2);
  //   readI2C(2);
  //   output(2);    
  // }
  writeI2C(1);
  readI2C(1);
  output(1);
  // writeI2C(2);
  // readI2C(2);
  // output(2);
  // writeI2C(3);
  // readI2C(3);
  // output(3);
  // writeI2C(4);
  // readI2C(4);
  // output(4);
  returnStateData();
}
void readI2C(int address) // get joint data from slave nodes
{ 
  // if(address == 1 or address == 5)
  if( address == 5)
  {
    if (Wire.requestFrom (address, sizeof(joint5Data[0]) ) == sizeof(joint5Data[0]) )
    {
      Wire.readBytes( (byte *) &joint5Data[address], sizeof(joint5Data[0]) );
    }
    for(int i=0; i<joints[address]; i++)
    {
      state_float_buffer[ start_joint[address] + i ] = joint5Data[address].floatData[i];
    }
  }
  else
  {
    if (Wire.requestFrom (address, sizeof(joint4Data[0]) ) == sizeof(joint4Data[0]) )
    {
      Wire.readBytes( (byte *) &joint4Data[address], sizeof(joint4Data[0]) );
    }
    for(int i=0; i<joints[address]; i++)
    {
      state_float_buffer[ start_joint[address] + i ] = joint4Data[address].floatData[i];
    }
  }

}

void writeI2C(int address) // send goal data to slave node
{
  arrayPointer = (byte*) &goal_float_buffer;
  byte buffer[ joints[address]*4 ];

  for(byte i = 0; i < 4*joints[address]; i++) 
    buffer[i] = goal_byte_buffer[ start_joint[address]*4 + i];
  
  Wire.beginTransmission(address); // transmit to device #4
  Wire.write(buffer, joints[address]*4 );
  Wire.endTransmission();
  if(DEBUG)
  {
    memcpy(goal_float_buffer, goal_byte_buffer, NUM_JOINTS*4);
    Serial.print("master sent: ");
    for (int i = 0; i < joints[address]; i++)  
    {
      Serial.print(goal_float_buffer[ start_joint[address] + i], 2);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void recvBytesWithStartEndMarkers() 
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0x3C;
    byte endMarker = 0x3E;
    byte rb;
   
    while (Serial.available() > 0 && newData == false) 
    {
        rb = Serial.read();
        if (recvInProgress == true) 
        {
            if (rb != endMarker) 
            {
                goal_byte_buffer[ndx] = rb;
                ndx++;
                if (ndx >= numBytes) 
                {
                    ndx = numBytes - 1;
                }
            }
            else 
            {
                recvInProgress = false;
                numReceived = ndx;  // save the number for use when printing
                ndx = 0;
                newData = true;
            }
        }
        else if (rb == startMarker) 
        {
            recvInProgress = true;
        }
    }
    
}

void returnStateData() 
{
  if (newData == true) 
  {
    memcpy( state_byte_buffer, state_float_buffer, NUM_JOINTS*4);
    Serial.write(state_byte_buffer, NUM_JOINTS*4);
    newData = false;
  }
}