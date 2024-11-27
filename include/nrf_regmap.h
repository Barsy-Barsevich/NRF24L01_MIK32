#pragma once



#define NRF_CONFIG          0x00
#define NRF_EN_AA           0x01
#define NRF_EN_RXADDR       0x02
#define NRF_SETUP_AW        0x03
#define NRF_SETUP_RETR      0x04
#define NRF_RF_CH           0x05
#define NRF_RF_SETUP        0x06
#define NRF_STATUS          0x07
#define NRF_OBSERVE_TX      0x08
#define NRF_RPD             0x09
#define NRF_RX_ADDR_P0      0x0A
#define NRF_RX_ADDR_P1      0x0B
#define NRF_RX_ADDR_P2      0x0C
#define NRF_RX_ADDR_P3      0x0D
#define NRF_RX_ADDR_P4      0x0E
#define NRF_RX_ADDR_P5      0x0F
#define NRF_TX_ADDR         0x10
#define NRF_RX_PW_P0        0x11
#define NRF_RX_PW_P1        0x12
#define NRF_RX_PW_P2        0x13
#define NRF_RX_PW_P3        0x14
#define NRF_RX_PW_P4        0x15
#define NRF_RX_PW_P5        0x16
#define NRF_FIFO_STATUS     0x17
#define NRF_DYNPD           0x1C
#define NRF_FEATURE         0x1D


//CONFIG
#define NRF_CONFIG_MASK_RX_DR       6
#define NRF_CONFIG_MASK_TX_DS       5
#define NRF_CONFIG_MASK_MAX_RT      4
#define NRF_CONFIG_EN_CRC           3
#define NRF_CONFIG_CRCO             2
#define NRF_CONFIG_PWR_UP           1
#define NRF_CONFIG_PRIM_RX          0

/* EN_AA */
#define NRF_EN_AA_ENAA_P5           5
#define NRF_EN_AA_ENAA_P4           4
#define NRF_EN_AA_ENAA_P3           3
#define NRF_EN_AA_ENAA_P2           2
#define NRF_EN_AA_ENAA_P1           1
#define NRF_EN_AA_ENAA_P0           0

/* EN_RXADDR */
#define NRF_EN_RXADDR_ERX_P5        5
#define NRF_EN_RXADDR_ERX_P4        4
#define NRF_EN_RXADDR_ERX_P3        3
#define NRF_EN_RXADDR_ERX_P2        2
#define NRF_EN_RXADDR_ERX_P1        1
#define NRF_EN_RXADDR_ERX_P0        0

/* SETUP_RETR */
#define NRF_SETUP_RETR_ARD          4
#define NRF_SETUP_RETR_ARC          0

/* RF_SETUP */
#define NRF_RF_SETUP_CONT_WAVE      7
#define NRF_RF_SETUP_RF_DR_LOW      5
#define NRF_RF_SETUP_PLL_LOCK       4
#define NRF_RF_SETUP_RF_DR_HIGH     3
#define NRF_RF_SETUP_RF_PWR         1

/* STATUS */
#define NRF_STATUS_RX_DR            6
#define NRF_STATUS_TX_DS            5
#define NRF_STATUS_MAX_RT           4
#define NRF_STATUS_RX_P_NO          1
#define NRF_STATUS_TX_FULL          0

/* OBSERVE_TX */
#define NRF_OBSERVE_TX_PLOS_CNT     4
#define NRF_OBSERVE_TX_ARC_CNT      0

/* FIFO_STATUS */
#define NRF_FIFO_STATUS_TX_REUSE    6
#define NRF_FIFO_STATUS_TX_FULL     5
#define NRF_FIFO_STATUS_TX_EMPTY    4
#define NRF_FIFO_STATUS_RX_FULL     1
#define NRF_FIFO_STATUS_RX_EMPTY    0

/* DYNPD */
#define NRF_DYNPD_DPL_P5            5
#define NRF_DYNPD_DPL_P4            4
#define NRF_DYNPD_DPL_P3            3
#define NRF_DYNPD_DPL_P2            2
#define NRF_DYNPD_DPL_P1            1
#define NRF_DYNPD_DPL_P0            0

/* FEATURE */
#define NRF_FEATURE_EN_DPL          2
#define NRF_FEATURE_EN_ACK_PAY      1
#define NRF_FEATURE_EN_DYN_ACK      0


/* Commands */
#define NRF_COM_R_REGISTER          0x00
#define NRF_COM_W_REGISTER          0x20
#define NRF_COM_R_RX_PAYLOAD        0x61
#define NRF_COM_W_TX_PAYLOAD        0xA0
#define NRF_COM_FLUSH_TX            0xE1
#define NRF_COM_FLUSH_RX            0xE2
#define NRF_COM_REUSE_TX_PL         0xE3
#define NRF_COM_R_RX_PL_WID         0x60
#define NRF_COM_W_ACK_PAYLOAD       0xA8
#define NRF_COM_W_TX_PAYLOAD_NO_ACK 0xB0
#define NRF_COM_NOP                 0xFF

