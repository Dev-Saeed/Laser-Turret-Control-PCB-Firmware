/*
 * esp32_link.cpp
 *
 *  Created on: Jul 21, 2025
 *      Author: sj
 */

#include "esp32_link.h"

// =================== Protocol ===================
#define START_BYTE 0xAA
#define END_BYTE   0x55

// =================== Static variables ===================
static UART_HandleTypeDef* activeUart = NULL;
static STM_PinDef outputPins[MAX_OUTPUT_PINS];
static uint8_t outputCount = 0;

static STM_PinDef inputPins[MAX_INPUT_PINS];
static uint8_t inputCount = 0;

// Buffers
static uint8_t rxBuf[3];
static uint8_t txBuf[3];

// =================== Public functions ===================
void STM_SetUART(UART_HandleTypeDef* huart) {
    activeUart = huart;
}

void STM_SetOutputPins(STM_PinDef* pins, uint8_t count) {
    if (count > MAX_OUTPUT_PINS) count = MAX_OUTPUT_PINS;
    outputCount = count;
    for (uint8_t i = 0; i < count; i++) {
        outputPins[i] = pins[i];
        // configure as output (done already in MX_GPIO_Init usually)
    }
}

void STM_SetInputPins(STM_PinDef* pins, uint8_t count) {
    if (count > MAX_INPUT_PINS) count = MAX_INPUT_PINS;
    inputCount = count;
    for (uint8_t i = 0; i < count; i++) {
        inputPins[i] = pins[i];
        // configure as input (done already in MX_GPIO_Init usually)
    }
}

// =================== Helpers ===================
static void applyRelayStates(uint8_t dataByte) {
    for (uint8_t i = 0; i < outputCount; i++) {
        GPIO_PinState state = (dataByte & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(outputPins[i].port, outputPins[i].pin, state);
    }
}

static uint8_t readLimitSwitches(void) {
    uint8_t result = 0;
    for (uint8_t i = 0; i < inputCount; i++) {
        if (HAL_GPIO_ReadPin(inputPins[i].port, inputPins[i].pin) == GPIO_PIN_SET) {
            result |= (1 << i);
        }
    }
    return result;
}

// =================== Main API actions ===================
void STM_ProcessIncoming(void) {
    if (activeUart == NULL) return;

    if (HAL_UART_Receive(activeUart, rxBuf, 3, 10) == HAL_OK) {
        if (rxBuf[0] == START_BYTE && rxBuf[2] == END_BYTE) {
            uint8_t relayData = rxBuf[1];
            applyRelayStates(relayData);
        }
    }
}

void STM_SendInputs(void) {
    if (activeUart == NULL) return;

    txBuf[0] = START_BYTE;
    txBuf[1] = readLimitSwitches();
    txBuf[2] = END_BYTE;
    HAL_UART_Transmit(activeUart, txBuf, 3, HAL_MAX_DELAY);
}

