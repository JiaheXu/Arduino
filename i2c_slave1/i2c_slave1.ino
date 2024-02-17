/*I2C Slave reciever code for HC-SR04 Stops the Gearmotor Through I2C*/
/*List of the lubraries included in the project*************************************************/
#include <Wire.h>                               // Arduino library that enables I2C functionality
/*Pin definition********************************************************************************/
int DO_Blink = 13;
/*List of the variables that will be recieved via I2C*******************************************/
byte I2C_OnOff;                                 //defining the variable that will be sent
/*Setup loop************************************************************************************/
void setup() 
{
  pinMode(DO_Blink, OUTPUT);                    // Sets the DO_Blink as output
  Wire.begin(1);                                // Join I2C bus as the slave with address 1
  Wire.onReceive(BlinkLAD);                     // When the data transmition is detected call receiveEvent function

}
/*Main loop**************************************************************************************/
void loop() 
{
delay(100);
}
/*Function/Event call****************************************************************************/
void BlinkLAD(int Press)
{
  I2C_OnOff = Wire.read();                      // Reads the data sent via I2C
  // IF THEN ELSE that blinks the LAD depending on the values recieved via I2C
  if (I2C_OnOff == 1)
  {
   digitalWrite(DO_Blink, HIGH);                // Sets the LAD On 
  }
  else if (I2C_OnOff == 0)
  {
   digitalWrite(DO_Blink, LOW);                 // Sets the LAD Off
  }
   
}