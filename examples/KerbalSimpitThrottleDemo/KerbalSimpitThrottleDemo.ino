/* KerbalSimpitThrottleDemo

   A demonstration of how to use KerbalSimPit to send throttle 
   or rotation command.

   Assume A0, A1, A2 are plugged on a potentiometer. Each potentiometer will be used
   to control throttle, pitch and roll of you rocket.
   See these links for basic descriptions of how to hook up a potentiometer:
     - https://www.arduino.cc/en/tutorial/potentiometer
     - https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40/circuit-1b-potentiometer
*/

#include "KerbalSimpit.h"

// Set the pin numbers:
const int THROTTLE_PIN = A0; // the pin used for controlling throttle

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
  // Display a message in KSP to indicate handshaking is complete.
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
}

void loop() {
  // Send at each loop a message to control the throttle and the pitch/roll axis.
  
  throttleMessage throttle_msg;
  // Read the value of the potentiometer
  int reading = analogRead(THROTTLE_PIN);
  // Convert it in KerbalSimpit Range
  throttle_msg.throttle = map(reading, 0, 1023, 0, INT16_MAX);
  // Send the message
  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);

}
