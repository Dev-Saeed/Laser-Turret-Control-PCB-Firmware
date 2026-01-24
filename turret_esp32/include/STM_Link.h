#ifndef STM_LINK_H
#define STM_LINK_H

#include <Arduino.h>
#include "TurretConfig.h"

#define START_BYTE 0xAA
#define END_BYTE   0x55

class STM_Link {
public:
    void beginUART(HardwareSerial &port, uint32_t baud, int rxPin, int txPin);

    // Send relay states (array size deduced automatically)
    template<size_t N>
    void sendRelayStates(const bool (&relayStates)[N]) {
        if (!uart) return;
        uint8_t dataByte = 0;
        for (size_t i = 0; i < N && i < 8; i++) { // only pack 8 bits
            if (relayStates[i]) dataByte |= (1 << i);
        }
        uart->write(START_BYTE);
        uart->write(dataByte);
        uart->write(END_BYTE);
    }

    // Read limit states into array (array size deduced automatically)
    template<size_t N>
    bool readLimitStates(bool (&limitStates)[N]) {
        if (!uart) return false;

        while (uart->available()) {
            char c = (char)uart->read();
            rxBuffer += c;
            if (rxBuffer.length() > 32) rxBuffer = "";

            if (rxBuffer.length() >= 3) {
                for (int i = 0; i <= (int)rxBuffer.length() - 3; i++) {
                    if ((uint8_t)rxBuffer[i] == START_BYTE &&
                        (uint8_t)rxBuffer[i + 2] == END_BYTE) {
                        uint8_t dataByte = (uint8_t)rxBuffer[i + 1];
                        for (size_t j = 0; j < N && j < 8; j++) {
                            limitStates[j] = (dataByte >> j) & 0x01;
                        }
                        rxBuffer = "";
                        return true;
                    }
                }
            }
        }
        return false;
    }

private:
    HardwareSerial* uart = nullptr;
    String rxBuffer;
};

#endif
