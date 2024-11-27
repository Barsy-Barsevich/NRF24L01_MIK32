#pragma once


//------------------------------------------------
#define ACTIVATE 0x50 //
#define RD_RX_PLOAD 0x61 // Define RX payload register address
#define WR_TX_PLOAD 0xA0 // Define TX payload register address
#define WR_TX_PLOAD_NO_ACK 0xB0 // Define TX payload register address
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
//------------------------------------------------
#define NRF_CONFIG      0x00 //'Config' register address
#define NRF_EN_AA       0x01 //'Enable Auto Acknowledgment' register address
#define NRF_EN_RXADDR   0x02 //'Enabled RX addresses' register address
#define NRF_SETUP_AW    0x03 //'Setup address width' register address
#define NRF_SETUP_RETR 0x04 //'Setup Auto. Retrans' register address
#define NRF_RF_CH 0x05 //'RF channel' register address
#define NRF_RF_SETUP 0x06 //'RF setup' register address
#define NRF_STATUS 0x07 //'Status' register address
#define NRF_OBSERVE_TX 0x08 //'Transmit observe' register
#define NRF_RX_ADDR_P0 0x0A //'RX address pipe0' register address
#define NRF_RX_ADDR_P1 0x0B //'RX address pipe1' register address
#define NRF_RX_ADDR_P2 0x0C //'RX address pipe2' register address
#define NRF_RX_ADDR_P3 0x0D //'RX address pipe3' register address
#define NRF_RX_ADDR_P4 0x0E //'RX address pipe4' register address
#define NRF_RX_ADDR_P5 0x0F //'RX address pipe5' register address
#define NRF_TX_ADDR 0x10 //'TX address' register address
#define NRF_RX_PW_P0 0x11 //'RX payload width, pipe0' register address
#define NRF_RX_PW_P1 0x12 //'RX payload width, pipe1' register address
#define NRF_RX_PW_P2 0x13 //'RX payload width, pipe1' register address
#define NRF_RX_PW_P3 0x14 //'RX payload width, pipe1' register address
#define NRF_RX_PW_P4 0x15 //'RX payload width, pipe1' register address
#define NRF_RX_PW_P5 0x16 //'RX payload width, pipe1' register address
#define NRF_FIFO_STATUS 0x17 //'FIFO Status Register' register address
#define NRF_DYNPD 0x1C
#define NRF_FEATURE 0x1D
//------------------------------------------------
#define NRF_CONFIG_PRIM_RX      0 //RX/TX control (1: PRX, 0: PTX)
#define NRF_CONFIG_PRIM_RX_M    (1<<NRF_CONFIG_PRIM_RX)
#define NRF_CONFIG_PWR_UP       1 //1: POWER UP, 0:POWER DOWN
#define NRF_CONFIG_PWR_UP_M     (1<<NRF_CONFIG_PWR_UP)
#define NRF_CONFIG_CRCO         2
#define NRF_CONFIG_CRCO_M       (1<<NRF_CONFIG_CRCO)
#define NRF_CONFIG_EN_CRC       3
#define NRF_CONFIG_EN_CRC_M     (1<<NRF_CONFIG_EN_CRC) 
#define NRF_CONFIG_MAX_RT       4 //Maximum number of TX retransmits interrupt
#define NRF_CONFIG_MAX_RT_M     (1<<NRF_CONFIG_MAX_RT)
#define NRF_CONFIG_TX_DS        5 //Data Sent TX FIFO interrupt
#define NRF_CONFIG_TX_DS_M      (1<<NRF_CONFIG_TX_DS)
#define NRF_CONFIG_RX_DR        6 //Data Ready RX FIFO interrupt
#define NRF_CONFIG_RX_DR_M      (1<<NRF_CONFIG_RX_DR)

#define NRF_RF_SETUP_RF_DR_LOW          5
#define NRF_RF_SETUP_RF_DR_HIGH         3
#define NRF_RF_SETUP_RF_PWR             1

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


typedef enum __RF_DataRate{
    RF_DataRate_250kbps = (1<<NRF_RF_SETUP_RF_DR_LOW),
    RF_DataRate_1Mbps = 0,
    RF_DataRate_2Mbps = (1<<NRF_RF_SETUP_RF_DR_HIGH),
} NRF24L01_RF_DR_t;

typedef enum __RF_Power {
    power_0dBm = 0b11<<NRF_RF_SETUP_RF_PWR,
    power_m6dBm = 0b10<<NRF_RF_SETUP_RF_PWR,
    power_m12dBm = 0b01<<NRF_RF_SETUP_RF_PWR,
    power_m18dBm = 0b00<<NRF_RF_SETUP_RF_PWR,
    no_attenuation = 0b11<<NRF_RF_SETUP_RF_PWR,
    attenuate_in_2_times = 0b10<<NRF_RF_SETUP_RF_PWR,
    attenuate_in_4_times = 0b01<<NRF_RF_SETUP_RF_PWR,
    attenuate_in_8_times = 0b00<<NRF_RF_SETUP_RF_PWR,
} NRF24L01_RF_PWR_t;

typedef enum __CRC {
    crc_disabled,
    crc_8bit,
    crc_16bit
} NRF24L01_CRC_t;


typedef struct __NRF24L01_t
{
    SPI_HandleTypeDef *spi;
    uint16_t cs;
    GPIO_TypeDef *ce_port;
    HAL_PinsTypeDef ce_pin;
    uint8_t pipe;
    uint8_t address_width;
    uint64_t tx_addr;
    uint64_t rx_addr;
    uint8_t payload_width;
    struct {
        uint8_t channel;
        NRF24L01_RF_DR_t datarate;
        NRF24L01_RF_PWR_t power;
    } rf;
} NRF24L01_t;


void NRF24L01_RX_Mode(NRF24L01_t *nrf);
void NRF24L01_TX_Mode(NRF24L01_t *nrf);
void NRF24L01_Init(NRF24L01_t *nrf);
void CE_DOWN(NRF24L01_t *nrf);
void CE_UP(NRF24L01_t *nrf);
uint8_t NRF24L01_ReadReg(NRF24L01_t *nrf, uint8_t addr);
void NRF24L01_WriteReg(NRF24L01_t *nrf, uint8_t addr, uint8_t data);
void NRF24L01_ReadBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan);
void NRF24L01_WriteBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan);
void NRF24L01_FlushRX(NRF24L01_t *nrf);
void NRF24L01_FlushTX(NRF24L01_t *nrf);
void NRF24L01_Transmit(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan);
uint8_t NRF24L01_Send(NRF24L01_t *nrf, uint8_t *buf);

HAL_StatusTypeDef NRF24L01_RX_data_ready(NRF24L01_t *nrf);
HAL_StatusTypeDef NRF24L01_TX_data_sent(NRF24L01_t *nrf);

HAL_StatusTypeDef NRF24L01_Read(NRF24L01_t *nrf, uint8_t *buf);