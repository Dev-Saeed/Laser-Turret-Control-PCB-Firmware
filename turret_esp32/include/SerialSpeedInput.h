#ifndef SERIAL_SPEED_INPUT_H
#define SERIAL_SPEED_INPUT_H

#include <Arduino.h>
#include "TurretConfig.h"  // must define DEBUG_SERIAL, DEBUG_RX, DEBUG_TX

// Simple line buffer for "pan,tilt" input from the serial monitor
static char _dbgBuf[64];
static size_t _dbgIdx = 0;

/**
 * Non-blocking reader for lines like: "pan,tilt"
 * Example: "2.4,-1.6" → pan_target_speed=2.4 , tilt_target_speed=-1.6
 * Call from loop() before applying motor control.
 */
static inline void handleSerialSpeeds(float &pan_target_speed, float &tilt_target_speed) {
  while (DEBUG_SERIAL.available() > 0) {
    char c = (char)DEBUG_SERIAL.read();

    if (c == '\r') continue;          // ignore CR

    if (c == '\n') {                   // end of line → parse
      _dbgBuf[_dbgIdx] = '\0';

      float panSpd, tiltSpd;
      // Accepts optional spaces around comma
      int n = sscanf(_dbgBuf, " %f , %f ", &panSpd, &tiltSpd);
      if (n == 2) {
        pan_target_speed  = panSpd;
        tilt_target_speed = tiltSpd;

        DEBUG_SERIAL.print(F("[OK] pan="));
        DEBUG_SERIAL.print(pan_target_speed, 3);
        DEBUG_SERIAL.print(F(", tilt="));
        DEBUG_SERIAL.println(tilt_target_speed, 3);
      } else {
        DEBUG_SERIAL.print(F("[ERR] expected: pan,tilt  e.g.  2.4,-1.6  | got: "));
        DEBUG_SERIAL.println(_dbgBuf);
      }

      // reset for next line
      _dbgIdx = 0;
    } else {
      // accumulate until newline; drop line if overflow
      if (_dbgIdx < sizeof(_dbgBuf) - 1) {
        _dbgBuf[_dbgIdx++] = c;
      } else {
        _dbgIdx = 0;  // overflow → reset buffer
      }
    }
  }
}

#endif // SERIAL_SPEED_INPUT_H
