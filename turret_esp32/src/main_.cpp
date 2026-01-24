// #include <Arduino.h>
// #include "TurretConfig.h"
// #include "STM_Link.h"
// #include "st_driver.h"
// #include "nema_driver.h"

// // UART used to receive from Control Station
// #define RX_UART station_SERIAL
// #define START_BYTE 0xAA
// #define END_BYTE   0x55
 
// STM_Link stmLink;

// // buffers
// uint8_t rxBuf[7];
// uint8_t rxIndex = 0;
// bool packetReady = false;

// // decoded inputs
// bool btn_arm = false;
// bool btn_laser_blue = false;
// bool btn_laser_green = false;
// // (stage1/stage2/stage3/manual/confirm if needed)
// // joystick axes can be decoded similarly if required

// void setup() {
//   delay(2000); // Wait for Serial Monitor to open
//   setupConfig();

//   // Init communication with STM32
//   stmLink.beginUART(STM32_SERIAL, STM32_BAUDRATE, STM32_RX3, STM32_TX3);

//   // Init RX UART
//   RX_UART.begin(station_BAUDRATE, SERIAL_8N1, station_RX, station_TX);

//   // Initialize servos and steppers and home them
//   stm_output[2] = 1; //Motor1;
//   stm_output[1] = 1; //Motor2;
//   stmLink.sendRelayStates(stm_output);
//   delay(1000);
//   init_ST(Servo_SERIAL, SERVO_BAUDRATE, S_RX, S_TX);
//   Tilt_home();
//   // st_OP_VELOCITY();
//   init_NEMA(EN_PIN, DIR_PIN, STEP_PIN, COM_PIN, LIMIT_PIN, STEPS_PER_REV);
//   // Pan_home();
//   delay(1000);
// }

// void loop() {
//   // ----------- (1) Read incoming packet -----------
//   while (RX_UART.available()) {
//     uint8_t b = RX_UART.read();

//     // find start
//     if (rxIndex == 0 && b != START_BYTE) {
//       continue;
//     }

//     rxBuf[rxIndex++] = b;

//     if (rxIndex == sizeof(rxBuf)) {
//       // verify end
//       if (rxBuf[0] == START_BYTE && rxBuf[6] == END_BYTE) {
//         packetReady = true;
//       }
//       rxIndex = 0;
//     }
//   }

//   if (packetReady) {
//     packetReady = false;

//     // ----------- (2) Decode button bits -----------
//     uint8_t btnBits = rxBuf[1];
//     btn_arm         = (btnBits >> 0) & 0x01;
//     btn_laser_blue  = (btnBits >> 1) & 0x01;
//     btn_laser_green = (btnBits >> 2) & 0x01;
//     // other bits if needed:
//     // bool stage1 = (btnBits >> 3) & 0x01;
//     // bool stage2 = (btnBits >> 4) & 0x01;
//     // bool stage3 = (btnBits >> 5) & 0x01;
//     bool manual_mode = (btnBits >> 6) & 0x01;
//     bool confirm_target = (btnBits >> 7) & 0x01;

//     // joystick values if needed:
//     // int pot_pan   = (int)rxBuf[2] - 100; // now in -100..100
//     // int pot_tilt  = (int)rxBuf[3] - 100;
//     // int pot_focus = (int)rxBuf[4] - 100;
//     // bool joy_press = rxBuf[5] == 1;

//     // ----------- (3) Update STM outputs -----------
//     stmLink.readLimitStates(stm_input); // read limit states first

//     stm_output[0] = btn_laser_blue;   // Buzzer (linked with laser blue)
//     stm_output[2] = 1;                // Motor1 always on
//     stm_output[1] = 1;                // Motor2 always on
//     stm_output[3] = btn_laser_green && btn_arm;     // Green
//     stm_output[4] = btn_laser_blue && btn_arm;     // Blue
//     stm_output[5] = btn_arm;          // Red
//     stm_output[6] = stm_input[6];     // LedA
//     stm_output[7] = btn_arm;                // LedB always on

//     if (confirm_target && manual_mode) {
//       Pan_home(); 
//       Pan_goTo(135.0f);
//       stm_output[0] = 1;     // Buzzer on
//       stm_output[4] = 1;     // Blue
//       stm_output[5] = 1;     // Red
//       stmLink.sendRelayStates(stm_output);
//       delay(3000);
//       stm_output[0] = 0;     // Buzzer on
//       stm_output[4] = 0;     // Blue
//       stm_output[5] = 0;     // Red
//       stmLink.sendRelayStates(stm_output);

//       // delay(500);


//       Pan_goTo(145.0f);
//       stm_output[0] = 1;     // Buzzer on
//       stm_output[4] = 1;     // Blue
//       stm_output[5] = 1;     // Red
//       stmLink.sendRelayStates(stm_output);
//       delay(3000);
//       stm_output[0] = 0;     // Buzzer on
//       stm_output[4] = 0;     // Blue
//       stm_output[5] = 0;     // Red
//       stmLink.sendRelayStates(stm_output);

//     }

//     stmLink.sendRelayStates(stm_output); // send outputs to STM32
//   }

//   Tilt_home();
//   // Loop delay (optional small delay)
//   delay(10);
// }
