#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "mik32_hal_spi.h"
#include "mik32_hal_gpio.h"
#include "nrf_regmap.h"


void NRF_SPI_Init();

void NRF_begin(void);
void NRF_readRegister(uint8_t addr, uint8_t* buf, uint8_t quan);
void NRF_writeRegister(uint8_t addr, uint8_t buf);
void NRF_readPayload(uint8_t* buf, uint8_t quan);
void NRF_writePayload(uint8_t* buf, uint8_t quan);
uint8_t NRF_getStatus();
void NRF_CE_UP();
void NRF_CE_DOWN();


void NRF_begin();

void NRF_startListening();
void NRF_stopListening();
void NRF_transmit(uint8_t* payload, uint8_t quan);
bool NRF_dataReadyCheck();
void NRF_dataRead(uint8_t* buf);