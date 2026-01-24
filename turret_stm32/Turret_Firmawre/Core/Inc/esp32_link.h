/*
 * esp32_link.h
 *
 *  Created on: Jul 21, 2025
 *      Author: sj
 */

#ifndef INC_ESP32_LINK_H_
#define INC_ESP32_LINK_H_

#include "main.h"
#include <stdbool.h>

// =================== Configurable maximum counts ===================
#define MAX_OUTPUT_PINS 16
#define MAX_INPUT_PINS  16

// =================== Data structure for pins ===================
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} STM_PinDef;

// =================== Public API ===================
#ifdef __cplusplus
extern "C" {
#endif

// Initialize with chosen UART (pass &huart1 or &huart2 etc.)
void STM_SetUART(UART_HandleTypeDef* huart);

// Configure output pins array
void STM_SetOutputPins(STM_PinDef* pins, uint8_t count);

// Configure input pins array
void STM_SetInputPins(STM_PinDef* pins, uint8_t count);

// Process incoming relay data and update output pins
void STM_ProcessIncoming(void);

// Read input pins and send their states
void STM_SendInputs(void);

#ifdef __cplusplus
}
#endif


#endif /* INC_ESP32_LINK_H_ */
