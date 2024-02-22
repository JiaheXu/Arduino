#include <util/atomic.h>
#include <Wire.h>

#define SERIAL_BAUD 9600
#define N_FLOATS 4

////////////////////////////////////////////////////////////////////i2c
int DEBUG = 0;
volatile byte* arrayPointer;
byte SLAVE_ADDRESS = 3;
int times = 1;
volatile byte lastMasterCommand = 99;
// volatile float array[N_FLOATS] = {1.1*int(SLAVE_ADDRESS), 2.2*int(SLAVE_ADDRESS),3.3*int(SLAVE_ADDRESS),4.4*int(SLAVE_ADDRESS)};
volatile float array[N_FLOATS] = {0.0, 0.0, 0.0, 0.0};
volatile boolean sendStuff;

const byte dataCount = 4;
union myData_UNION{
 float floatData[dataCount];
 byte rawData[dataCount*sizeof(float)];
};
myData_UNION myData;
myData_UNION * pMyData;
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////pid
// A class to compute the control signal
class SimplePID{
  private:
    float kp, kd, ki;
    int umin, umax; // Parameters
    float eprev, eintegral; // Storage

  public:
  // Constructor
  SimplePID() : kp(1), kd(0), ki(0), umin(60), umax(255), eprev(0.0), eintegral(0.0){}

  // A function to set the parameters
  void setParams(float kpIn, float kdIn, float kiIn, int uminIn, int umaxIn)
  {
    kp = kpIn; kd = kdIn; ki = kiIn; umin = uminIn; umax = umaxIn;
  }


  // A function to compute the control signal
  void evalu(int value, int target, float deltaT, int &pwr, int &dir){
    // error
    int e = target - value;
  
    // derivative
    float dedt = (e-eprev)/(deltaT);
  
    // integral
    eintegral = eintegral + e*deltaT;
  
    // control signal
    float u = kp*e + kd*dedt + ki*eintegral;
  
    // motor power
    pwr = (int) fabs(u);
    if( pwr > umax ){
      pwr = umax;
    }
  
    if( pwr < umin ){
      pwr = umin;
    }

    // motor direction
    dir = 0;
    if(u<0)
    {
      dir = 1;
    }

    if(e == 0)
      pwr = 0;  

    // store previous error
    eprev = e;
  }
  
};

// How many motors
#define NMOTORS 4
// Pins
// 0 1 2 3
// 4 5 6 7
// 8 9 10 11
// 24 25 28 29
const int encb[] = {0,4,8,24};
const int enca[] = {1,5,9,25};
const int in1[] = {2,6,10,28};
const int pwm[] = {3,7,11,29};

// Globals
long prevT = 0;
volatile int posi[] = {0,0,0,0};
// PID class instances
SimplePID pid[NMOTORS];
int target[NMOTORS];
////////////////////////////////////////////////////////////////////

void setup() 
{
  Serial.begin(9600);
  for(int k = 0; k < NMOTORS; k++)
  {
    pinMode(enca[k],INPUT);
    pinMode(encb[k],INPUT);
    pinMode(pwm[k],OUTPUT);
    pinMode(in1[k],OUTPUT);
    pid[k].setParams(1,0,0,50,255);
    // attachInterrupt(digitalPinToInterrupt(enca[k]),readEncoder<k>,RISING);
  }
  
  attachInterrupt(digitalPinToInterrupt(enca[0]),readEncoder<0>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[1]),readEncoder<1>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[2]),readEncoder<2>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[3]),readEncoder<3>,RISING);  

  //i2c
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);
}


void requestEvent()
{  
  writeI2C();
}
void receiveEvent(int howMany)
{
  readI2C();
}

void readI2C()
{
  byte buffer[4*N_FLOATS];
  float f[4];
  for(byte i = 0; i < 4*N_FLOATS; i++) 
  {
    byte c = Wire.read(); // receive byte as a character
    buffer[i] = c;
  }
  memcpy(f, buffer, 4*N_FLOATS);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    for(int k = 0; k < NMOTORS; k++)
    {
      target[k] = int(f[k])*100; // Todo need to change the scaling
    }
  }
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

void loop() {

  // set target position
  // Todo
  // target[0] = 2500;
  // target[1] = -2500;
  // target[2] = 1500;
  // target[3] = -1500;   

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;

  // Read the position in an atomic block to avoid a potential misread
  int pos[NMOTORS];
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    for(int k = 0; k < NMOTORS; k++)
    {
      pos[k] = posi[k];
      array[k] = pos[k]/10.0;
    }
  }
  
  // loop through the motors
  for(int k = 0; k < NMOTORS; k++)
  {
    int pwr, dir;
    // evaluate the control signal
    pid[k].evalu(pos[k],target[k],deltaT,pwr,dir);
    // signal the motor
    setMotor(dir,pwr,pwm[k],in1[k]);
  }
  
  if(DEBUG)
  {
    for(int k = 0; k < NMOTORS; k++)
    {
      Serial.print(target[k]);
      Serial.print(" ");
      Serial.print(pos[k]);
      Serial.print(" ");
    }
    Serial.println();
  }
  
}

void setMotor(int dir, int pwm_val, int pwm_pin, int in1_pin)
{
  analogWrite(pwm_pin, pwm_val);
  if(dir == 1)
    digitalWrite(in1_pin, HIGH);
  else
    digitalWrite(in1_pin, LOW);  
}

template <int j>
void readEncoder()
{
  int b = digitalRead(encb[j]);
  if(b > 0)
  {
    posi[j]++;
  }
  else
  {
    posi[j]--;
  }
}