#include <RCSwitch.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "signal.h"

#define LIGHTS 6

#define MAX_MODES 22

RCSwitch receiver = RCSwitch(); 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, LIGHTS, NEO_GRB + NEO_KHZ800);

int last = 0;
bool traveling = false;
uint32_t off = strip.Color(0 ,0, 0);
int genTime = 75;
long lastMillis = 0;
int pulseTime = 6000; 
bool showing = false;

void setup() {
  Serial.begin(115200); 
  receiver.enableReceive(0); //PIN 2 on the board equals interrupt 0
  strip.begin();
  strip.show();
  randomSeed(analogRead(0));

  //strip.setBrightness(64);
}

void loop(){
  check_receiver();

  if (traveling){
    travel();
  } else {
    //test_pulses();  
  }
}

void test_pulses(){

  if (millis() - lastMillis <= pulseTime){
    return;  
  }

  clearStrip();  
  uint32_t col = pickRandomColour();
  // every third pixel gets the random pixel colour
  for(uint16_t i=last; i<strip.numPixels(); i+=3) {
    strip.setPixelColor(i, col);
  }
      strip.show();

  lastMillis = millis();
  advanceLast();
}

void advanceLast() {
  last++;
  if (last > 2){
    last = 0;
  }  
}

bool check_receiver(){
  if(receiver.available()) {    
    int val = receiver.getReceivedValue(); 

    Serial.print("Received: ");
    Serial.println(val);

    switch(val) {
    case SIGNAL_MODE:
      changeMode();
      break;
    case SIGNAL_START:
      startPlaying();
      break;
    case SIGNAL_STOP:
      stopPlaying();
      break;
    default:
      break;
    }
    
    delay(400);
    receiver.resetAvailable(); // reset for next signal
    return true;
  }

  return false;
  
}

int modeCount = 0;
void changeMode() {
  if (!traveling){
    Serial.print("Adjusting mode: ");
    modeCount++;
    if(modeCount > MAX_MODES) {
      modeCount = 0;
    }
    Serial.println(modeCount);
  }
}

void startPlaying() {
  Serial.println("Starting.");
  traveling = true;
}

void stopPlaying() {
  Serial.println("Stopping");
  traveling = false;
  modeCount = 0;
}

uint32_t pickRandomColour(){
  return strip.Color(random(255), random(255), random(255)); 
}

void clearStrip() {
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, off);
  }
  strip.show();
}

void travel(){
    switch (modeCount){
      case 16:
        colorWipe(strip.Color(255,0,0), genTime);
        break;
      case 17: 
        rainbow(40);
        break;
      case 2: 
        colorWipe(strip.Color(255,255,0), genTime);
        break;
      case 3: 
        rainbowCycle(40);
        break;
      case 4: 
        colorWipe(strip.Color(255,0,255), genTime);
        break;
      case 5: 
        theaterChaseRainbow(genTime);
        break;
      case 6: 
        colorWipe(strip.Color(255,255,255), genTime);
        break;
      case 7:
        theaterChase(strip.Color(127, 127, 127), 1.2*genTime); // White
        break; 
      case 8: 
        colorWipe(strip.Color(0,255,0), genTime);
        break;
      case 9:  
        theaterChase(strip.Color(127, 0, 0), 1.2*genTime);
        break; 
      case 10: 
        colorWipe(strip.Color(0,255,255), genTime);
        break;
      case 11: 
        theaterChase(strip.Color(127, 127, 0), 1.2*genTime);
        break;
      case 12: 
        colorWipe(strip.Color(0,0,255), genTime);
        break;
      case 13: 
        theaterChase(strip.Color(127, 0, 127), 1.2*genTime);
        break; 
      case 14: 
        theaterChase(strip.Color(0, 127, 127), 1.2*genTime);
        break;
      case 15: 
        theaterChase(strip.Color(0,0,127), 1.2*genTime);
        break;
      case 0: 
      case 1: 
        blinkColours();
        break;
      case 18:
      case 19: 
      case 20: 
        theaterChase(pickRandomColour(), 1.2*genTime);
        break;
      case 21: 
      case 22:
      default: 
        colorWipe(pickRandomColour(), genTime);
    }
    clearStrip();
    traveling = false;
}


void blinkColours(){  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, pickRandomColour());
  }
  strip.show();
  delay(3000);
  clearStrip();
}


void colorWipe(uint32_t c, uint8_t wait) {
  for (int j = 0; j <= 15; j++){
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  clearStrip();
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
  clearStrip();
}

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

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i+=2) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  clearStrip();
}


void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int k = 0; k<=1; k++){
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  }
}
