#ifndef NEMA_DRIVER_H
#define NEMA_DRIVER_H

#include <Arduino.h>
#include <AccelStepper.h>
#include "physics.h"
#include "TurretConfig.h"

// Initialize stepper driver (pins, limits, etc.)
void init_NEMA(int enPin, int dirPin, int stepPin, int comPin, int limitPin, float stepsPerRev);

// Perform homing with limit switch
void Pan_home();

// Move to a specific pan angle (in degrees)
void Pan_goTo(float panAngleDeg);

// Calibrate the pan stepper 
void Pan_Calibrate();

// Update stepper every loop (non-blocking), with desired rpm and angle limits
void nema_pan_speed_control(float rpmCommand, float minAngle, float maxAngle);

// Get current angle and velocity
float pan_getAngle();
float pan_getVelocity();

#endif // NEMA_DRIVER_H
