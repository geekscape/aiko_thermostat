/* aiko_thermostat.h
 * ~~~~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * - Consider replacing PString with built-in Arduino IDE String ?
 * - Replace global variables with get/set function calls.
 */

// Allocate strings in Flash program memory to save RAM
// http://arduino.cc/en/Reference/PROGMEM

#include <avr/pgmspace.h>
#define PROGSTRING(name) static const prog_uchar name[] PROGMEM
// PROGSTRING(name) = { "value" };

// PString provides improved string manipulation

#define BUFFER_SIZE 64
char globalBuffer[BUFFER_SIZE];  // Store dynamically constructed strings
PString globalString(globalBuffer, sizeof(globalBuffer));

static const long DEFAULT_BAUD_RATE = 38400;

static const int DEFAULT_SET_POINT_LOW  = 1900;  // Fixed-point 2 decimal places
static const int DEFAULT_SET_POINT_HIGH = 2050;  // ditto

static const byte LCD_BLINK_NONE           = 0;
static const byte LCD_BLINK_SET_POINT_LOW  = 1;
static const byte LCD_BLINK_SET_POINT_HIGH = 2;

struct relayType {
  byte pin;
  byte state;
};

extern struct relayType relayTable[];

static const byte THERMOSTAT_OFF       = 0;
static const byte THERMOSTAT_AUTOMATIC = 1;
static const byte THERMOSTAT_MANUAL    = 2;

static const byte THERMOSTAT_SETPOINT_MODE_OFF  = 0;
static const byte THERMOSTAT_SETPOINT_MODE_LOW  = 1;
static const byte THERMOSTAT_SETPOINT_MODE_HIGH = 2;

extern byte dutyCycle;

extern unsigned long heaterCounter[2];

extern int potentiometerValue0;

extern int setPointLow;
extern int setPointHigh;

extern byte thermostatMode;
extern byte thermostatSetPointMode;

/* ------------------------------------------------------------------------- */
