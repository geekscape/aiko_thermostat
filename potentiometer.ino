/* potentiometer.ino
 * ~~~~~~~~~~~~~~~~~
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
 * - None, yet.
 */

/* ------------------------------------------------------------------------- */

int potentiometerValue0 = 0;

void potentiometer0Handler(void) {
  potentiometerValue0 = 1023 - analogRead(PIN_POTENTIOMETER);

  if (thermostatSetPointMode == THERMOSTAT_SETPOINT_MODE_OFF) {
    displayRefresh = potentiometerValue0 / 114 + 1;           // Result: 1 to 9
  }

  int value = potentiometerValue0 / 61;  // Result: 0 to 16
  int newSetPoint = value * 50 + 1600;   // 1600 to 2400 fixed-point, 2 decimals

  if (thermostatSetPointMode == THERMOSTAT_SETPOINT_MODE_LOW) {
    if (newSetPoint >= setPointHigh  &&  newSetPoint <= 2350) {
      setPointHigh = newSetPoint + 50;
    }

    if (newSetPoint < setPointHigh) setPointLow = newSetPoint;
  }

  if (thermostatSetPointMode == THERMOSTAT_SETPOINT_MODE_HIGH) {
    if (newSetPoint <= setPointLow) newSetPoint = setPointLow + 50;

    setPointHigh = newSetPoint;
  }
}

/* ------------------------------------------------------------------------- */
