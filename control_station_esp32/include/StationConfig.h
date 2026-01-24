#ifndef STATIONCONFIG_H
#define STATIONCONFIG_H

#include <Arduino.h>

// ============================================================
// 🎛️ BUTTONS
// ============================================================

#define BTN_ARM         14
#define BTN_LASER_BLUE  17
#define BTN_LASER_GREEN 4
#define BTN_STAGE_A     25  // stage1/2/3 encoded with two buttons
#define BTN_STAGE_B     26
#define BTN_MANUAL      27
#define BTN_CONFIRM     16

// ============================================================
// 🕹️ JOYSTICK
// ============================================================
// Analog inputs for X/Y/Focus axes
#define JOY_POT_PAN     34
#define JOY_POT_TILT    35
#define JOY_POT_FOCUS   32
#define JOY_BTN_PRESS   33
#define gnd             13  // ground reference if wired

// =============================
// 📡 SERIAL & COMMUNICATION
// =============================
#define DEBUG_SERIAL    Serial
#define DEBUG_BAUDRATE  115200
#define DEBUG_RX         3
#define DEBUG_TX         1

// turret <-> Control Station (UART1)
#define turret_SERIAL   Serial1
#define turret_BAUDRATE 115200
#define turret_TX       23
#define turret_RX       22

// Control Station <-> gui (UART2)
#define gui_SERIAL      Serial
#define gui_BAUDRATE    115200
#define gui_RX          3
#define gui_TX          1

// I2C Interface
#define SDA             21
#define SCL             22

// STM32 Serial
// #define STM32_SERIAL    Serial
// #define STM32_BAUDRATE  115200
// #define STM32_TX3       32
// #define STM32_RX3       33

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

// ============================================================
// 📌 STM32 I/O 
// ============================================================
extern bool stm_output[16];
extern bool stm_input[7];

extern bool Buzzer, Motor1, Motor2;
extern bool Laser1, Laser2, Laser3;
extern bool STM_LedA, STM_LedB;
extern bool STM_IO1, STM_IO2, STM_IO3, STM_IO4, STM_IO5, STM_IO6, STM_IO7, STM_IO8;

// ============================================================
// ⚙️ FUNCTIONS DECLARATIONS
// ============================================================

// Initializes pin modes or other configuration
void setupConfig();

// Reads joystick axis and maps to [-100 .. 100]
float readJoystickAxis(int analogPin);

#endif // STATIONCONFIG_H
