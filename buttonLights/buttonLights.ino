#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

int buttonState = 0;
int buttonPresses = 0;
long currentMillis = 0;
int interval = 4000;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pinMode(A0, INPUT);
  strip.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  read_button();
}

void read_button() {
  buttonState = digitalRead(A0);
  if (buttonState == HIGH) {
    buttonPresses = buttonPresses + 1; 
  if (buttonPresses == 1){
    theaterChase(strip.Color(127, 127, 127), 50);
  }
  else if (buttonPresses == 2){
    theaterChase(strip.Color(127, 127, 0), 50);
  }
  else if (buttonPresses == 3){
    theaterChase(strip.Color(127, 0, 127), 50);
  }
  else if (buttonPresses == 4){
    theaterChase(strip.Color(0, 127, 127), 50);
  }
  else if (buttonPresses == 5){
    theaterChase(strip.Color(127, 0, 0), 50);
  }
  else if (buttonPresses == 6){
    theaterChase(strip.Color(0, 127, 0), 50);
  }
  else if (buttonPresses == 7){
    theaterChase(strip.Color(0, 0, 127), 50);
  }
  else if (buttonPresses == 8){
    theaterChaseRainbow(50);
    buttonPresses = 0;
  } 
  }
}

// Functions below taken from the strand test example by the
// neopixel library

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
