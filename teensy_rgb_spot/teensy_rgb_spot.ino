#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define RGB_PIN        10
#define RELAY_PIN       8
#define ESTOP_PIN      6
#define NUMPIXELS 7

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pinMode(RGB_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(ESTOP_PIN, INPUT);
  pixels.begin();
}
int val = 0;
int last_val = -1;
void loop() 
{

  val = digitalRead(ESTOP_PIN);

  pixels.clear();
  if(last_val != val)
  {

    if(val == HIGH) // estop active
    {
      for(int i=0; i<NUMPIXELS; i++) 
      {
        pixels.setPixelColor(i, pixels.Color(150, 150, 0)); // yellow light
        pixels.show();
      }
      delay(5000);
      for(int i=0; i<NUMPIXELS; i++) 
      {
        pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // red light
        pixels.show();
      }
      
    } 
    
    // estop active deactive
    else
    {
      for(int i=0; i<NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
        pixels.show();
      }
      digitalWrite(RELAY_PIN, LOW);
    }
  }
  digitalWrite(RELAY_PIN, val); // pass to relay
  last_val = val;

  // if(val == HIGH)
  // delay(100);
}