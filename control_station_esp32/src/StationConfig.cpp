#include "StationConfig.h"

// allocate arrays
bool stm_output[16] = {0};
bool stm_input[7]   = {0};

bool Buzzer = 0, Motor1 = 0, Motor2 = 0;
bool Laser1 = 0, Laser2 = 0, Laser3 = 0;
bool STM_LedA = 0, STM_LedB = 0;
bool STM_IO1 = 0, STM_IO2 = 0, STM_IO3 = 0, STM_IO4 = 0;
bool STM_IO5 = 0, STM_IO6 = 0, STM_IO7 = 0, STM_IO8 = 0;

void setupConfig() {
  // Configure button pins
  pinMode(BTN_ARM, INPUT_PULLUP);
  pinMode(BTN_LASER_BLUE, INPUT_PULLUP);
  pinMode(BTN_LASER_GREEN, INPUT_PULLUP);
  pinMode(BTN_STAGE_A, INPUT_PULLUP);
  pinMode(BTN_STAGE_B, INPUT_PULLUP);
  pinMode(BTN_MANUAL, INPUT_PULLUP);
  pinMode(BTN_CONFIRM, INPUT_PULLUP);
  pinMode(gnd, OUTPUT);
  digitalWrite(gnd, LOW);

  // Joystick press button
  pinMode(JOY_BTN_PRESS, INPUT);

  // Analog joystick pins are automatically configured
}

// Utility to map joystick analog reading to -100..100
float readJoystickAxis(int analogPin) {
  int raw = analogRead(analogPin); // ESP32 ADC: 0 to 4095
  float mapped = ((float)raw / 4095.0f) * 200.0f - 100.0f; // Map to -100..100
  return mapped;
}

