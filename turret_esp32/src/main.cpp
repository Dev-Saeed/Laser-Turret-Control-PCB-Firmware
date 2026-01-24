#include <Arduino.h>
#include "JsonMessenger.h"
#include "st_driver.h"
#include "nema_driver.h"
#include "physics.h"
#include "TurretConfig.h"
#include "STM_Link.h"
#include "SerialSpeedInput.h"
// #include "stepper_focus_driver.h"

STM_Link stmLink;
JsonMessenger stationMessenger;

unsigned long lastRelaySendTime = 0;
const unsigned long relayIntervalMs = 5;

void setup() {
  setupConfig();

  DEBUG_SERIAL.begin(DEBUG_BAUDRATE, SERIAL_8N1, DEBUG_RX, DEBUG_TX);

  // === Communication Setup ===
  stmLink.beginUART(STM32_SERIAL, STM32_BAUDRATE, STM32_RX3, STM32_TX3);
  stationMessenger.begin(station_SERIAL, station_BAUDRATE, station_RX, station_TX);
  delay(200);

  // === Motor Setup ===
  init_ST(Servo_SERIAL, SERVO_BAUDRATE, S_RX, S_TX); // Tilt motor (servo)
  init_NEMA(EN_PIN, DIR_PIN, STEP_PIN, COM_PIN, LIMIT_PIN, STEPS_PER_REV); // Pan motor (stepper)
  delay(200);

  // Enable relays for motors
  stm_output[2] = 1; // Motor1 (Pan)
  stm_output[1] = 1; // Motor2 (Tilt)
  stmLink.sendRelayStates(stm_output);
  // init_BYJ(STEP1, STEP2, STEP3, STEP4);
  delay(1000);

  // === Homing Motors ===
  st_writeByte_eeprom(1, 18, 45); // Set phase value to 128
  Tilt_home();
  // st_reset();
  Pan_home();
  // Focus_home();
  delay(2000);
  delay(100);

  // st_OP_VELOCITY(); // Enable servo velocity control mode
  Pan_goTo(160);
  Pan_goTo(140);
}

void loop() {
  unsigned long t0 = micros();

  // === (1) Receive command from Control Station ===
  stationMessenger.receive();

  // === (2) Read limit switches from STM32 ===
  stmLink.readLimitStates(stm_input);

  // === (3) Parse and apply command ===
  if (stationMessenger.lastCommand.valid) {
    const ControlCommand &cmd = stationMessenger.lastCommand;

    Buzzer  = cmd.actuator.buzzer_on;
    Motor1  = cmd.actuator.pan_motor_on;
    Motor2  = cmd.actuator.tilt_motor_on;
    Laser1  = cmd.actuator.laser1_on;
    Laser2  = cmd.actuator.laser2_on;
    Laser3  = cmd.actuator.laser3_on;
    // Focus   = cmd.actuator.focus_on;

    pan_target_speed  = cmd.motion.pan_speed_rpm;
    tilt_target_speed = cmd.motion.tilt_speed_rpm;
    lseres_focus_angle = cmd.motion.laser_focus_angle;
  }

  // === (4) Update relay outputs ===
  stm_output[0] = Buzzer || stm_input[6];
  stm_output[2] = 1;       // Motor1 always enabled
  stm_output[1] = 1;       // Motor2 always enabled
  stm_output[3] = Laser1 || stm_input[6];
  stm_output[4] = Laser2;
  stm_output[5] = 1;
  stm_output[6] = stm_input[6];
  stm_output[7] = Laser3;

  if (!(pan_getAngle() > 125 && pan_getAngle() < 155)) {
  stm_output[0] = 0; // Buzzer
  stm_output[3] = 0; // Laser1
  stm_output[4] = 0; // Laser2
  // stm_output[5] = 0; // Laser3
  }

  if (millis() - lastRelaySendTime >= relayIntervalMs) {
  stmLink.sendRelayStates(stm_output);
  lastRelaySendTime = millis();
  } 
  digitalWrite(LED_BUILTIN, Laser3); // Debug LED

  // === (5) Apply motion control ===
  // handleSerialSpeeds(pan_target_speed, tilt_target_speed);
  nema_pan_speed_control(pan_target_speed, PAN_ANGLE_MIN, PAN_ANGLE_MAX);
  st_tilt_speed_control(tilt_target_speed, TILT_ANGLE_MIN, TILT_ANGLE_MAX);
  // if (Focus == 1) {
  //   stepper_aim_angle_control(lseres_focus_angle, FOCUS_ANGLE_MIN, FOCUS_ANGLE_MAX);
  // }
  
  // === (6) (Optional) Read motor feedback ===
  // pan_current_angle  = pan_getAngle();
  tilt_current_angle = tilt_getAngle();
  // pan_current_speed  = pan_getVelocity();
  // tilt_current_speed = tilt_getVelocity();
  // pan_current_voltage  = tilt_getVoltage();
  // tilt_current_voltage = tilt_getVoltage();

  // === (7) (Optional) Send status back to Station ===
  // TurretStatus status;
  // status.motion.pan_angle_deg  = pan_current_angle;
  // status.motion.tilt_angle_deg = tilt_current_angle;
  // status.motion.pan_speed_rpm  = pan_current_speed;
  // status.motion.tilt_speed_rpm = tilt_current_speed;
  // status.motion.pan_voltage    = pan_current_voltage;
  // status.motion.tilt_voltage   = tilt_current_voltage;
  // stationMessenger.sendStatus(status);

  unsigned long loopTime = micros() - t0;
  // DEBUG_SERIAL.println(tilt_current_angle);
  // st_read_eeprom(); 
  // delay(2000);
}
