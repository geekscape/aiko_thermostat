/* utility.ino
 * ~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * Version: 0.0
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * - Refactor leading zero function, if parameter less than 10, then return "0".
 * - Move dateString() and timeString() into Aiko (with or without PString) ?
 *   - Date will alternate with "Aiko v0.0" identification string.
 * - Move temperatureString() into Aiko (with or without PString) ?
 */

static const char *AIKO_TITLE_SHORT = "Aiko v0.0";  // ToDo: PROGMEM

char *booleanString(
  const char* prefix,
  byte  state) {

  globalString.begin();
  globalString  = prefix;
  globalString += state ? "On " : "Off";
  return(globalBuffer);
}

char *dateString() {
  globalString.begin();
  globalString = AIKO_TITLE_SHORT;  // "DDD dd-mm"
  return(globalBuffer);
}

char *numberString(
  const char *prefix,
  int         value,
  const char *suffix) {

  globalString.begin();
  globalString = prefix;
  if (value < 100) globalString += " ";
  if (value <  10) globalString += " ";
  globalString += value;
  globalString += suffix;
  return(globalBuffer);
}

char *timeString(
  unsigned long counter,
  boolean       dayFlag,
  boolean       secondFlag) {

  byte second =  counter          % 60;
  byte minute = (counter /    60) % 60;
  byte hour   = (counter /  3600) % 24;
  int  day    = (counter / 86400);

  globalString.begin();

  if (dayFlag) {
    if (day < 100) globalString += " ";
    if (day <  10) globalString += " ";
    globalString += day;
    globalString += " ";
  }

  if (hour < 10) globalString += "0";
  globalString += hour;
  globalString += ":";
  if (minute < 10) globalString += "0";
  globalString += minute;

  if (secondFlag) {
    globalString += ":";
    if (second < 10) globalString += "0";
    globalString += second;
  }

  return(globalBuffer);
}

char *temperatureString(
  const char* prefix,
  const char* suffix,
  int         value) {  // fixed-point, 2 decimal places

  globalString.begin();
  globalString = prefix;

  globalString += value / 100;
  globalString += ".";
  byte fraction = value % 100;
  if (fraction < 10) globalString += "0";
  globalString += fraction;

  globalString += suffix;
  return(globalBuffer);
}

/* ------------------------------------------------------------------------- */
