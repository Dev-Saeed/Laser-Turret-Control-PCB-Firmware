#ifndef TEST_H
#define TEST_H

#include <Arduino.h>

#define S_RX 18
#define S_TX 19
#define DEBUG_SERIAL Serial
extern HardwareSerial Servo_SERIAL; 


inline void test() {

  DEBUG_SERIAL.begin(115200);
  Servo_SERIAL.begin(57600, SERIAL_8N1, S_RX, S_TX);
  delay(500);
  DEBUG_SERIAL.println("ESP32 UART started.");

  while (true) {
    static uint32_t lastSendTime = 0;
    static uint32_t counter = 0;
    static String buffer = "";

    // Send message every 2 second
    if (millis() - lastSendTime >= 10) {
      String msg = "MSG " + String(counter++) + "\r\n";
      Servo_SERIAL.print(msg);
      DEBUG_SERIAL.print("Sent: ");
      DEBUG_SERIAL.print(msg);
      lastSendTime = millis();
    }

    // Read STM32 response
    while (Servo_SERIAL.available()) {
      char c = Servo_SERIAL.read();
      buffer += c;
      if (c == '\n') {
        DEBUG_SERIAL.print("Received: ");
        DEBUG_SERIAL.print(buffer);
        buffer = "";
      }
    }

  }

}

#endif