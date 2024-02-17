#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define RGB_PIN        10
#define ESTOP_PIN      6
#define NUMPIXELS 7

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pinMode(RGB_PIN, OUTPUT);
  pinMode(ESTOP_PIN, INPUT);
  pixels.begin();
}
int val = 0;
void loop() {

  val = digitalRead(ESTOP_PIN);
  pixels.clear();


  for(int i=0; i<NUMPIXELS; i++) {
    if(val == HIGH)
      pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    else
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();
  }
  if(val == HIGH)
      delay(100);
}