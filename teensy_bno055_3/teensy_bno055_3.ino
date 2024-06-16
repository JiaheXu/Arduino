#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>

#define BNO055_DELAY_MS (100)

Adafruit_BNO055 sensor = Adafruit_BNO055();

float filterPOld, filterPNew = 0; // Precision variables, Derived in setup function

float thetaM; // Measured inclination based on accelerometer X axis.
float thetaFOld=0; // Filtered Old Value - Initial value is 0
float thetaFNew; // Filtered New Value, will derived and will replace the old value for next iteration

float phiM;
float phiFOld=0; // Filtered Old Value - Initial value is 0
float phiFNew; // Filtered New Value, will derived and will replace the old value for next iteration

unsigned long timePrevious; // Variable to store the old time in milliseconds.
float timeDiff; // variable to capture the time difference
float thetaG = 0; // Angular distance in terms of angle on x axis
float phiG = 0; // Angular distance in terms of angle on y axis

float roll = 0; // Complementary filter variable
float pitch = 0; // Complementary filter variable
float trust = 0.95; // Trust percentage for complementary filter on gyroscope

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensor.begin();
  delay(1000);
  int8_t temp=sensor.getTemp();
  sensor.setExtCrystalUse(true);
  // Derive the filter precision for the old and new value
  filterPOld = 0.95;// FILTER_PRECISION in percentage / 100;
  filterPNew = 1 - filterPOld;
  timePrevious = millis(); // Initializing the time in milliseconds
}
float state_float_buffer[ 3 ];
byte state_byte_buffer[ 3*4 ];
void loop() {
  // put your main code here, to run repeatedly:
  // Reading accelerometer data 
  uint8_t system, gyro, accel, mg = 0;
  sensor.getCalibration(&system, &gyro, &accel, &mg);

  sensors_event_t event; 
  sensor.getEvent(&event);

  state_float_buffer[0] = event.orientation.z;
  state_float_buffer[1] = event.orientation.y;
  state_float_buffer[2] = event.orientation.x;
   
  Serial.print("<");
  memcpy( state_byte_buffer, state_float_buffer, 3*4);
  Serial.write(state_byte_buffer, 3*4);  
  Serial.print(">");
  delay(BNO055_DELAY_MS);

}