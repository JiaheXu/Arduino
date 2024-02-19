#include <util/atomic.h>
#include <Wire.h>
#define SERIAL_BAUD 9600
#define N_FLOATS 4

int DEBUG = 0;
volatile byte* arrayPointer;

byte SLAVE_ADDRESS = 1;
volatile byte lastMasterCommand = 99;
volatile float array[N_FLOATS] = {5.5, 6.6, 7.7, 8.8};
volatile boolean sendStuff;

const byte dataCount = 4;
union myData_UNION{
 float floatData[dataCount];
 byte rawData[dataCount*sizeof(float)];
};
myData_UNION myData;
myData_UNION * pMyData;

// A class to compute the control signal
class SimplePID{
  private:
    float kp, kd, ki;
    int umax, umin; // Parameters
    float eprev, eintegral; // Storage

  public:
  // Constructor
  SimplePID() : kp(1), kd(0), ki(0), umin(60), umax(255), eprev(0.0), eintegral(0.0){}

  // A function to set the parameters
  void setParams(float kpIn, float kdIn, float kiIn, int uminIn, int umaxIn){
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

void setup() {
  Serial.begin(9600);

  for(int k = 0; k < NMOTORS; k++){
    pinMode(enca[k],INPUT);
    pinMode(encb[k],INPUT);
    pinMode(pwm[k],OUTPUT);
    pinMode(in1[k],OUTPUT);

    pid[k].setParams(1,0,0,50,255);
  }
  
  attachInterrupt(digitalPinToInterrupt(enca[0]),readEncoder<0>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[1]),readEncoder<1>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[2]),readEncoder<2>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[3]),readEncoder<3>,RISING);  
  Serial.println("target pos");
}

void loop() {

  // set target position
  int target[NMOTORS];
  target[0] = 2500;
  target[1] = -2500;
  target[2] = 1500;
  target[3] = -1500;   
  // target[0] = 750*sin(prevT/1e6);
  // target[1] = -750*sin(prevT/1e6);

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;

  // Read the position in an atomic block to avoid a potential misread
  int pos[NMOTORS];
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    for(int k = 0; k < NMOTORS; k++){
      pos[k] = posi[k];
    }
  }
  
  // loop through the motors
  for(int k = 0; k < NMOTORS; k++){
    int pwr, dir;
    // evaluate the control signal
    pid[k].evalu(pos[k],target[k],deltaT,pwr,dir);
    // signal the motor
    setMotor(dir,pwr,pwm[k],in1[k]);
  }

  for(int k = 0; k < NMOTORS; k++){
    Serial.print(target[k]);
    Serial.print(" ");
    Serial.print(pos[k]);
    Serial.print(" ");
  }
  Serial.println();
}

void setMotor(int dir, int pwm_val, int pwm_pin, int in1_pin){
  analogWrite(pwm_pin, pwm_val);
  if(dir == 1){
    digitalWrite(in1_pin,HIGH);
  }
  else{
    digitalWrite(in1_pin,LOW);
  }  
}

template <int j>
void readEncoder(){
  int b = digitalRead(encb[j]);
  if(b > 0){
    posi[j]++;
  }
  else{
    posi[j]--;
  }
}