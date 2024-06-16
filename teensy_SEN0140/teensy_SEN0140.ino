#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Wire.h>

float angles[3]; // yaw pitch roll
float quat[4]; // w x y z
// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();

void setup() {
  Serial.begin(9600);
  Wire.begin();

  delay(5);
  sixDOF.init(); //begin the IMU
  delay(5);
}
float state_float_buffer[ 4 ];
byte state_byte_buffer[ 4*4 ];
void loop() {

  sixDOF.getEuler(angles);
  Serial.print(angles[0]);
  Serial.print(" | ");
  Serial.print(angles[1]);
  Serial.print(" | ");
  Serial.println(angles[2]);


  // sixDOF.getQ(quat);
  // state_float_buffer[3] = quat[0];
  // state_float_buffer[0] = quat[1];
  // state_float_buffer[1] = quat[2];
  // state_float_buffer[2] = quat[3];

  // Serial.print("<");
  // memcpy( state_byte_buffer, state_float_buffer, 4*4);
  // Serial.write(state_byte_buffer, 4*4);  
  // Serial.print(">");

  delay(100);
}