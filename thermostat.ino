/* thermostat.ino
 * ~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * Version: 0.0
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * - Incorporate PID control: http://arduino.cc/playground/Code/PIDLibrary
 */

static const byte HEATER_OFF = 0;
static const byte HEATER_ON  = 1;

static const byte TEMPERATURE_OKAY = 0;
static const byte TEMPERATURE_LOW  = 1;
static const byte TEMPERATURE_HIGH = 2;

byte          dutyCycle = 0;
unsigned long dutyCycleCounter[2];

unsigned long heaterCounter[2] = { 0, 0 };
byte          heaterState = HEATER_OFF;            // heaterCounter[heaterIndex]

static byte lcdBlinkMap[] = {             // lcdBlinkMap[thermostatSetPointMode]
  LCD_BLINK_NONE, LCD_BLINK_SET_POINT_LOW, LCD_BLINK_SET_POINT_HIGH
};

int setPointLow;                                // fixed-point, 2 decimal places
int setPointHigh;                               // ditto

byte thermostatMode         = THERMOSTAT_OFF;
byte thermostatSetPointMode = THERMOSTAT_SETPOINT_MODE_OFF;

unsigned long thermostatAutoCounter = 0;

static char* THERMOSTAT_RESULT[] = { "okay", "low", "high" };

byte thermostatInitialized = false;

void thermostatInitialize(void) {
  if (! thermostatInitialized) {
    thermostatInitialized = true;
  }
}

void thermostatCounter(void) {
  heaterCounter[heaterState] ++;

  if (thermostatMode == THERMOSTAT_AUTOMATIC) {
    dutyCycleCounter[heaterState] ++;
    thermostatAutoCounter ++;
  }
}

void thermostatHandler(void) {
  if (! thermostatInitialized) thermostatInitialize();

  byte          result;
  unsigned long total;

  switch (thermostatMode) {
    case THERMOSTAT_OFF:
      heaterOff();
      break;

    case THERMOSTAT_AUTOMATIC:
      total = dutyCycleCounter[0] + dutyCycleCounter[1];

      if (total > 0) dutyCycle = (dutyCycleCounter[1] * 100) / total;

      if (temperatureDS1820Value < setPointLow)  heaterOn();
      if (temperatureDS1820Value > setPointHigh) heaterOff();

      publishMessage("thermostat", THERMOSTAT_RESULT[result], NULL);
      break;

    case THERMOSTAT_MANUAL:
      heaterOn();
      break;
  }
}

void thermostatSetMode(void) {
  dutyCycle           = 0;
  dutyCycleCounter[0] = 0;
  dutyCycleCounter[1] = 0;

  thermostatSetPointMode = THERMOSTAT_SETPOINT_MODE_OFF;

  if (thermostatMode == THERMOSTAT_AUTOMATIC  &&  thermostatAutoCounter >= 5) {
    thermostatMode = THERMOSTAT_OFF;
  }
  else {
    thermostatMode = (thermostatMode + 1) % (THERMOSTAT_MANUAL + 1);
  }

  thermostatHandler();
  thermostatAutoCounter = 0;
}

void thermostatSetSPMode(void) {
  if (thermostatMode == THERMOSTAT_AUTOMATIC) {
    thermostatSetPointMode =
      (thermostatSetPointMode + 1) % (THERMOSTAT_SETPOINT_MODE_HIGH + 1);

    lcdBlink(lcdBlinkMap[thermostatSetPointMode]);

    if (thermostatSetPointMode == THERMOSTAT_SETPOINT_MODE_OFF) eepromStore();
  }
}

void heaterOff(void) {
  heaterState = HEATER_OFF;

  if (relayTable[0].state == true) relayToggle0();
}

void heaterOn(void) {
  heaterState = HEATER_ON;

  if (relayTable[0].state == false) relayToggle0();
}

/* ------------------------------------------------------------------------- */
