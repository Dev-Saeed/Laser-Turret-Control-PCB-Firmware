#ifndef DXL_SERVO_H
#define DXL_SERVO_H

#include <Arduino.h>
#include "physics.h"
#include "TurretConfig.h"
#include <Dynamixel2Arduino.h>

void initDynamixel();

void dxl_speed_to(float rpm);
float get_user_pan_angle();
void dxl_go_to(int go_to);
void dxl_turn_to(int angle_deg);
void dxl_pan_speed_control(float rpm, float min_angle, float max_angle);
void scan_dynemixel();
void change_baudrate(int baudrate);
void dxl_OP_VELOCITY();
void dxl_OP_POSITION();
void dxl_info();
void dxl_all_info();

#endif