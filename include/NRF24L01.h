#pragma once


//------------------------------------------------
#define CS_GPIO_PORT GPIOA
#define CS_PIN GPIO_PIN_4
#define CS_ON HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET)
#define CS_OFF HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET)
#define CE_GPIO_PORT GPIOA
#define CE_PIN GPIO_PIN_3
#define CE_RESET HAL_GPIO_WritePin(CE_GPIO_PORT, CE_PIN, GPIO_PIN_RESET)
#define CE_SET HAL_GPIO_WritePin(CE_GPIO_PORT, CE_PIN, GPIO_PIN_SET)
#define IRQ_GPIO_PORT GPIOA
#define IRQ_PIN GPIO_PIN_2
#define IRQ HAL_GPIO_ReadPin(IRQ_GPIO_PORT, IRQ_PIN)
#define LED_GPIO_PORT GPIOC
#define LED_PIN GPIO_PIN_13
#define LED_ON HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET)
#define LED_TGL HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN)
//------------------------------------------------
#define ACTIVATE 0x50 //
#define RD_RX_PLOAD 0x61 // Define RX payload register address
#define WR_TX_PLOAD 0xA0 // Define TX payload register address
#define WR_TX_PLOAD_NO_ACK 0xB0 // Define TX payload register address
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
//------------------------------------------------
#define NRF_CONFIG 0x00 //'Config' register address
#define EN_AA 0x01 //'Enable Auto Acknowledgment' register address
#define EN_RXADDR 0x02 //'Enabled RX addresses' register address
#define SETUP_AW 0x03 //'Setup address width' register address
#define SETUP_RETR 0x04 //'Setup Auto. Retrans' register address
#define RF_CH 0x05 //'RF channel' register address
#define RF_SETUP 0x06 //'RF setup' register address
#define STATUS 0x07 //'Status' register address
#define OBSERVE_TX 0x08 //'Transmit observe' register
#define RX_ADDR_P0 0x0A //'RX address pipe0' register address
#define RX_ADDR_P1 0x0B //'RX address pipe1' register address
#define RX_ADDR_P2 0x0C //'RX address pipe2' register address
#define RX_ADDR_P3 0x0D //'RX address pipe3' register address
#define RX_ADDR_P4 0x0E //'RX address pipe4' register address
#define RX_ADDR_P5 0x0F //'RX address pipe5' register address
#define TX_ADDR 0x10 //'TX address' register address
#define RX_PW_P0 0x11 //'RX payload width, pipe0' register address
#define RX_PW_P1 0x12 //'RX payload width, pipe1' register address
#define RX_PW_P2 0x13 //'RX payload width, pipe1' register address
#define RX_PW_P3 0x14 //'RX payload width, pipe1' register address
#define RX_PW_P4 0x15 //'RX payload width, pipe1' register address
#define RX_PW_P5 0x16 //'RX payload width, pipe1' register address
#define FIFO_STATUS 0x17 //'FIFO Status Register' register address
#define DYNPD 0x1C
#define FEATURE 0x1D
//------------------------------------------------
#define PRIM_RX 0x00 //RX/TX control (1: PRX, 0: PTX)
#define PWR_UP 0x01 //1: POWER UP, 0:POWER DOWN
#define RX_DR 0x40 //Data Ready RX FIFO interrupt
#define TX_DS 0x20 //Data Sent TX FIFO interrupt
#define MAX_RT 0x10 //Maximum number of TX retransmits interrupt
//------------------------------------------------
#define W_REGISTER 0x20 //запись в регистр
//------------------------------------------------
#include "mik32_hal_spi.h"
#include "mik32_hal_gpio.h"
//------------------------------------------------

#define PIPE_0      0x01
#define PIPE_1      0x02
#define PIPE_2      0x04
#define PIPE_3      0x08
#define PIPE_4      0x10
#define PIPE_5      0x20



typedef struct __NRF24L01_t
{
    SPI_HandleTypeDef *spi;
    uint16_t cs;
    GPIO_TypeDef *ce_port;
    HAL_PinsTypeDef ce_pin;
    uint8_t pipe;
    uint64_t tx_addr;
    uint64_t rx_addr;
    uint8_t payload_width;
    uint8_t rf_channel;
} NRF24L01_t;


void NRF24L01_RX_Mode(NRF24L01_t *nrf);
void NRF24L01_TX_Mode(NRF24L01_t *nrf);
void NRF24_Init(NRF24L01_t *nrf);
void CE_DOWN(NRF24L01_t *nrf);
void CE_UP(NRF24L01_t *nrf);
uint8_t NRF24_ReadReg(NRF24L01_t *nrf, uint8_t addr);
void NRF24_WriteReg(NRF24L01_t *nrf, uint8_t addr, uint8_t data);
void NRF24_ReadBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan);
void NRF24_WriteBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan);
void NRF24_FlushRX(NRF24L01_t *nrf);
void NRF24_FlushTX(NRF24L01_t *nrf);
void NRF24_Transmit(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan);
uint8_t NRF24L01_Send(NRF24L01_t *nrf, uint8_t *buf);