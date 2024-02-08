#include <util/atomic.h> // For the ATOMIC_BLOCK macro

#define ENCA 2 // WHITE
#define ENCB 3 // YELLOW
#define PWM 5
// #define IN2 6
#define IN1 7

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/

volatile int lastEncoded = 0; // Here updated value of encoder store.
volatile long encoderValue = 0; // Raw encoder value

long prevT = 0;
float eprev = 0;
float eintegral = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  
  // pinMode(ENCA, INPUT_PULLUP); 
  // pinMode(ENCB, INPUT_PULLUP);
  // digitalWrite(ENCA, HIGH); //turn pullup resistor on
  // digitalWrite(ENCB, HIGH); //turn pullup resistor on
  // //call updateEncoder() when any high/low changed seen
  // //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  // attachInterrupt(0, updateEncoder, CHANGE); 
  // attachInterrupt(1, updateEncoder, CHANGE);

  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  Serial.println("target pos");
  // pinMode(IN2,OUTPUT);
}

void loop() {
  
  // set target position
  int target = 1000;
  // int target = 250*sin(prevT/1e6);

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
    // pos = encoderValue;
  }

  // error
  int e = pos - target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;

  Serial.print("u: ");
  Serial.print(u);
  Serial.println();


  // motor power
  int pwr = int(fabs(u));
  if( pwr > 50 ){
    pwr = 50;
  }
  if( pwr < 7 ){
    pwr = 7;
  }
  // motor direction
  int dir = 1;
  if(u < 0){
    dir = 0;
  }
  // Serial.println("val: ");
  // Serial.println(encoderValue);
  if(e == 0)
    pwr = 0;

  // setMotor(1, 10, PWM, IN1);
  // delay(200);
  // Serial.println(pos);
  Serial.print("pwr: ");
  Serial.print(pwr);
  Serial.println();

  setMotor(dir, pwr, PWM, IN1);

  eprev = e;
  Serial.print("target: ");
  Serial.print(target);
  Serial.print(" ");
  Serial.print("pos: ");
  Serial.print(pos);
  Serial.println();
  // delay(100);

}

void setMotor(int dir, int pwmVal, int pwm, int in1){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
  }
  else
  {
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
}



void updateEncoder(){
  int MSB = digitalRead(ENCA); //MSB = most significant bit
  int LSB = digitalRead(ENCB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue ++;

  lastEncoded = encoded; //store this value for next time
  Serial.println("val: ");
  Serial.println(encoderValue);
}