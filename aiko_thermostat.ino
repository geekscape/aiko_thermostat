/* aiko_thermostat.ino
 * ~~~~~~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * Version: 0.0
 *
 * Documentation
 * ~~~~~~~~~~~~~
 * Software: http://geekscape.github.com/aiko_thermostat
 * Hardware: https://github.com/lukeweston/Pebble
 * ----------------------------------------------------------------------------
 *
 * Third-Party libraries
 * ~~~~~~~~~~~~~~~~~~~~~
 * These libraries are not included in the Arduino IDE and
 * need to be downloaded and installed separately.
 *
 * - Aiko Arduino library
 *   https://github.com/geekscape/aiko_arduino
 *
 * - OneWire library
 *   http://www.pjrc.com/teensy/td_libs_OneWire.html  # Version 2.1 or newer
 *
 * - PString library
 *   http://arduiniana.org/libraries/pstring
 *
 * To Do
 * ~~~~~
 * - None, yet.
 */

#include <OneWire.h>  // Required by Aiko library
#include <PString.h>

#include <AikoCommands.h>
#include <AikoDevices.h>
#include <AikoEvents.h>

using namespace Aiko;
using namespace Command;
using namespace Device;

#include "aiko_thermostat.h"

void setup(void) {
  eepromInitialize();
  relayInitialize();

  Events.addHandler(buttonHandler0,             100);
  Events.addHandler(thermostatCounter,         1000);
  Events.addHandler(clockHandler,              1000);
  Events.addHandler(lcdHandler,                 100);
  Events.addHandler(luxHandler,                1000);
  Events.addHandler(messageHandler,           10000);
  Events.addHandler(potentiometer0Handler,      100);
  Events.addHandler(serialHandler,              100);
  Events.addHandler(temperatureDS1820Handler,  1000);
  Events.addHandler(thermostatHandler,        10000);
  Events.addHandler(throbberHandler,            250);

  throbberEnabled = true;     // Remove when throbber controlled by comms state
}

void loop(void) {
  Events.loop();
}

/* ------------------------------------------------------------------------- */
