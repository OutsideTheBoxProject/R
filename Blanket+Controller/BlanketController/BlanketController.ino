#include <RCSwitch.h>  

#include "signal.h"

#define MODE_BUTTON   3
#define START_BUTTON  4
#define STOP_BUTTON   5
#define TRANSMITTER   10

RCSwitch sender = RCSwitch(); 

void setup() {
  // put your setup code here, to run once:
  // Activate the buttons.
  pinMode(MODE_BUTTON, INPUT);
  digitalWrite(MODE_BUTTON, HIGH);
  pinMode(START_BUTTON, INPUT);
  digitalWrite(START_BUTTON, HIGH);
  pinMode(STOP_BUTTON, INPUT);
  digitalWrite(STOP_BUTTON, HIGH);

  // Turn on the transmitter
  sender.enableTransmit(TRANSMITTER);
  sender.setProtocol(2);

  Serial.begin(9600);
  Serial.println("Setup Complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  checkModeState();
  checkStartState();
  checkStopState();
}

/*
 * Abstractly asks, given a button and a pointer to a 
 * state parameter, if that button has been pressed once.
 * 
 * Returns true 
 */
bool buttonPressedOnce(int button, int *state) {
  int buttonState = digitalRead(button);

  if(HIGH == buttonState && HIGH != *state) {
    // Button newly pressed
    *state = buttonState;
    return true;
  } 
  *state = buttonState;
  return false;
}


/*
 * Chck if mode button has been pressed.
 */
int modeButtonState = 0;
void checkModeState() {
  if(buttonPressedOnce(MODE_BUTTON, &modeButtonState)) {
    Serial.println("Mode Pressed.");
    sender.send(SIGNAL_MODE, 3);
  }
}

/*
 * Check if start has been pressed.
 */
int startButtonState = 0;
void checkStartState() {
  if(buttonPressedOnce(START_BUTTON, &startButtonState)) {
    Serial.println("Start Pressed.");
    sender.send(SIGNAL_START, 3);
  }
}

/*
 * Check if stop has been pressed.
 */
int stopButtonState = 0;
void checkStopState() {
  if(buttonPressedOnce(STOP_BUTTON, &stopButtonState)) {
    Serial.println("Stop Pressed.");
    sender.send(SIGNAL_STOP, 3);
  }
}

