#ifndef ST_DRIVER_H
#define ST_DRIVER_H

#include <Arduino.h>
#include <SCServo.h>
#include "physics.h"
#include "TurretConfig.h"

// Initialize the DYNAMIXEL servo (XC430-T150BB-T)
void init_ST(HardwareSerial &serialPort, uint32_t baud, int rxPin, int txPin);

// Perform homing
void Tilt_home();

// Calibrate the tilt servo
void Tilt_Calibrate();

// Update servo speed control with desired RPM and angle limits
void st_tilt_speed_control(float rpm, float min_angle, float max_angle);

// Get current angle, velocity, and voltage
float tilt_getAngle();
float tilt_getVelocity();
float tilt_getVoltage();

// Control functions
void st_go_to(float angle_deg, int rpm);
void st_speed_to(float rpm);
void st_OP_POSITION();
void st_OP_VELOCITY();

// Register read/write functions
void st_info();
void st_writeWord_eeprom(u8 ID, u8 MemAddr, u16 wDat);
void st_writeByte_eeprom(u8 ID, u8 MemAddr, u8 bDat);
void st_resetAngle();
void st_read_eeprom();
void test_all_phase_values();
void st_reset();
#endif // ST_DRIVER_H