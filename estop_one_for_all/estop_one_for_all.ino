//Xbee TX setup:

#include <SoftwareSerial.h>
#define rxPin 2 //DOUT --> PIN 2
#define txPin 3 //DIN --> PIN 3
SoftwareSerial xbee = SoftwareSerial(rxPin, txPin);

int outPin = 12;    // pushbutton connected to digital pin 8
int inPin = 7;    // pushbutton connected to digital pin 8

// B on 20
// 7E 00 10 17 01 00 13 A2 00 41 90 8A 87 FF FE 02 44 31 05 D7
// B off 19
// 7E 00 0F 17 01 00 13 A2 00 41 90 8A 87 FF FE 02 46 52 B9

// C on 20
// 7E 00 10 17 01 00 13 A2 00 41 89 0E EE FF FE 02 44 31 05 F3
// C off 19
// 7E 00 0F 17 01 00 13 A2 00 41 89 0E EE FF FE 02 46 52 D5


// D on 20
// 7E 00 10 17 01 00 13 A2 00 41 90 89 B2 FF FE 02 44 31 05 AD
// D off 19
// 7E 00 0F 17 01 00 13 A2 00 41 90 89 B2 FF FE 02 46 52 8F

// ON means estop on, robot stop
// OFF means estop off, robot moves

byte B_ON[] = {0x7E, 0x00, 0x10, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x42, 0x44, 0xD1, 0x46, 0xFF, 0xFE, 0x02, 0x44, 0x31, 0x05, 0x1C}; //19 FR
byte B_OFF[] = {0x7E, 0x00, 0x0F, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x42, 0x44, 0xD1, 0x46, 0xFF, 0xFE, 0x02, 0x46, 0x52, 0xFE}; //20 D1

byte C_ON[] = {0x7E, 0x00, 0x10, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x89, 0x0E, 0xEE, 0xFF, 0xFE, 0x02, 0x44, 0x31, 0x05, 0xF3}; //20 D1
byte C_OFF[] = {0x7E, 0x00, 0x0F, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x89, 0x0E, 0xEE, 0xFF, 0xFE, 0x02, 0x46, 0x52, 0xD5}; //19 FR

byte D_ON[] = {0x7E, 0x00, 0x10, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x90, 0x89, 0xB2, 0xFF, 0xFE, 0x02, 0x44, 0x31, 0x05, 0xAD}; //20 D1
byte D_OFF[] = {0x7E, 0x00, 0x0F, 0x17, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x90, 0x89, 0xB2, 0xFF, 0xFE, 0x02, 0x46, 0x52, 0x8F}; //19 FR


// RGB set
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        5
#define NUMPIXELS 7

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
  Serial.begin(9600);
  Serial.println("Remote E-Stop");
  //xbee setup
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  xbee.begin(9600);


  pinMode(outPin, OUTPUT);  // sets the digital pin 13 as output
  

  pinMode(inPin, INPUT);    // sets the digital pin 8 as input




  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
}


void trigger_all()
{
  xbee.write(B_ON,20);
  xbee.write(C_ON,20);
  xbee.write(D_ON,20);
}

void reset_all()
{
  xbee.write(B_OFF,19);
  xbee.write(C_OFF,19);
  xbee.write(D_OFF,19);
}

int val = 0;
int last_val = -1;
void loop()
{

  pixels.clear();

  digitalWrite(outPin, HIGH);
  val = digitalRead(inPin);
  if( val == HIGH)
  {
    if(last_val != val)
      trigger_all();
    // Serial.println("ON ON ON ON ON ON");

    for(int i=0; i<NUMPIXELS; i++) 
    {
      pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      pixels.show();
    }
    delay(300);

  }
  else
  {
    if(last_val != val)
      reset_all();
    // Serial.println("OFF OFF OFF OFF OFF OFF");
    for(int i=0; i<NUMPIXELS; i++) 
    {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      pixels.show();
    }
    delay(300);    
    // if(last_val == HIGH)
    // {
    //   reset_all();
    // }
  }
  // last_val = val;
  // trigger_all();
  // delay(2000);
  // reset_all();
  // delay(2000);
  last_val = val;

}