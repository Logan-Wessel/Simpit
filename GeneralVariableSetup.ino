// Initial setup for values used later for controls and stuff like that


byte currentActionStatus = 0;
//int buttonState; // the current reading from the input pin
int buttonStateIgnition; // the current reading from the input pin
int buttonStateAbort; // the current reading from the input pin

// the following variables are unsigned long's because the time, measured
// in miliseconds, will quickly become a bigger number than can be stored
// in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin
                                     // was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase
                                     // if the output flickers
