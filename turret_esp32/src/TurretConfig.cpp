// pins.cpp
#include "TurretConfig.h"

bool stm_output[16] = {false};
bool stm_input[7]   = {false};

bool Buzzer = false, Motor1 = false, Motor2 = false;
bool Laser1 = false, Laser2 = false, Laser3 = false, Focus = false;
bool STM_LedA = false, STM_LedB = false;
bool STM_IO1 = false, STM_IO2 = false, STM_IO3 = false, STM_IO4 = false,
     STM_IO5 = false, STM_IO6 = false, STM_IO7 = false, STM_IO8 = false;
     
void setupConfig() {
  // LEDs as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Buttons as inputs with pull-up (adjust if you use pull-down instead)
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);

  // Stepper pins as outputs
  pinMode(STEP1, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(STEP3, OUTPUT);
  pinMode(STEP4, OUTPUT);

  // CAN pins
  pinMode(CAN_TX, OUTPUT);
  pinMode(CAN_RX, INPUT);

  // I2C pins will be initialized by Wire.begin(), no need to set manually
  // pinMode(SDA, INPUT);
  // pinMode(SCL, INPUT);

  // STM32 UART pins
  pinMode(STM32_TX3, OUTPUT);
  pinMode(STM32_RX3, INPUT);

  // Secondary UART pins
  pinMode(S_TX, OUTPUT);
  pinMode(S_RX, INPUT);

  // Module control pins
  pinMode(MOD_EN, OUTPUT);
  pinMode(MOD_TX, OUTPUT);
  pinMode(MOD_RX, INPUT);
}