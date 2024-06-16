#include <util/atomic.h> // For the ATOMIC_BLOCK macro
#include <EEPROM.h>

#define ENCA 1 // green
#define ENCB 0 // yellow
#define PWM 3 //EN
#define IN1 2 //ph
#define IN2 7

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;

int POS_ADDRESS = 0;

void setup() {
  Serial.begin(9600);
  // writeIntIntoEEPROM( POS_ADDRESS, 0);
  posi = readIntFromEEPROM(POS_ADDRESS);

  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  // pinMode(IN2,OUTPUT);
  
  // Serial.println("target pos");
}

void loop() {

  // set target position
  // int target = -2940;
  // int target = -36;
  int target = 0;

  // PID constants
  float kp = 1;
  float kd = 0.025;
  float ki = 0.0;

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;

  // Read the position in an atomic block to avoid a potential
  // misread if the interrupt coincides with this code running
  // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
  int pos = 0; 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;
  }
  Serial.print("MEM: ");
  Serial.println( readIntFromEEPROM(0) );
  // Serial.println(sizeof(int));
  // error
  int e = pos - target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;

  // motor power
  float pwr = fabs(u);
  if( pwr > 100 ){
    pwr = 100;
    // pwr = 20;
  }

  if( pwr < 50 ){
    pwr = 50;
  }
  // pwr = 40;
  if(e==0)
  {
    pwr = 0;
  }

  // motor direction
  int dir = 0;
  if(u>0)
  {
    dir = 1;
  }

  // signal the motor
  setMotor(dir,pwr,PWM,IN1);


  // store previous error
  eprev = e;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();
}

void setMotor(int dir, int pwmVal, int pwm, int in1){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
  }
  else{
    digitalWrite(in1,LOW);

  }  
}

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
  writeIntIntoEEPROM( POS_ADDRESS, posi);
  // writeIntIntoEEPROM( POS_ADDRESS, 0);
}


void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, (number >> 24) & 0xFF);
  EEPROM.write(address + 1, (number >> 16) & 0xFF);
  EEPROM.write(address + 2, (number >> 8) & 0xFF);
  EEPROM.write(address + 3, number & 0xFF);
}
long readIntFromEEPROM(int address)
{
  return ((long)EEPROM.read(address) << 24) +
         ((long)EEPROM.read(address + 1) << 16) +
         ((long)EEPROM.read(address + 2) << 8) +
         (long)EEPROM.read(address + 3);
}