/* button.ino
 * ~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * Description
 * ~~~~~~~~~~~
 * Turn analog input for button resistor ladder into a "buttonIndex".
 * When the button is pressed, then fire an associated event.
 *
 * To Do
 * ~~~~~
 * - Support for Freetronics LCD/Button shield.
 * - Move this code into Aiko as AikoDeviceButtonLadder.h and .cpp.
 *   - Constructor is passed the "buttonEventTable[]" as a parameter.
 * - Holding button(s) down causes repetitive event generation.
 * - Holding button(s) down for a while performs some action, e.g preferences.
 */

struct buttonEventType {
  int  lowWaterMark;
  int  highWaterMark;
  byte buttonMask;
  void (*handler)(void);
};

// Analog input button voltage ladder values for Pebble V1 shield

static const struct buttonEventType buttonEventTable[] = {
  1023, 1023, 0x00, NULL,                 // No button,              1023
   802,  822, 0x04, thermostatSetMode,    // Left button,             812
   688,  708, 0x02, NULL,                 // Middle button,           698
   582,  602, 0x06, NULL,                 // Left + Middle buttons,   592
   497,  517, 0x01, thermostatSetSPMode,  // Right button,            507
   439,  459, 0x05, NULL,                 // Left + Right buttons,    449
   402,  422, 0x03, NULL,                 // Middle + Right buttons,  412
   362,  382, 0x07, relayToggle1          // All buttons,             372
};

static const byte BUTTON_EVENT_COUNT =
  sizeof(buttonEventTable)/sizeof(buttonEventType);

// Handlers for managing press and release for each button

struct buttonStateType {
  void (*handler)(byte);
};

static const struct buttonStateType buttonStateTable[] = {
  leftButtonHandler, middleButtonHandler, rightButtonHandler
};

byte buttonIndex = 0;                         // Current button(s) pressed or 0

byte buttonInitialized0 = false;

void buttonInitialize0(void) {
  if (! buttonInitialized0) {
    digitalWrite(PIN_BUTTONS, LOW);           // Ensure internal pull-up is off

    buttonInitialized0 = true;
  }
}

void buttonHandler0(void) {
  if (! buttonInitialized0) buttonInitialize0();

  int  value = analogRead(PIN_BUTTONS);
  byte newIndex = 0;

  for (byte index = 0;  index < BUTTON_EVENT_COUNT;  index ++) {
    if (buttonEventTable[index].lowWaterMark  <= value  &&
        buttonEventTable[index].highWaterMark >= value) {

      newIndex = index;
    }
  }

  if (newIndex != buttonIndex) {
    byte oldMask = buttonEventTable[buttonIndex].buttonMask;
    buttonIndex = newIndex;
    byte buttonMask = buttonEventTable[buttonIndex].buttonMask;

    if (buttonIndex > 0 && buttonEventTable[buttonIndex].handler != NULL) {
      (* buttonEventTable[buttonIndex].handler)();
    }

    for (byte index = 0;  index < 3;  index ++) {
      byte oldState = oldMask    & 0x04;
      byte newState = buttonMask & 0x04;

      if (oldState != newState) {
        publishBoolean("button", index, newState);
        if (buttonStateTable[index].handler != NULL) {
          (* buttonStateTable[index].handler)(newState > 0);
        }
      }

      oldMask    <<= 1;
      buttonMask <<= 1;
    }
  }
}

void leftButtonHandler(
  byte buttonState) {
}

void middleButtonHandler(
  byte buttonState) {
}

void rightButtonHandler(
  byte buttonState) {
}

/* ------------------------------------------------------------------------- */
