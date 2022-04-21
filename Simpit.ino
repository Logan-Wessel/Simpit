// Main file for Simpit Arduino -> Simpit KSP communication

#include "KerbalSimpit.h"


// Set the pin numbers


// Analog

const int Throttle = A0;

// Shared

const int Stage = 42;

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
const int SasCop = 28;

// Pilot Controls

const int RollRightP = 47;
const int RollLeftP = 48;
const int PitchUpP = 49;
const int PitchDownP = 50;
const int YawRightP = 51;
const int YawLeftP = 52;
const int SasP = 53;

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
const int Ignition = 41;




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
  ActionGroupsHandler();

}


// Functions ====================================================================================================

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch (messageType) {
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
