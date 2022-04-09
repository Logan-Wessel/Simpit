// Main file for Simpit Arduino -> Simpit KSP communication

#include "KerbalSimpit.h"


GeneralVariableSetup();

// Communication setup

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

// Runs once, to do things that only need to happen once right at the start
// but after the serial port is open


// Setup ====================================================================================================

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set up toggle switches with builtin pullup.
  pinMode(SasCop, INPUT_PULLUP);
  pinMode(SasP, INPUT_PULLUP);
  pinMode(RCS, INPUT_PULLUP);
  pinMode(Gear, INPUT_PULLUP);
  pinMode(Lights, INPUT_PULLUP);
  pinMode(Brakes, INPUT_PULLUP);
  pinMode(Ignition, INPUT_PULLUP);
  pinMode(Abort, INPUT_PULLUP);
  pinMode(Stage, INPUT_PULLUP);

  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }  
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

  // Calls functions to get input states

  ThrottleHandler();
  SASHandler();
  RCSHandler()
  StageHandler();
  AbortHandler();
  GearHnadler();
  BrakesHandler();
  LightsHandler();

}


// Functions ====================================================================================================

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  case ACTIONSTATUS_MESSAGE:
    // Checking if the message is the size we expect is a very basic
    // way to confirm if the message was received properly.
    if (msgSize == 1) {
      currentActionStatus = msg[0];

      //Let the LED_BUILIN match the current SAS state
      //if(currentActionStatus & SAS_ACTION){
      //  digitalWrite(LED_BUILTIN, HIGH);
      //} else {
      //  digitalWrite(LED_BUILTIN, LOW);
      //}
    }
    break;
  }
}
