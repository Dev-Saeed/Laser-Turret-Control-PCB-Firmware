#ifndef STEPPER_FOCUS_DRIVER_H
#define STEPPER_FOCUS_DRIVER_H

#include <Arduino.h>

// Set this gear ratio from main
extern float focus_gear_ratio;

// Setup & homing
void init_BYJ(int s1, int s2, int s3, int s4);
void Focus_home();

// Motion control
void stepper_aim_angle_control(float target_angle, float angle_min, float angle_max);

#endif
