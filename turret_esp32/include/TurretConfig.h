#ifndef TURRETCONFIG_H
#define TURRETCONFIG_H

#include <Arduino.h>

// =============================
// 🔌 GPIO PINS
// =============================

// NEMA Stepper Driver (TB6600)
#define EN_PIN          25
#define DIR_PIN         26
#define STEP_PIN        27
#define COM_PIN         14
#define LIMIT_PIN       5
#define STEPS_PER_REV   1600

// Stepper Motor Driver (28YJ-48)
#define STEP1           23
#define STEP2           4
#define STEP3           2
#define STEP4           15

// LEDs
#define LED1            34
#define LED2            35
#define LED_BUILTIN     2

// Buttons & Switches
#define S1              2
#define S2              15

// =============================
// 📡 SERIAL & COMMUNICATION
// =============================

// Station Serial
#define DEBUG_SERIAL    Serial
#define DEBUG_BAUDRATE  115200
#define DEBUG_TX         1
#define DEBUG_RX         3

// Turret <-> Control Station (UART1)
#define station_SERIAL  Serial
#define station_BAUDRATE 115200
#define station_TX      1  // 17
#define station_RX      3  // 16

// Servo Serial
#define Servo_SERIAL    Serial1
#define SERVO_BAUDRATE  1000000
#define S_TX            19
#define S_RX            18

// CAN Interface
#define CAN_TX          12
#define CAN_RX          13 
// Modbus Interface
#define MOD_EN          5
#define MOD_TX          17
#define MOD_RX          16
// I2C Interface
#define SDA             21
#define SCL             22

// STM32 Serial
#define STM32_SERIAL    Serial2
#define STM32_BAUDRATE  115200
#define STM32_TX3       32
#define STM32_RX3       33

// =============================
// 📌 STM32 I/O CONTROL NOTES
// =============================
// On STM32 side (F0 series), output & input arrays are defined as:
/*
STM_PinDef myOutputs[] = {
    {K1_GPIO_Port, K1_Pin},
    {K2_GPIO_Port, K2_Pin},
    {K3_GPIO_Port, K3_Pin},
    {K4_GPIO_Port, K4_Pin},
    {K5_GPIO_Port, K5_Pin},
    {K6_GPIO_Port, K6_Pin},
    {LedA_GPIO_Port, LedA_Pin},
    {LedB_GPIO_Port, LedB_Pin},
    {IO1_GPIO_Port, IO1_Pin},
    {IO2_GPIO_Port, IO2_Pin},
    {IO3_GPIO_Port, IO3_Pin},
    {IO4_GPIO_Port, IO4_Pin},
    {IO5_GPIO_Port, IO5_Pin},
    {IO6_GPIO_Port, IO6_Pin},
    {IO7_GPIO_Port, IO7_Pin},
    {IO8_GPIO_Port, IO8_Pin},
    // Add more if needed
};
STM_PinDef myInputs[] = {
    {L1_GPIO_Port, L1_Pin},
    {L2_GPIO_Port, L2_Pin},
    {L3_GPIO_Port, L3_Pin},
    {L4_GPIO_Port, L4_Pin},
    {L5_GPIO_Port, L5_Pin},
    {L6_GPIO_Port, L6_Pin},
    {Switch_GPIO_Port, Switch_Pin},
    // Add more if needed
};
*/

// =============================
// 📌 STM32 I/O
// =============================
extern bool stm_output[16];
extern bool stm_input[7];

extern bool Buzzer, Motor1, Motor2;
extern bool Laser1, Laser2, Laser3, Focus;
extern bool STM_LedA, STM_LedB;
extern bool STM_IO1, STM_IO2, STM_IO3, STM_IO4, STM_IO5, STM_IO6, STM_IO7, STM_IO8;

// =============================
// ⚙️ FUNCTION DECLARATIONS
// =============================
void setupConfig();

#endif // TURRETCONFIG_H
