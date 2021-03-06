/* ----------- includes --------------*/

#include <Process.h>
#include <TimerOne.h>
#include <MPU9250.h>
#include <quaternionFilters.h>
#include <Wiegand.h>

/* ----------- defines --------------*/

#define LIGHT (String)"http://192.168.0.100/api/EE2Md-XKTaIjMj9J78ztSPZruD9l9P6A0SknxsJd/lights/1/state"
#define COLOURCOMMAND1 (String)"curl -X PUT -d '{\"hue\" : "
#define COLOURCOMMAND2 (String)", \"sat\" : 250, \"bri\": 254, \"transitiontime\": 0 }' " + LIGHT
#define COLOURCOMMAND2T (String)", \"sat\" : 250, \"bri\": 254, \"transitiontime\": 30 }' " + LIGHT
#define SETBRIGHTNESS1 (String)"curl -X PUT -d '{\"bri\":"
#define SETBRIGHTNESS2 (String)"}' " + LIGHT

#define OFF (String)"curl -X PUT -d '{\"on\":false}' " + LIGHT
#define ON (String)"curl -X PUT -d '{\"on\":true}' " + LIGHT

#define RED (String)"0"
#define PINK (String)"56100"
#define PURPLE (String)"54100"
#define BLUE (String)"46920"
#define TURQUOISE (String)"42765"
#define DARKGREEN (String)"35092"
#define GREEN (String)"25500"
#define BRIGHTGREEN (String)"20128"
#define YELLOW (String)"12750"
#define ORANGE (String)"5384"
#define ORANGERED (String)"1029"

#define BRIGHTMAX 254
#define BRIGHTMIN 0

#define AHRS false         // Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging
#define ADDRESS 0x71
#define ALTADDRESS 0xFF

#define MASTERCODE 10976553

#define DEBUG 1

/* ----------- global variables --------------*/
//lights
const int buttonPin = 4;
int buttonState = 0;
bool lightON = 0;
Process p;
int brightness = 254;
bool brighter = 0;

//Microphone
const int sampleWindow = 250;
unsigned int knock; 

//MPU
int intPin = 2;  // These can be changed, 2 and 3 are the Arduinos ext int pins
MPU9250 sensor;
int sampling = 100; // measure every .1 seconds
int lastMillis = 0;

//RFID
WIEGAND wg;
long lastRFID = 0;


/* ----------- functions --------------*/

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up... Initialising Bridge.");
  Bridge.begin();   // Initialize the Bridge
  Serial.println("Bridge initialised.");
  //pinMode(intPin, INPUT);
  //digitalWrite(intPin, LOW);
  
  Wire.begin();
  Serial.println("Wire initialised.");

  byte c = sensor.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
  Serial.print(" I should be "); Serial.println(ADDRESS, HEX);

  if (c == ADDRESS || c == ALTADDRESS) // WHO_AM_I should always be 0x68
  {
    Serial.println("MPU9250 is online...");

    // Start by performing self test and reporting values
    sensor.MPU9250SelfTest(sensor.SelfTest);
    Serial.print("x-axis self test: acceleration trim within : ");
    Serial.print(sensor.SelfTest[0],1); Serial.println("% of factory value");

    // Calibrate gyro and accelerometers, load biases in bias registers
    sensor.calibrateMPU9250(sensor.gyroBias, sensor.accelBias);

    sensor.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    Serial.println("MPU9250 initialized for active data mode....");
  } // if (c == ADDRESS)
  else
  {
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
  }

  wg.begin(5, 2, 6, 3);
  Serial.println("RFID Reader initialised.");
  
  Serial.println("Ready to go!");
}

void loop() {
  RFIDLoop();
  gyroLoop();
  microphoneLoop();

  buttonState = digitalRead(buttonPin);

  if ( buttonState == HIGH ) {
    Serial.println("Button pressed");
    playTravel();
  }
}

void RFIDLoop(){
  if ( wg.available() &&  (abs(millis() - lastRFID) > 1000) ) {
    lastRFID = millis();
    unsigned long code = wg.getCode();
    Serial.println(code);
    if (code == MASTERCODE){
      Serial.println("They connected! <3");
    }
  }
  
}

void playTravel() {
  Serial.println("Starting light show");

  placeCommand(ON, 0);
  lightON = 1;
  placeColourCommand(RED, 1, 4000);
  placeColourCommand(PINK, 1, 4000);
  placeColourCommand(PURPLE, 1, 4000);
  placeColourCommand(BLUE, 1, 4000);
  placeColourCommand(TURQUOISE, 1, 4000);
  placeColourCommand(DARKGREEN, 1, 4000);
  placeColourCommand(GREEN, 1, 4000);
  placeColourCommand(BRIGHTGREEN, 1, 4000);
  placeColourCommand(YELLOW, 1, 4000);
  placeColourCommand(ORANGE, 1, 4000);
  placeColourCommand(ORANGERED, 1, 4000);
  placeColourCommand(RED, 1, 4000);
  placeColourCommand(PINK, 1, 4000);
  placeColourCommand(PURPLE, 1, 4000);
  placeColourCommand(BLUE, 1, 4000);
  if (DEBUG){
    placeCommand(OFF, 0);
    lightON = 0;
  }
  Serial.println("Done with light show");
}

void placeColourCommand(String hue, int transition, int del) {
  String command = COLOURCOMMAND1 + hue;
  if (transition == 1) {
    command = command + COLOURCOMMAND2T;
  } else {
    command = command + COLOURCOMMAND2;
  }
  placeCommand(command, del);
}

void placeCommand(String command, int del) {
  Serial.println(command);
  p.runShellCommand(command);
  while (p.running());
  delay(del);
}


void microphoneLoop(){
  unsigned long start = millis(); 
  unsigned int peakToPeak = 0; 

  unsigned int signalMax = 0; 
  unsigned int signalMin = 1024; 

  while (millis()-start < sampleWindow){
    knock = analogRead(0);
    if (knock < 1024){
      if (knock > signalMax){
        signalMax = knock;   
      }  else if (knock < signalMin){
        signalMin = knock;   
      }
    }
  }

  peakToPeak = signalMax - signalMin; 
  double volts = (peakToPeak * 3.3)/1024; 

  if (volts >= 1.0){
    Serial.println("Knock Knock."); 
    Serial.print("volts: "); 
    Serial.println(volts);
    if (volts >= 2.1){
      Serial.println("That's it!  That's way too loud!"); 
      if (lightON == 0){
        placeCommand(ON, 0);   
        lightON = 1;
      } else if (lightON == 1){
        placeCommand(OFF, 0); 
        lightON = 0;   
      }
    } else if (lightON == 1){
      if (brighter == 1){
        brightness += 25; 
        if (brightness > BRIGHTMAX){
          brightness = BRIGHTMAX;  
          brighter = 0;
        }
      } else if (brighter == 0){
        brightness -= 25; 
        if (brightness < BRIGHTMIN){
          brightness = BRIGHTMIN; 
          brighter = 1;  
        }
      }
      placeCommand(SETBRIGHTNESS1 + brightness + SETBRIGHTNESS2, 0);
    }
    
  }
}


void gyroLoop()
{
  if (millis() - lastMillis > sampling){
    if (sensor.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
    {  
      sensor.readAccelData(sensor.accelCount);  // Read the x/y/z adc values
      sensor.getAres();

      sensor.ax = (float)sensor.accelCount[0]*sensor.aRes; // - accelBias[0];
      sensor.delt_t = millis() - sensor.count;

      // update LCD once per half-second independent of read rate
      if (sensor.delt_t > 500)
      {
        if(SerialDebug)
        {
          Serial.print("ax = "); Serial.print((int)1000*sensor.ax);
          Serial.println(" mg");
        }
      }
    }
    lastMillis = millis();
  }
}
