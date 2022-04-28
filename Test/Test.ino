// Main file for Simpit Arduino -> Simpit KSP communication

#include "KerbalSimpit.h"

// Pinouts ====================================================================================================

// Analog

const int Throttle = A0;

// Shared

const int Stage = 42; // or 41

// Action Groups

const int AG1 = 29;
const int AG2 = 43;
const int AG3 = 58;
const int AG4 = 59;
const int AG5 = 60;

// CoPilot Controls

const int RollRightCop = 22;
const int RollLeftCop = 23;
const int PitchUpCop = 24;
const int PitchDownCop = 25;
const int YawRightCop = 26;
const int YawLeftCop = 27;
const int SASCop = 28;

// Pilot Controls

const int RollRightP = 47;
const int RollLeftP = 48;
const int PitchUpP = 49;
const int PitchDownP = 50;
const int YawRightP = 51;
const int YawLeftP = 52;
const int SASP = 53;

// MidConsole Controls

const int Lights = 30;
const int Brakes = 31;
const int Gear = 32;
const int RCS = 33;
const int RCSDirL = 34;
const int RCSDirR = 35;
const int RCSDirF = 36;
const int RCSDirB = 37;
const int RCSDirU = 38;
const int RCSDirD = 39;
const int Abort = 40;

// General Variable Setup ====================================================================================================

// Initial setup for values used later for controls and stuff like that

byte currentActionStatus = 0;

//int buttonState; // the current reading from the input pin

int buttonStateIgnition; // the current reading from the input pin
int buttonStateAbort; // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonStateStage;
int is_action_activated = 0;

// the following variables are unsigned long's because the time, measured
// in miliseconds, will quickly become a bigger number than can be stored
// in an int.

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int lastDebounceTimeStage = lastDebounceTime;
int lastButtonStateStage = lastButtonState;
int lastDebounceTimeAbort = lastDebounceTime;
int lastButtonStateAbort = lastButtonState;


// Communication setup

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.

KerbalSimpit mySimpit(Serial);


// Setup ====================================================================================================

// Runs once, to do things that only need to happen once right at the start
// but after the serial port is open

void setup() {

  // Open the serial connection.

  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Set up toggle switches with builtin pullup.

  pinMode(SASCop, INPUT_PULLUP);
  pinMode(SASP, INPUT_PULLUP);
  pinMode(RCS, INPUT_PULLUP);
  pinMode(Gear, INPUT_PULLUP);
  pinMode(Lights, INPUT_PULLUP);
  pinMode(Brakes, INPUT_PULLUP);
  pinMode(Abort, INPUT_PULLUP);
  pinMode(Stage, INPUT_PULLUP);
  pinMode(AG1, INPUT_PULLUP);
  pinMode(AG2, INPUT_PULLUP);
  pinMode(AG3, INPUT_PULLUP);
  pinMode(AG4, INPUT_PULLUP);
  pinMode(AG5, INPUT_PULLUP);


  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.

  while (!mySimpit.init()) {
    delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);

  // Sets our callback function. The KerbalSimpit library will
  // call this function every time a packet is received.

  mySimpit.inboundHandler(messageHandler);

  // Send a message to the plugin registering for the Action status channel.
  // The plugin will now regularly send Action status  messages while the
  // flight scene is active in-game.

  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
}


// Loop ====================================================================================================

// Loop to do controls and anything that needs to happen always

void loop() {
  // Check for new serial messages.

  mySimpit.update();

  // Controls ====================================================================================================

  // Throttle lever ====================================================================================================

  // Throttle reading and input
  // Send at each loop a message to control the throttle and the pitch/roll axis.

  throttleMessage throttle_msg;

  // Read the value of the potentiometer

  int readingThrottle = analogRead(Throttle);

  // Convert it in KerbalSimpit Range

  throttle_msg.throttle = map(readingThrottle, 0, 1023, 0, INT16_MAX);

  // Send the message

  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);


  // SAS Switches ====================================================================================================

  // Get the switch state

  bool sas_switch_stateC = digitalRead(SASCop);

  // Update the switch to match the state, only if a change is needed to avoid spamming commands.

  if (sas_switch_stateC && !(currentActionStatus & SAS_ACTION)) {
    mySimpit.printToKSP("Activate CoPilot SAS!");
    mySimpit.activateAction(SAS_ACTION);
  }
  if (!sas_switch_stateC && (currentActionStatus & SAS_ACTION)) {
    mySimpit.printToKSP("Desactivate CoPilot SAS!");
    mySimpit.deactivateAction(SAS_ACTION);
  }

  // Get the switch state

  bool sas_switch_stateP = digitalRead(SASP);

  // Update the switch to match the state, only if a change is needed to avoid spamming commands.

  if (sas_switch_stateP && !(currentActionStatus & SAS_ACTION)) {
    mySimpit.printToKSP("Activate Pilot SAS!");
    mySimpit.activateAction(SAS_ACTION);
  }
  if (!sas_switch_stateP && (currentActionStatus & SAS_ACTION)) {
    mySimpit.printToKSP("Desactivate CoPilot SAS!");
    mySimpit.deactivateAction(SAS_ACTION);
  }


  // RCS Switches ====================================================================================================

  // Get the switch state

  bool rcs_switch_state = digitalRead(RCS);

  // Update the switch to match the state, only if a change is needed to avoid spamming commands.

  if (rcs_switch_state && !(currentActionStatus & RCS_ACTION)) {
    mySimpit.printToKSP("Activate RCS!");
    mySimpit.activateAction(RCS_ACTION);
  }
  if (!rcs_switch_state && (currentActionStatus & RCS_ACTION)) {
    mySimpit.printToKSP("Desactivate RCS!");
    mySimpit.deactivateAction(RCS_ACTION);
  }


  // Stage Button ====================================================================================================

  int readingStage = digitalRead(Stage);

  // If the switch changed, due to noise or pressing:

  if (readingStage != lastButtonStateStage) {

    // reset the debouncing timer

    lastDebounceTimeStage = millis();
  }

  if ((millis() - lastDebounceTimeStage) > debounceDelay) {

    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:

    if (readingStage != buttonStateStage) {
      buttonStateStage = readingStage;

      // If the new button state is HIGH, that means the button
      // has just been pressed.

      if (buttonStateStage == HIGH) {

        // Send a message to the plugin activating the Stage
        // action group. The plugin will then activate the
        // next stage.

        mySimpit.activateAction(STAGE_ACTION);
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:

  lastButtonStateStage = readingStage;


  // Abort Button ====================================================================================================

  // Read the state of the switch into a local variable.

  int readingAbort = digitalRead(Abort);

  // If the switch changed, due to noise or pressing:

  if (readingAbort != lastButtonState) {

    // reset the debouncing timer

    lastDebounceTimeAbort = millis();
  }

  if ((millis() - lastDebounceTimeAbort) > debounceDelay) {

    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:

    if (readingAbort != buttonStateAbort) {
      buttonStateAbort = readingAbort;

      // If the new button state is HIGH, that means the button
      // has just been pressed.

      if (buttonStateAbort == HIGH) {

        // Send a message to the plugin activating the Stage
        // action group. The plugin will then activate the
        // next stage.

        mySimpit.activateAction(ABORT_ACTION);
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:

  lastButtonStateAbort = readingAbort;


  // Gear Switch ====================================================================================================


  // Get the switch state

  bool gear_switch_state = digitalRead(Gear);

  // Update the switch to match the state, only if a change is needed to avoid spamming commands.

  if (gear_switch_state && !(currentActionStatus & GEAR_ACTION)) {
    mySimpit.printToKSP("Activate Gear!");
    mySimpit.activateAction(GEAR_ACTION);
  }
  if (!gear_switch_state && (currentActionStatus & GEAR_ACTION)) {
    mySimpit.printToKSP("Desactivate Gear!");
    mySimpit.deactivateAction(GEAR_ACTION);
  }


  // Lights Switch ====================================================================================================

  // Get the switch state

  bool lights_switch_state = digitalRead(Lights);

  // Update the switch to match the state, only if a change is needed to avoid spamming commands.

  if (lights_switch_state && !(currentActionStatus & LIGHT_ACTION)) {
    mySimpit.printToKSP("Activate Lights!");
    mySimpit.activateAction(LIGHT_ACTION);
  }
  if (!lights_switch_state && (currentActionStatus & LIGHT_ACTION)) {
    mySimpit.printToKSP("Desactivate Lights!");
    mySimpit.deactivateAction(LIGHT_ACTION);
  }


  // Brakes Switch ====================================================================================================

  // Get the switch state

  bool brakes_switch_state = digitalRead(Brakes);

  // Update the switch to match the state, only if a change is needed to avoid spamming commands.

  if (brakes_switch_state && !(currentActionStatus & BRAKES_ACTION)) {
    mySimpit.printToKSP("Activate Brakes!");
    mySimpit.activateAction(BRAKES_ACTION);
  }
  if (!lights_switch_state && (currentActionStatus & BRAKES_ACTION)) {
    mySimpit.printToKSP("Desactivate Brakes!");
    mySimpit.deactivateAction(BRAKES_ACTION);
  }


  // ACtion Group Switch ====================================================================================================

  // Get the switch state

  bool AG1_switch_state = digitalRead(AG1);
  bool AG2_switch_state = digitalRead(AG2);
  bool AG3_switch_state = digitalRead(AG3);
  bool AG4_switch_state = digitalRead(AG4);
  bool AG5_switch_state = digitalRead(AG5);

  // Update the switch to match the state, only if a change is needed to avoid spamming commands.

  // Action Group 1

  if (AG1_switch_state && !(currentActionStatus )) {//& is_action_activated(AG1))) {
    mySimpit.printToKSP("Activate Action Group 1!");
    mySimpit.activateCAG(AG1);
  }
  if (!AG1_switch_state && (currentActionStatus )) {//& is_action_activated(AG1))) {
    mySimpit.printToKSP("Desactivate Action Group 1!");
    mySimpit.deactivateCAG(AG1);
  }


  // Action Group 2

  if (AG2_switch_state && !(currentActionStatus )) {//& is_action_activated(AG2))) {
    mySimpit.printToKSP("Activate Action Group 2!");
    mySimpit.activateCAG(AG2);
  }
  if (!AG2_switch_state && (currentActionStatus )) {//& is_action_activated(AG2))) {
    mySimpit.printToKSP("Desactivate Action Group 2!");
    mySimpit.deactivateCAG(AG2);
  }


  // Action Group 3

  if (AG3_switch_state && !(currentActionStatus )) {//& is_action_activated(AG3))) {
    mySimpit.printToKSP("Activate Action Group 3!");
    mySimpit.activateCAG(AG3);
  }
  if (!AG3_switch_state && (currentActionStatus )) {//& is_action_activated(AG3))) {
    mySimpit.printToKSP("Desactivate Action Group 3!");
    mySimpit.deactivateCAG(AG3);
  }


  // Action Group 4

  if (AG4_switch_state && !(currentActionStatus )) {//& is_action_activated(AG4))) {
    mySimpit.printToKSP("Activate Action Group 4!");
    mySimpit.activateCAG(AG4);
  }
  if (!AG4_switch_state && (currentActionStatus )) {//& is_action_activated(AG4))) {
    mySimpit.printToKSP("Desactivate Action Group 4!");
    mySimpit.deactivateCAG(AG4);
  }

  // Action Group 5

  if (AG5_switch_state && !(currentActionStatus )) {//& is_action_activated(AG5))) {
    mySimpit.printToKSP("Activate Action Group 5!");
    mySimpit.activateCAG(AG5);
  }
  if (!AG5_switch_state && (currentActionStatus )) {//& is_action_activated(AG5))) {
    mySimpit.printToKSP("Desactivate Action Group 5!");
    mySimpit.deactivateCAG(AG5);
  }
}


// Functions ====================================================================================================

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch (messageType) {
    case ACTIONSTATUS_MESSAGE:

      // Checking if the message is the size we expect is a very basic
      // way to confirm if the message was received properly.

      if (msgSize == 1) {
        currentActionStatus = msg[0];

        //Let the LED_BUILIN match the current RCS state
        if(currentActionStatus & RCS_ACTION){
          digitalWrite(LED_BUILTIN, HIGH);
        } else {
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
      break;
  }
}
