#include <Process.h>

#define LIGHT (String)"http://192.168.0.100/api/EE2Md-XKTaIjMj9J78ztSPZruD9l9P6A0SknxsJd/lights/1/state"
#define COLOURCOMMAND1 (String)"curl -X PUT -d '{\"hue\" : "
#define COLOURCOMMAND2 (String)", \"sat\" : 250, \"bri\":254, \"transitiontime\": 0 }' " + LIGHT
#define COLOURCOMMAND2T (String)", \"sat\" : 250, \"bri\":254, \"transitiontime\": 30 }' " + LIGHT

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


//Global Variables
const int buttonPin = 2; 
int buttonState = 0; 
int lightON = 0;
Process p;


void setup(){
  Bridge.begin();   // Initialize the Bridge
  Serial.begin(9600);
  pinMode(buttonPin, INPUT); 
  Serial.println("Ready to go!");
}

void loop(){
  
  buttonState = digitalRead(buttonPin);

  if( buttonState == HIGH ){
    Serial.println("Button pressed");
    playTravel(); 
  }

}

void playTravel(){
  Serial.println("Starting light show");

  placeCommand(ON, 0);
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
  placeCommand(OFF, 0);

  Serial.println("Done with light show");  
}

void placeColourCommand(String hue, int transition, int del){
  String command = COLOURCOMMAND1 + hue; 
  if (transition == 1){
    command = command + COLOURCOMMAND2T;  
  } else {
    command = command + COLOURCOMMAND2;  
  }
  placeCommand(command, del);
}

void placeCommand(String command, int del){
  Serial.println( "Command is: " + command);
  p.runShellCommand(command);
  while(p.running());
  delay(del);
}



