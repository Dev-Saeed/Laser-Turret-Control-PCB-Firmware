#ifndef PHYSICS_H
#define PHYSICS_H

// Pan axis (DYNAMIXEL XC430-T150BB-T)
extern float pan_current_angle;
extern float pan_target_angle;
extern float PAN_ANGLE_MIN;
extern float PAN_ANGLE_MAX;

extern float pan_current_speed;
extern float pan_target_speed;
extern float PAN_SPEED_MIN;
extern float PAN_SPEED_MAX;

extern float pan_current_acceleration;
extern float pan_target_acceleration;
extern float PAN_ACCELERATION_MIN;
extern float PAN_ACCELERATION_MAX;

extern float pan_current_voltage;

// Tilt axis (WAVESHARE ST3025)
extern float tilt_current_angle;
extern float tilt_target_angle;
extern float TILT_ANGLE_MIN;
extern float TILT_ANGLE_MAX;

extern float tilt_current_speed;
extern float tilt_target_speed;
extern float TILT_SPEED_MIN;
extern float TILT_SPEED_MAX;

extern float tilt_current_acceleration;
extern float tilt_target_acceleration;
extern float TILT_ACCELERATION_MIN;
extern float TILT_ACCELERATION_MAX;

extern float tilt_current_voltage;

extern float FOCUS_ANGLE_MIN;
extern float FOCUS_ANGLE_MAX;
extern float lasers_stepper_ratio;
extern float lseres_focus_angle;

#endif // PHYSICS_H
