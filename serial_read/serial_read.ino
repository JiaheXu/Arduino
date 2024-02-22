const byte numBytes = 90;
byte receivedBytes[numBytes];
byte numReceived = 0;

boolean newData = false;
#define N_FLOATS 22

const byte dataCount = 22;
byte byteCount = 22*4;
union myData_UNION{
 float floatData[dataCount];
 byte rawData[dataCount*sizeof(float)];
};
myData_UNION myData;
myData_UNION * pMyData;

float f[22];
void setup() {
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvBytesWithStartEndMarkers();
    showNewData();
}

void recvBytesWithStartEndMarkers() 
{
    static boolean recvInProgress = false;
    // static boolean recvInProgress = true;
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
                receivedBytes[ndx] = rb;
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
    memcpy(f, receivedBytes, dataCount*4);
}

void showNewData() {
  // Serial.write(receivedBytes, dataCount*4);
  for(int i=0;i<22;i++)
    Serial.print(f[i]);
}