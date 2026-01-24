#include "physics.h"

// ===== Pan axis variables =====
float pan_current_angle      = 0.0f;
float pan_target_angle       = 0.0f;
float PAN_ANGLE_MIN          = 0.0f;
float PAN_ANGLE_MAX          = 290.0f;
 
float pan_current_speed      = 0.0f;
float pan_target_speed       = 0.0f;
float PAN_SPEED_MIN          = 0.0f;
float PAN_SPEED_MAX          = 200; 

float pan_current_acceleration = 0.0f;
float pan_target_acceleration  = 0.0f;
float PAN_ACCELERATION_MIN     = 0.0f;
float PAN_ACCELERATION_MAX     = 50.0f;

float pan_current_voltage      = 0.0f;

// ===== Tilt axis variables =====
float tilt_current_angle     = 0.0f;
float tilt_target_angle      = 0.0f;
float TILT_ANGLE_MIN         = 195.0f;
float TILT_ANGLE_MAX         = 250.0f;

float tilt_current_speed     = 0.0f;
float tilt_target_speed      = 0.0f;
float TILT_SPEED_MIN         = 0.0f;
float TILT_SPEED_MAX         = 3400.0f / 68.0f; // ≈50 RPM

float tilt_current_acceleration = 0.0f;
float tilt_target_acceleration  = 0.0f;
float TILT_ACCELERATION_MIN     = 0.0f;
float TILT_ACCELERATION_MAX     = 50.0f; // step/s²

float tilt_current_voltage      = 0.0f;

float lseres_focus_angle        = 0.0f;
float FOCUS_ANGLE_MIN           = 0.0f;
float FOCUS_ANGLE_MAX           = 180.0f;
float lasers_stepper_ratio      = 110/20.0f;
