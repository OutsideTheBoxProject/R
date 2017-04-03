/* ----------- includes --------------*/

#include <Process.h>
#include <Wire.h>
#include <TimerOne.h>

/* ----------- defines --------------*/

#define LIGHT (String)"http://192.168.0.100/api/EE2Md-XKTaIjMj9J78ztSPZruD9l9P6A0SknxsJd/lights/1/state"
#define COLOURCOMMAND1 (String)"curl -X PUT -d '{\"hue\" : "
#define COLOURCOMMAND2 (String)", \"sat\" : 250, \"bri\":254, \"transitiontime\": 0 }' " + LIGHT
#define COLOURCOMMAND2T (String)", \"sat\" : 250, \"bri\":254, \"transitiontime\": 30 }' " + LIGHT
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

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C

#define GYRO_FULL_SCALE_250_DPS 0x00
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2_G 0x00
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

#define DEBUG 1

/* ----------- global variables --------------*/
//lights
const int buttonPin = 2;
int buttonState = 0;
bool lightON = 0;
Process p;
int brightness = 254;
bool brighter = 0;

//MPU
long int ti; 
volatile bool intFlag = false; 
long int cpt = 0;

//Microphone
const int sampleWindow = 250;
unsigned int knock; 


/* ----------- functions --------------*/

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up... Initialising Bridge.");
  Bridge.begin();   // Initialize the Bridge
  Serial.println("Bridge initialised.");
//  Wire.begin();
//  Serial.println("Wire intitialised.");
//  pinMode(buttonPin, INPUT);
//  Serial.println("Button Pin set.");
//  
//  // Configure gyroscope range
//  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_2000_DPS);
//  Serial.println("Gyroscope set.");
//  // Configure accelerometers range
//  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_16_G);
//  // Set by pass mode for the magnetometers
//  I2CwriteByte(MPU9250_ADDRESS,0x37,0x02);
//  
  // Request first magnetometer single measurement
  //I2CwriteByte(MAG_ADDRESS,0x0A,0x01);
  
  Serial.println("Ready to go!");
}

void loop() {
  //gyroLoop();
  microphoneLoop();

  buttonState = digitalRead(buttonPin);

  if ( buttonState == HIGH ) {
    Serial.println("Button pressed");
    playTravel();
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
  Serial.println( "Command is: " + command);
  p.runShellCommand(command);
  while (p.running());
  delay(del);
}


void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
    // Set register address
    Wire.beginTransmission(Address);
     Wire.write(Register);
    Wire.endTransmission();
    // Read Nbytes
    Wire.requestFrom(Address, Nbytes);
    uint8_t index=0;
    while (Wire.available()){
      Data[index++]=Wire.read();
    }
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
  Serial.println("Fired up I2CwriteByte.");
  // Set register address
  Wire.beginTransmission(Address);
  Serial.println("Transmission begun.");
  Wire.write(Register);
  Serial.println("Register written.");
  Wire.write(Data);
  Serial.println("Data written.");
  Wire.endTransmission();
  Serial.println("Transmission ended.");
}

void callback(){
  intFlag=true;
  digitalWrite(13, digitalRead(13) ^ 1);
}

void gyroLoop(){
   // _______________
  // ::: Counter :::
  
  // Display data counter
  Serial.print (cpt++,DEC);
  Serial.print ("\t");
  
  // ____________________________________
  // :::  accelerometer and gyroscope ::: 

  // Read accelerometer and gyroscope
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
  
  
  // Create 16 bits values from 8 bits data
  
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];

  // Gyroscope
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  int16_t gz=Buf[12]<<8 | Buf[13];
  
    // Display values
  
  // Accelerometer
  Serial.print (ax,DEC); 
  Serial.print ("\t");
  Serial.print (ay,DEC);
  Serial.print ("\t");
  Serial.print (az,DEC);  
  Serial.print ("\t");
  
  // Gyroscope
  Serial.print (gx,DEC); 
  Serial.print ("\t");
  Serial.print (gy,DEC);
  Serial.print ("\t");
  Serial.print (gz,DEC);  
  Serial.print ("\t");

  // _____________________
  // :::  Magnetometer ::: 

  // Read register Status 1 and wait for the DRDY: Data Ready
//  
//  uint8_t ST1;
//  do
//  {
//    I2Cread(MAG_ADDRESS,0x02,1,&ST1);
//  }
//  while (!(ST1&0x01));
//
//  // Read magnetometer data  
//  uint8_t Mag[7];  
//  I2Cread(MAG_ADDRESS,0x03,7,Mag);
//  
//
//  // Create 16 bits values from 8 bits data
//  
//  // Magnetometer
//  int16_t mx=-(Mag[3]<<8 | Mag[2]);
//  int16_t my=-(Mag[1]<<8 | Mag[0]);
//  int16_t mz=-(Mag[5]<<8 | Mag[4]);
//  
//  
//  // Magnetometer
//  Serial.print (mx+200,DEC); 
//  Serial.print ("\t");
//  Serial.print (my-70,DEC);
//  Serial.print ("\t");
//  Serial.print (mz-700,DEC);  
//  Serial.print ("\t");
  
  // End of line
  Serial.println("");
//  delay(100);  
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

