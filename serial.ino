/* serial.ino
 * ~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * Description
 * ~~~~~~~~~~~
 * - None, yet.
 *
 * To Do
 * ~~~~~
 * - Get Serial.begin(DEFAULT_BAUD_RATE) from EEPROM.
 * - Command to wipe EEPROM.
 * - Move this code into Aiko as AikoCommandSerial.h and .cpp.
 */

/* ------------------------------------------------------------------------- */

static const char *AIKO_TITLE_LONG = "[aiko_thermostat 0.0]";  // ToDo: PROGMEM
static const char *FREE_RAM_STRING = "Free RAM: ";             // ToDo: PROGMEM

byte serialInitialized = false;

void serialInitialize(void) {
  if (! serialInitialized) {
    Serial.begin(DEFAULT_BAUD_RATE);
    serialWelcome();

    serialInitialized = true;
  }
}

char buffer[8];
byte bufferIndex = 0;

void serialHandler(void) {
  if (! serialInitialized) serialInitialize();

  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\r') {
      bufferIndex = 0;
    }
    else {
      buffer[bufferIndex ++] = ch;
      if (bufferIndex == 8) bufferIndex = 0;
      buffer[bufferIndex] = '\0';
    }
  }
}

void serialWelcome(void) {
  Serial.println(AIKO_TITLE_LONG);
  Serial.println(FREE_RAM_STRING);
  Serial.println(freeRAM());
}

/* ------------------------------------------------------------------------- */
