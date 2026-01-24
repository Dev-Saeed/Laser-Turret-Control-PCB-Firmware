#include "STM_Link.h"

void STM_Link::beginUART(HardwareSerial &port, uint32_t baud, int rxPin, int txPin) {
    uart = &port;
    uart->begin(baud, SERIAL_8N1, rxPin, txPin);
    rxBuffer.reserve(32);
}