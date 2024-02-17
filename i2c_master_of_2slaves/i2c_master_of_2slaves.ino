/*I2C Master write code*/
/*List of the lubraries included in the project*************************************************/
#include <Wire.h>                                                           //Arduino library that enables I2C functionality
/*defining local variables that will be used in the program*************************************/
String readString;                                                          // defining the string
int ChoiceOfTransmission;                                                   // definig a varijable that will be decide to which arduino are we sending the data 
/*List of the variables that will be sent via I2C***********************************************/
byte I2C_OnOff;                                                             //defining the variable that will be sent
/*Setup loop************************************************************************************/
void setup() 
{
  Wire.begin();                                                             // join I2C bus (address here is optional for master)
  Serial.begin(9600);
  Serial.println("Type On to turn on the LED and Off to shut it down!");    // Instruction text and check off what is loaded
}
/*Main loop**************************************************************************************/
void loop() 
{
  /*While that loops through input from the serialmonitor input and assembels them to the string***/ 
  while (Serial.available()) 
  {
    delay(2);                                                               // Delay that allows byte to arrive in input buffer
    char c = Serial.read();                                                 // Reads the char form serial monitor
    readString += c;                                                        // Add the char to the readString string
  }
  /************************************************************************************************/
  /*IF THEN ELSE that depending on the input text sets te value to I2C_OnOff***********************/    if (readString == "BlueOn" or readString == "BLUEON" or readString == "blueon")
  {
    I2C_OnOff = 1;
    ChoiceOfTransmission = 1;
  }
  else if (readString == "RedOn" or readString == "REDON" or readString == "redon")
  {
    I2C_OnOff = 1;
    ChoiceOfTransmission = 2; 
  }
  else if (readString == "BlueOff" or readString == "BLUEOFF" or readString == "blueoff")
  {
    I2C_OnOff = 0;
    ChoiceOfTransmission = 1; 
  }
  else if (readString == "RedOff" or readString == "REDOFF" or readString == "redoff")
  {
    I2C_OnOff = 0;
    ChoiceOfTransmission = 2;    
  }
  /*IF for printing out the users input************************************************************/
  /************************************************************************************************/
  if (readString.length() >0)                                                   // while string lenght is not 0  
  {
    Serial.println(readString);                                                 // prints out the user inputed string
    readString="";                                                              // deletes/empties the string
  }
  /************************************************************************************************/
  /*Switch statemant that depending on the chosen slave arduino executes the transmition of dat****/
  switch (ChoiceOfTransmission)
  {
    case 1:               //Sending data on the first arduino
      Wire.beginTransmission(1);                                                // Oening the transmition chanel to device with the name 1                               
      Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
      Wire.endTransmission();                                                   // Closeing the transmition chanel
      ChoiceOfTransmission = 0;                                                 // setting the case choice varijable to so that there are no repeat transmitions unless user types the comand again 
      break;
    case 2:               //Sending data on the second arduino
      Wire.beginTransmission(2);                                                // Oening the transmition chanel to device with the name 1                               
      Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
      Wire.endTransmission();                                                   // Closeing the transmition chanel
      ChoiceOfTransmission = 0;                                                 // setting the case choice varijable to so that there are no repeat transmitions unless user types the comand again
      break;
    default:              // if the user choice is not put in or the command has 
                          // allready been sent then choice variable is 0 or set to 0, 
                          // which is out of the range for command to tansmit data, 
                          // if that is the case then this part of the code is executed.
      break;              
  }
  /************************************************************************************************/  
}