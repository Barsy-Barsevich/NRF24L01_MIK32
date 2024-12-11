
#include "NRF24L01.h"

/**
 * @brief Переход в режим приема данных
 */
void NRF24L01_RX_Mode(NRF24L01_t *nrf)
{
    uint8_t regval=0x00;
    regval = NRF24L01_ReadReg(nrf, NRF_CONFIG);
    //разбудим модуль и переведём его в режим приёмника, включив биты NRF_CONFIG_PWR_UP и NRF_CONFIG_PRIM_RX
    regval |= NRF_CONFIG_PWR_UP_M | NRF_CONFIG_PRIM_RX_M;
    NRF24L01_WriteReg(nrf, NRF_CONFIG, regval);
    CE_UP(nrf);
    HAL_DelayUs(150); //Задержка минимум 130 мкс
    // Flush buffers
    NRF24L01_FlushRX(nrf);
    NRF24L01_FlushTX(nrf);
}

/**
 * @brief Переход в режим отправки данных
 */
void NRF24L01_TX_Mode(NRF24L01_t *nrf)
{
    NRF24L01_WriteBuf(nrf, NRF_TX_ADDR, (uint8_t*)&(nrf->pipe.tx_addr), 5);
    CE_DOWN(nrf);
    // Flush buffers
    NRF24L01_FlushRX(nrf);
    NRF24L01_FlushTX(nrf);
}

/**
 * @brief Инициализация
 */
void NRF24L01_Init(NRF24L01_t *nrf)
{
    CE_DOWN(nrf);
    HAL_DelayUs(4500);
    //NRF24L01_WriteReg(nrf, NRF_CONFIG, 0b00001010);
    uint8_t config = NRF_CONFIG_PWR_UP_M;
    switch (nrf->crc)
    {
        case crc_disabled: config |= 0; break;
        case crc_8bit: config |= NRF_CONFIG_EN_CRC_M; break;
        case crc_16bit: config |= NRF_CONFIG_EN_CRC_M | NRF_CONFIG_CRCO_M; break;
        default: config |= 0;
    }
    if (nrf->irq.rx) config |= NRF_CONFIG_RX_DR_M;
    if (nrf->irq.tx) config |= NRF_CONFIG_TX_DS_M;
    if (nrf->irq.max_rt) config |= NRF_CONFIG_MAX_RT_M;
    NRF24L01_WriteReg(nrf, NRF_CONFIG, config);

    HAL_DelayUs(4500);
    uint8_t en_aa = 0x01; //< rx0 enabled by default
    if (nrf->pipe.rx1_en) en_aa |= 0x02;
    if (nrf->pipe.rx2_en) en_aa |= 0x04;
    if (nrf->pipe.rx3_en) en_aa |= 0x08;
    if (nrf->pipe.rx4_en) en_aa |= 0x10;
    if (nrf->pipe.rx5_en) en_aa |= 0x20;
    NRF24L01_WriteReg(nrf, NRF_EN_AA, en_aa);
    NRF24L01_WriteReg(nrf, NRF_EN_RXADDR, en_aa);

    uint8_t setup_aw;
    switch (nrf->address_width)
    {
        case 3: setup_aw = 0b01; break;
        case 4: setup_aw = 0b10; break;
        case 5: setup_aw = 0b11; break;
        default: setup_aw = 0b11;
    }
    NRF24L01_WriteReg(nrf, NRF_SETUP_AW, setup_aw);
    NRF24L01_WriteReg(nrf, NRF_SETUP_RETR, 0x5F); // // 1500us, 15 retrans
    NRF24L01_WriteReg(nrf, NRF_FEATURE, 0);
    NRF24L01_WriteReg(nrf, NRF_DYNPD, 0); // dynamic payload length off
    NRF24L01_WriteReg(nrf, NRF_STATUS, 0b01110000); // reset interrupt flags
    NRF24L01_WriteReg(nrf, NRF_RF_CH, nrf->rf.channel);
    NRF24L01_WriteReg(nrf, NRF_RF_SETUP, nrf->rf.datarate | nrf->rf.power);

    NRF24L01_WriteBuf(nrf, NRF_TX_ADDR, (uint8_t*)&(nrf->pipe.tx_addr), 5);
    NRF24L01_WriteBuf(nrf, NRF_RX_ADDR_P0, (uint8_t*)&(nrf->pipe.tx_addr), 5);
    uint64_t rx_addr = (uint64_t)nrf->pipe.common_rx_msaddr << 8 | nrf->pipe.rx1_lsaddr;
    NRF24L01_WriteBuf(nrf, NRF_RX_ADDR_P1, (uint8_t*)&(rx_addr), 5);
    NRF24L01_WriteBuf(nrf, NRF_RX_ADDR_P2, &(nrf->pipe.rx2_lsaddr), 1);
    NRF24L01_WriteBuf(nrf, NRF_RX_ADDR_P3, &(nrf->pipe.rx3_lsaddr), 1);
    NRF24L01_WriteBuf(nrf, NRF_RX_ADDR_P4, &(nrf->pipe.rx4_lsaddr), 1);
    NRF24L01_WriteBuf(nrf, NRF_RX_ADDR_P5, &(nrf->pipe.rx5_lsaddr), 1);

    NRF24L01_WriteReg(nrf, NRF_RX_PW_P0, nrf->payload_width);
    NRF24L01_WriteReg(nrf, NRF_RX_PW_P1, nrf->payload_width);
    NRF24L01_WriteReg(nrf, NRF_RX_PW_P2, nrf->payload_width);
    NRF24L01_WriteReg(nrf, NRF_RX_PW_P3, nrf->payload_width);
    NRF24L01_WriteReg(nrf, NRF_RX_PW_P4, nrf->payload_width);
    NRF24L01_WriteReg(nrf, NRF_RX_PW_P5, nrf->payload_width);
    NRF24L01_RX_Mode(nrf);
}

/**
 * @brief Линии CE задать низкий уровень
 */
void CE_DOWN(NRF24L01_t *nrf)
{
    HAL_GPIO_WritePin(nrf->interface.ce_port, nrf->interface.ce_pin, 0);
}

/**
 * @brief Линии CE задать высокий уровень
 */
void CE_UP(NRF24L01_t *nrf)
{
    HAL_GPIO_WritePin(nrf->interface.ce_port, nrf->interface.ce_pin, 1);
}

/**
 * @brief Чтение регистра NRF24L01
 * @return содержимое регистра
 */
uint8_t NRF24L01_ReadReg(NRF24L01_t *nrf, uint8_t addr)
{
    uint8_t data = 0, cmd;
    HAL_SPI_CS_Enable(nrf->interface.spi, nrf->interface.cs);
    HAL_SPI_Exchange(nrf->interface.spi, &addr, &data, 1, SPI_TIMEOUT_DEFAULT);
    if (addr != NRF_STATUS)
    {
        cmd = 0xFF;
        HAL_SPI_Exchange(nrf->interface.spi, &cmd, &data, 1, SPI_TIMEOUT_DEFAULT);
    }
    HAL_SPI_CS_Disable(nrf->interface.spi);
    return data;
}

/**
 * @brief Запись регистра NRF24L01
 */
void NRF24L01_WriteReg(NRF24L01_t *nrf, uint8_t addr, uint8_t data)
{
    addr |= W_REGISTER;
    uint8_t dummy;
    HAL_SPI_CS_Enable(nrf->interface.spi, nrf->interface.cs);
    HAL_SPI_Exchange(nrf->interface.spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_SPI_Exchange(nrf->interface.spi, &data, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_SPI_CS_Disable(nrf->interface.spi);
}

/**
 * @brief Чтение массива данных с NRF24L01 в буфер
 * @param addr адрес регистра NRF24L01
 * @param buf указатель на буфер
 * @param quan длина буфера
 */
void NRF24L01_ReadBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan)
{
    HAL_SPI_CS_Enable(nrf->interface.spi, nrf->interface.cs);
    uint8_t dummy;
    HAL_SPI_Exchange(nrf->interface.spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    dummy = 0xFF;
    for (uint8_t i=0; i<quan; i++)
    {
        HAL_SPI_Exchange(nrf->interface.spi, &dummy, buf+i, 1, SPI_TIMEOUT_DEFAULT);
    }
    HAL_SPI_CS_Disable(nrf->interface.spi);
}

/**
 * @brief Запись массива данных в NRF24L01
 * @param addr адрес регистра NRF24L01
 * @param buf указатель на буфер
 * @param quan длина буфера
 */
void NRF24L01_WriteBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan)
{
    addr |= W_REGISTER;
    HAL_SPI_CS_Enable(nrf->interface.spi, nrf->interface.cs);
    uint8_t dummy;
    HAL_SPI_Exchange(nrf->interface.spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_DelayUs(1000);
    for (uint8_t i=0; i<quan; i++)
    {
        HAL_SPI_Exchange(nrf->interface.spi, buf+i, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    }
    HAL_SPI_CS_Disable(nrf->interface.spi);
}

/**
 * @brief Очистка буфера FIFO приемника
 */
void NRF24L01_FlushRX(NRF24L01_t *nrf)
{
    HAL_SPI_CS_Enable(nrf->interface.spi, nrf->interface.cs);
    uint8_t cmd = FLUSH_RX, dummy;
    HAL_SPI_Exchange(nrf->interface.spi, &cmd, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_DelayUs(1000);
    HAL_SPI_CS_Disable(nrf->interface.spi);
}

/**
 * @brief Очистка буфера FIFO передатчика
 */
void NRF24L01_FlushTX(NRF24L01_t *nrf)
{
    HAL_SPI_CS_Enable(nrf->interface.spi, nrf->interface.cs);
    uint8_t cmd = FLUSH_TX, dummy;
    HAL_SPI_Exchange(nrf->interface.spi, &cmd, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_DelayUs(1000);
    HAL_SPI_CS_Disable(nrf->interface.spi);
}

/**
 * @brief
 */
void NRF24L01_Transmit(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan)
{
    CE_DOWN(nrf);
    HAL_SPI_CS_Enable(nrf->interface.spi, nrf->interface.cs);
    uint8_t dummy;
    HAL_SPI_Exchange(nrf->interface.spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);//отправим адрес в шину
    HAL_DelayUs(1000);
    for (uint8_t i=0; i<quan; i++)
    {
        HAL_SPI_Exchange(nrf->interface.spi, buf+i, &dummy, 1, SPI_TIMEOUT_DEFAULT);//отправим данные в буфер
    }
    HAL_SPI_CS_Disable(nrf->interface.spi);
    CE_UP(nrf);
}

/**
 * @brief Отправка пакета данных
 * @param buf буфер, из которого считываются данные для передачи. Количество считывемых
 * байт данных определяется настройкой длины пакета
 * @return содержимое регистра OBSERVE_TX NRF, если передача успешна, значение 0
 */
uint8_t NRF24L01_Send(NRF24L01_t *nrf, uint8_t *buf)
{
    uint8_t status=0, regval;
    NRF24L01_TX_Mode(nrf);
    regval = NRF24L01_ReadReg(nrf, NRF_CONFIG);
    regval |= (1<<NRF_CONFIG_PWR_UP);
    regval &= ~(1<<NRF_CONFIG_PRIM_RX);
    NRF24L01_WriteReg(nrf, NRF_CONFIG, regval);
    HAL_DelayUs(150);
    NRF24L01_Transmit(nrf, WR_TX_PLOAD_NO_ACK, buf, nrf->payload_width);
    CE_UP(nrf);
    HAL_DelayUs(15);
    CE_DOWN(nrf);
    regval = NRF24L01_ReadReg(nrf, NRF_OBSERVE_TX);
    return regval;
}

/**
 * @brief
 */
HAL_StatusTypeDef NRF24L01_RX_data_ready(NRF24L01_t *nrf)
{
    uint8_t status = NRF24L01_ReadReg(nrf, NRF_STATUS);
    return (status & NRF_CONFIG_RX_DR_M) != 0 ? HAL_OK : HAL_BUSY;
}

/**
 * @brief
 */
HAL_StatusTypeDef NRF24L01_TX_data_sent(NRF24L01_t *nrf)
{
    uint8_t status = NRF24L01_ReadReg(nrf, NRF_STATUS);
    return (status & NRF_CONFIG_TX_DS_M) != 0 ? HAL_OK : HAL_BUSY;
}

/**
 * @brief Прием данных
 * @param buf буфер, куда записываются принятные данные. Количество записываемых
 * байт данных определяется настройкой длины пакета
 * @return Если 7, то ошибка. Если 0..5 - данные приняты по соответсвенному
 * логическому каналу
 */
int8_t NRF24L01_Read(NRF24L01_t *nrf, uint8_t *buf)
{
    uint8_t status = NRF24L01_ReadReg(nrf, NRF_STATUS);
    if ((status & NRF_CONFIG_RX_DR_M) == 0) return -1;
    NRF24L01_ReadBuf(nrf, RD_RX_PLOAD, buf, nrf->payload_width);
    NRF24L01_WriteReg(nrf, NRF_STATUS, NRF_STATUS_RX_DR_M);
    NRF24L01_FlushRX(nrf);
    //return (status & NRF_STATUS_RX_P_NO_M) >> NRF_STATUS_RX_P_NO;
    //return status;
    return (status >> 1) & 0b111;
}