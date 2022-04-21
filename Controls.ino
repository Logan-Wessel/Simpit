// Main file for all control functions


// Throttle lever ====================================================================================================

void ThrottleHandler {

  // Throttle reading and input
  // Send at each loop a message to control the throttle and the pitch/roll axis.

  throttleMessage Throttle;

  // Read the value of the potentiometer

  int reading = analogRead(Throttle);

  // Convert it in KerbalSimpit Range

  throttle_msg.throttle = map(reading, 0, 1023, 0, INT16_MAX);

  // Send the message

  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
}


// SAS Switches ====================================================================================================

void SASHandler {

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

  void RCSHandler {

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
  }


  // Stage Button ====================================================================================================

  void StageHandler {

    // Read the state of the switch into a local variable.

    if (digitalRead(Ignition)) {
      int readingIngnition = digitalRead(Ignition);
    }
    else if (digitalRead(Stage)) {
      int readingIngnition = digitalRead(Stage);
    }

    // If the switch changed, due to noise or pressing:

    if (readingStage != lastButtonStateStage) {

      // reset the debouncing timer

      lastDebounceTimeIgnitnion = millis();
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
  }


  // Abort Button ====================================================================================================

  void AbortHandler {

    // Read the state of the switch into a local variable.

    int readingABort = digitalRead(Abort);

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
  }


  // Gear Switch ====================================================================================================

  void GearHandler {

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
  }


  // Lights Switch ====================================================================================================

  void LightsHandler {

    // Get the switch state

    bool lights_switch_state = digitalRead(Lights);

    // Update the switch to match the state, only if a change is needed to avoid spamming commands.

    if (lights_switch_state && !(currentActionStatus & LIGHTS_ACTION)) {
      mySimpit.printToKSP("Activate Lights!");
      mySimpit.activateAction(LIGHTS_ACTION);
    }
    if (!lights_switch_state && (currentActionStatus & LIGHTS_ACTION)) {
      mySimpit.printToKSP("Desactivate Lights!");
      mySimpit.deactivateAction(LIGHTS_ACTION);
    }
  }


  // Brakes Switch ====================================================================================================

  void BrakesHandler {

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
  }


  // ACtion Group Switch ====================================================================================================

  void ActionGroupsHandler {

    // Get the switch state

    bool AG1_switch_state = digitalRead(AG1);
    bool AG2_switch_state = digitalRead(AG2);
    bool AG3_switch_state = digitalRead(AG3);
    bool AG4_switch_state = digitalRead(AG4);
    bool AG5_switch_state = digitalRead(AG5);

    // Update the switch to match the state, only if a change is needed to avoid spamming commands.


    // Action Group 1

    if (AG1_switch_state && !(currentActionStatus & is_action_activated(AG1))) {
      mySimpit.printToKSP("Activate Action Group 1!");
      mySimpit.activateCAG(AG1);
    }
    if (!AG1_switch_state && (currentActionStatus & is_action_activated(AG1))) {
      mySimpit.printToKSP("Desactivate Action Group 1!");
      mySimpit.deactivateCAG(AG1);
    }


    // Action Group 2

    if (AG2_switch_state && !(currentActionStatus & is_action_activated(AG2))) {
      mySimpit.printToKSP("Activate Action Group 2!");
      mySimpit.activateCAG(AG2);
    }
    if (!AG2_switch_state && (currentActionStatus & is_action_activated(AG2))) {
      mySimpit.printToKSP("Desactivate Action Group 2!");
      mySimpit.deactivateCAG(AG2);
    }


    // Action Group 3

    if (AG3_switch_state && !(currentActionStatus & is_action_activated(AG3))) {
      mySimpit.printToKSP("Activate Action Group 3!");
      mySimpit.activateCAG(AG3);
    }
    if (!AG3_switch_state && (currentActionStatus & is_action_activated(AG3))) {
      mySimpit.printToKSP("Desactivate Action Group 3!");
      mySimpit.deactivateCAG(AG3);
    }


    // Action Group 4

    if (AG4_switch_state && !(currentActionStatus & is_action_activated(AG4))) {
      mySimpit.printToKSP("Activate Action Group 4!");
      mySimpit.activateCAG(AG4);
    }
    if (!AG4_switch_state && (currentActionStatus & is_action_activated(AG4))) {
      mySimpit.printToKSP("Desactivate Action Group 4!");
      mySimpit.deactivateCAG(AG4);
    }

    // Action Group 5

    if (AG5_switch_state && !(currentActionStatus & is_action_activated(AG5))) {
      mySimpit.printToKSP("Activate Action Group 5!");
      mySimpit.activateCAG(AG5);
    }
    if (!AG5_switch_state && (currentActionStatus & is_action_activated(AG5))) {
      mySimpit.printToKSP("Desactivate Action Group 5!");
      mySimpit.deactivateCAG(AG5);
    }
  }
