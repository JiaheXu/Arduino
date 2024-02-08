#include <Wire.h>


void setup() {

  Wire.begin();        // join i2c bus (address optional for master)

  Serial.begin(9600);  // start serial for output

}


void loop() {

  Wire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8


  while (Wire.available()) { // peripheral may send less than requested

    char c = Wire.read(); // receive a byte as character

    Serial.print(c);         // print the character

  }


  delay(500);

}

// #include <Wire.h>


// void setup() {

//   Wire.begin(8);                // join i2c bus with address #8

//   Wire.onRequest(requestEvent); // register event

// }


// void loop() {

//   delay(100);

// }


// // function that executes whenever data is requested by master

// // this function is registered as an event, see setup()

// void requestEvent() {

//   Wire.write("hello "); // respond with message of 6 bytes

//   // as expected by master

// }