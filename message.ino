/* message.ino
 * ~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * Version: 0.0
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * - Publish heart-beat.
 * - Implement MQTT messages.
 * - Include node name and timestamp.
 */

static char *DUTY2_STRING = "dutyCycle";                        // ToDo: PROGMEM
static char *TEMP2_STRING = "temperature ";                     // ToDo: PROGMEM

static char *FALSE_STRING = "false";                            // ToDo: PROGMEM
static char *TRUE_STRING  = "true";                             // ToDo: PROGMEM

void messageHandler(void) {
  if (temperatureDS1820Ready) {
    publishMessage(
      temperatureString(TEMP2_STRING, " C", temperatureDS1820Value), NULL, NULL
    );

    if (thermostatMode == THERMOSTAT_AUTOMATIC) {
      publishInteger(DUTY2_STRING, dutyCycle, "%");
    }
  }

  publishInteger("lux", luxValue, NULL);
}

void publishBoolean(
  char *name,
  byte  identifier,
  byte  value) {

  publishInteger(name, identifier, (value  ?  TRUE_STRING  :  FALSE_STRING));
}

void publishInteger(
  char *value1,
  int   value2,
  char *value3) {

  char buffer[17];

  publishMessage(value1, itoa(value2, buffer, 10), value3);
}

void publishMessage(
  char *value1,
  char *value2,
  char *value3) {

  Serial.print("(");

  Serial.print(value1);

  if (value2 != NULL) {
    Serial.print(" ");
    Serial.print(value2);
  }

  if (value3 != NULL) {
    Serial.print(" ");
    Serial.print(value3);
  }

  Serial.println(")");
}

/* ------------------------------------------------------------------------- */
