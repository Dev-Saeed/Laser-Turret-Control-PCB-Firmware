// #include <Arduino.h>
// #include "StationConfig.h"
 

// #define TX_UART Serial
// #define START_BYTE 0xAA
// #define END_BYTE   0x55

// void setup() {
//   setupConfig();
//   TX_UART.begin(JSON_BAUDRATE, SERIAL_8N1, JSON_RX, JSON_TX);
// }

// void loop() {

//   bool btn_arm         = !digitalRead(BTN_ARM);
//   bool btn_laser_blue  = !digitalRead(BTN_LASER_BLUE);
//   bool btn_laser_green = !digitalRead(BTN_LASER_GREEN);

//   bool stageA = !digitalRead(BTN_STAGE_A);
//   bool stageB = !digitalRead(BTN_STAGE_B);
//   bool stage1 = false;
//   bool stage2 = false;
//   bool stage3 = false;
//   if (stageA && !stageB)      stage1 = true;
//   else if (!stageA && stageB) stage2 = true;
//   else if (stageA && stageB)  stage3 = true;

//   bool manual_mode    = !digitalRead(BTN_MANUAL);
//   bool confirm_target = !digitalRead(BTN_CONFIRM);


//   int rawPan   = analogRead(JOY_POT_PAN);
//   int rawTilt  = analogRead(JOY_POT_TILT);
//   int rawFocus = analogRead(JOY_POT_FOCUS);
//   bool joyPress = !digitalRead(JOY_BTN_PRESS);


//   auto mapAxis = [](int raw) {
//     float v = ((float)raw / 4095.0f) * 200.0f - 100.0f;
//     if (v < -100) v = -100;
//     if (v > 100) v = 100;
//     return (uint8_t)(v + 100);
//   };

//   uint8_t potPanVal   = mapAxis(rawPan);
//   uint8_t potTiltVal  = mapAxis(rawTilt);
//   uint8_t potFocusVal = mapAxis(rawFocus);


//   uint8_t btnBits = 0;
//   btnBits |= (btn_arm         ? (1 << 0) : 0);
//   btnBits |= (btn_laser_blue  ? (1 << 1) : 0);
//   btnBits |= (btn_laser_green ? (1 << 2) : 0);
//   btnBits |= (stage1          ? (1 << 3) : 0);
//   btnBits |= (stage2          ? (1 << 4) : 0);
//   btnBits |= (stage3          ? (1 << 5) : 0);
//   btnBits |= (manual_mode     ? (1 << 6) : 0);
//   btnBits |= (confirm_target  ? (1 << 7) : 0);


//   uint8_t txBuf[7];
//   txBuf[0] = START_BYTE;
//   txBuf[1] = btnBits;
//   txBuf[2] = potPanVal;
//   txBuf[3] = potTiltVal;
//   txBuf[4] = potFocusVal;
//   txBuf[5] = joyPress ? 1 : 0;
//   txBuf[6] = END_BYTE;


//   TX_UART.write(txBuf, sizeof(txBuf));


//   delay(20);
// }
