/* relay.ino
 * ~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * - Move this code into Aiko as AikoDeviceRelay.h and .cpp.
 */

static char *RELAY_STRING = "relay";                            // ToDo: PROGMEM

static const byte PIN_RELAY_0 = 6;
static const byte PIN_RELAY_1 = 7;     // LCD backlight on some modified Pebbles

byte relayInitialized = false;

struct relayType relayTable[] = {
  PIN_RELAY_0, false,
  PIN_RELAY_1, false
};

void relayInitialize(void) {
  pinMode(PIN_RELAY_0, OUTPUT);
  pinMode(PIN_RELAY_1, OUTPUT);

  relayToggle(1);  // Ensure LCD backlight is on for modified Pebbles

  relayInitialized = true;
}

void relayToggle(
  byte index) {

  relayType *relay = & relayTable[index];

  (*relay).state = ! (*relay).state;
  digitalWrite((*relay).pin, (*relay).state);
  publishBoolean(RELAY_STRING, index, (*relay).state);
}

void relayToggle0(void) {
  relayToggle(0);
}

void relayToggle1(void) {
  relayToggle(1);
}

/* ------------------------------------------------------------------------- */
