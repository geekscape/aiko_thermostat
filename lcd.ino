/* lcd.ino
 * ~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * Version: 0.0
 * ----------------------------------------------------------------------------
 * Description
 * ~~~~~~~~~~~
 * Custom character bit map generator: http://www.quinapalus.com/hd44780udg.html
 *
 *    +----------------------+    +----------------------+
 * 0: | DDD MM-DD HH:MM:SS / |    | Sun 25-12 12:34:56 / |
 * 1: | Lo:  TT.TT Hi: TT.TT |    | Lo:  19.50 Hi: 21.00 |
 * 2: | Temp TT.TT C Lux LLL |    | Temp 20.12 C Lux 314 |
 * 3: | Heater is SSS [MMMM] |    | Heater is Off [Auto] |
 *    +----------------------+    +----------------------+
 *
 * 1: | On:   DDD HH:MM:SS   |    Cycle total time for heater off and on
 * 3: | Heater is Off        |    Heater always off
 *
 * 1: | Lo:  TT.TT Hi: TT.TT |    Show thermostat set-points
 * 3: | Heater is Off [Auto] |    Thermostat enabled, heater off
 *
 * 1: | Duty cycle: CCC%     |    Show thermostat duty cycle
 * 3: | Heater is On  [Auto] |    Thermostat enabled, heater on
 *
 * 1: | Off:  DDD HH:MM:SS   |    Cycle total time for heater off and on
 * 3: | Heater is On         |    Heater always on
 *
 * To Do
 * ~~~~~
 * - Support for Freetronics 2 row x 16 column LCD screen.
 */

static char *DUTY1_STRING   = "Duty cycle: ";                   // ToDo: PROGMEM
static char *HEATER_STRING  = "Heater is ";                     // ToDo: PROGMEM
static char *REFRESH_STRING = "Refresh: ";                      // ToDo: PROGMEM
static char *TEMP1_STRING   = "Temp ";                          // ToDo: PROGMEM

static char *DISPLAY_MODE[]    = { "Off:  ",  "On:   "  };      // ToDo: PROGMEM
static char *THERMOSTAT_MODE[] = { "       ", " [Auto]" };      // ToDo: PROGMEM

static const char bitmapDegree[8] = {
  0x02, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00  // small
//0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00  // large
};

byte          displayBlinkField  = LCD_BLINK_NONE;
unsigned long displayCounter     = 0;
byte          displayMode        = 0;                // Duty cycle or Set point
byte          displayRefresh     = 1;                // For removing "Refresh=?"
byte          displayRefreshFlag = false;
byte          displayRefreshOld  = displayRefresh;
byte          displayTick        = 0;                // For blinking

void lcdHandler(void) {
  if (lcd4096Initialized == false) {
    lcd4096Initialize();
    lcdCreateCustomCharacter(6, bitmapDegree);
    lcdClear();
  }

  displayTick = (displayTick + 1) % 4;

  if (secondCounter > displayCounter) {
    displayCounter     = secondCounter + displayRefresh;
    displayMode        = (displayMode + 1) % 2;
    displayRefreshFlag = false;
  }

  if (displayRefresh != displayRefreshOld) {
    displayRefreshOld  = displayRefresh;
    displayRefreshFlag = true;
  }

  lcdPosition(0, 0);
  lcdWriteString(dateString());
  lcdPosition(0, 10);
  lcdWriteString(timeString(secondCounter, false, true));

  lcdPosition(1, 0);
  if (thermostatMode == THERMOSTAT_AUTOMATIC) {
    if (displayBlinkField != LCD_BLINK_NONE  ||  displayMode == 0) {
      char *displayString = "           ";

      if (displayBlinkField != LCD_BLINK_SET_POINT_LOW  ||  displayTick > 0) {
        displayString = temperatureString("Lo:  ", " ", setPointLow);
      }
      lcdWriteString(displayString);

      displayString = "         ";

      if (displayBlinkField != LCD_BLINK_SET_POINT_HIGH  ||  displayTick > 0) {
        displayString = temperatureString("Hi: ", "", setPointHigh);
      }
      lcdWriteString(displayString);
    }
    else {
      lcdWriteString(numberString(DUTY1_STRING, dutyCycle, "%    "));
    }
  }
  else {
    lcdWriteString(DISPLAY_MODE[displayMode]);
    lcdWriteString(timeString(heaterCounter[displayMode], true, true));
    lcdWriteString("  ");
  }

  if (temperatureDS1820Ready) {
    lcdPosition(2, 0);
    lcdWriteString(
      temperatureString(TEMP1_STRING, "\6C  ", temperatureDS1820Value)
    );
  }

  if (luxValue < 1000) {            // Don't overflow limited screen real-estate
    lcdPosition(2, 13);
    lcdWriteString(numberString("Lux ", luxValue, ""));
  }

  lcdPosition(3, 0);
  if (displayRefreshFlag) {
    globalString.begin();
    globalString  = REFRESH_STRING;
    globalString += displayRefresh;
    globalString += "   ";
    lcdWriteString(globalBuffer);
  }
  else {
    lcdWriteString(booleanString(HEATER_STRING, relayTable[0].state));
  }
  lcdWriteString(THERMOSTAT_MODE[thermostatMode == THERMOSTAT_AUTOMATIC]);
}

void lcdBlink(
  byte blinkField) {

  displayBlinkField = blinkField;
}

/* ------------------------------------------------------------------------- */
