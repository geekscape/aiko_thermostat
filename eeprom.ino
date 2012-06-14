/* eeprom.ino
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
 * - None, yet.
 */

#include <avr/eeprom.h>
#include <EEPROM.h>  // Only used by commandWipe()

// EEPROM macros: http://projectgus.com/2010/07/eeprom-access-with-arduino

#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))

#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))

#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))

#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }

#define MIN(x,y) ( x > y ? y : x )

static const long EEPROM_MAGIC = 0xdada0000;      // Unique for this application

struct __eeprom_data {
  long magic;
  int  setPointLow;
  int  setPointHigh;
};

byte eepromInitialized = false;

void eepromInitialize(void) {
  long magic;

  if (! eepromInitialized) {
    eeprom_read(magic, magic);
    if (magic != EEPROM_MAGIC) eepromSetup();
    eepromRestore();

    eepromInitialized = true;
  }
}

void eepromRestore(void) {
  eeprom_read(setPointLow,  setPointLow);
  eeprom_read(setPointHigh, setPointHigh);
}

void eepromSetup(void) {
  eeprom_write(DEFAULT_SET_POINT_LOW,  setPointLow);
  eeprom_write(DEFAULT_SET_POINT_HIGH, setPointHigh);
  eeprom_write(EEPROM_MAGIC,           magic);
}

void eepromStore(void) {
  eeprom_write(setPointLow,  setPointLow);
  eeprom_write(setPointHigh, setPointHigh);
}

/* ------------------------------------------------------------------------- */
