#include "KerbalSimpit.h"

// Set the pin numbers:
const int Throttle = A0;
const int ActionGroup3 = 58;
const int ActionGroup4 = 59;
const int ActionGroup5 = 60;
const int RollRightCop = 22;
const int RollLeftCop = 23;
const int PitchUpCop = 24;
const int PitchDownCop = 25;
const int YawRightCop = 26;
const int YawLeftCop = 27;
const int SasCop = 28;
const int ActionGroup1 = 29;
const int Lights = 30;
const int Breaks = 31;
const int Gear = 32;
const int RCS = 33;
const int RCSDirL = 34;
const int RCSDirR = 35;
const int RCSDirF = 36;
const int RCSDirB = 37;
const int RCSDirU = 38;
const int RCSDirD = 39;
const int Abord= 40;
const int Ignition= 41;
const int AllStageSwitches= 42;
const int ActionGroup2= 43;
const int RollRightP = 47;
const int RollLeftP = 48;
const int PitchUpP = 49;
const int PitchDownP = 50;
const int YawRightP = 51;
const int YawLeftP = 52;
const int SasO = 53; 

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }  
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
}

void loop() {
  // Send at each loop a message to control the throttle and the pitch/roll axis.
  
  throttleMessage Throttle;
  // Read the value of the potentiometer
  int reading = analogRead(Throttle);
  // Convert it in KerbalSimpit Range
  throttle_msg.throttle = map(reading, 0, 1023, 0, INT16_MAX);
  // Send the message
  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
}
