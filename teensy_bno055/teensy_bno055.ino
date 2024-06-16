#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}

float state_float_buffer[ 4 ];
byte state_byte_buffer[ 4*4 ];

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  // Serial.print("X: ");
  // Serial.print(event.orientation.x, 4);
  // Serial.print("\tY: ");
  // Serial.print(event.orientation.y, 4);
  // Serial.print("\tZ: ");
  // Serial.print(event.orientation.z, 4);
  // Serial.println("");

imu::Quaternion quat = bno.getQuat();

  /* Display the quat data */
  // Serial.print("qW: ");
  // Serial.print("<");
  // Serial.print(quat.w(), 4);
  // Serial.print(quat.x(), 4);
  // Serial.print(quat.y(), 4);
  // Serial.print(quat.z(), 4);
  // // Serial.print(">");
  // Serial.println(">");

  state_float_buffer[0] = quat.x();
  state_float_buffer[1] = quat.y();
  state_float_buffer[2] = quat.z();
  state_float_buffer[3] = quat.w();    
  Serial.print("<");
  memcpy( state_byte_buffer, state_float_buffer, 4*4);
  Serial.write(state_byte_buffer, 4*4);  
  Serial.print(">");
    
  delay(100);
}
