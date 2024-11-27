#include "NRF24L01.h"

void NRF24L01_RX_Mode(NRF24L01_t *nrf)
{
    uint8_t regval=0x00;
    regval = NRF24_ReadReg(nrf, NRF_CONFIG);
    //разбудим модуль и переведём его в режим приёмника, включив биты PWR_UP и PRIM_RX
    regval |= (1<<PWR_UP)|(1<<PRIM_RX);
    NRF24_WriteReg(nrf, NRF_CONFIG, regval);
    CE_UP(nrf);
    HAL_DelayUs(150); //Задержка минимум 130 мкс
    // Flush buffers
    NRF24_FlushRX(nrf);
    NRF24_FlushTX(nrf);
}

void NRF24L01_TX_Mode(NRF24L01_t *nrf)
{
    NRF24_WriteBuf(nrf, TX_ADDR, (uint8_t*)&(nrf->tx_addr), 3);
    CE_DOWN(nrf);
    // Flush buffers
    NRF24_FlushRX(nrf);
    NRF24_FlushTX(nrf);
}


void NRF24_Init(NRF24L01_t *nrf)
{
    CE_DOWN(nrf);
    HAL_DelayUs(4500);
    NRF24_WriteReg(nrf, NRF_CONFIG, 0b00001010);
    HAL_DelayUs(4500);
    NRF24_WriteReg(nrf, EN_AA, nrf->pipe);
    NRF24_WriteReg(nrf, EN_RXADDR, nrf->pipe);
    //NRF24_WriteReg(nrf, SETUP_AW, 0b11); // address field 5 bytes
    uint8_t setup_aw;
    switch (nrf->address_width)
    {
        case 3: setup_aw = 0b01; break;
        case 4: setup_aw = 0b10; break;
        case 5: setup_aw = 0b11; break;
        default: setup_aw = 0b11;
    }
    NRF24_WriteReg(nrf, SETUP_AW, setup_aw);
    
    NRF24_WriteReg(nrf, SETUP_RETR, 0x5F); // // 1500us, 15 retrans
    NRF24_WriteReg(nrf, FEATURE, 0);
    NRF24_WriteReg(nrf, DYNPD, 0); // dynamic payload length off
    NRF24_WriteReg(nrf, STATUS, 0b01110000); // reset interrupt flags
    NRF24_WriteReg(nrf, RF_CH, nrf->rf_channel); // 2400+76=2476MHz radio freq
    //NRF24_WriteReg(nrf, RF_SETUP, 0x06); // 1Mbps, 0dBm
    NRF24_WriteReg(nrf, RF_SETUP, nrf->rf_datarate | nrf->rf_power);
    NRF24_WriteBuf(nrf, TX_ADDR, (uint8_t*)&(nrf->tx_addr), 5);
    NRF24_WriteBuf(nrf, RX_ADDR_P0, (uint8_t*)&(nrf->tx_addr), 5);
    NRF24_WriteBuf(nrf, RX_ADDR_P1, (uint8_t*)&(nrf->rx_addr), 5);
    NRF24_WriteReg(nrf, RX_PW_P0, nrf->payload_width);
    NRF24_WriteReg(nrf, RX_PW_P1, nrf->payload_width);
    NRF24L01_RX_Mode(nrf);
}


void CE_DOWN(NRF24L01_t *nrf)
{
    HAL_GPIO_WritePin(nrf->ce_port, nrf->ce_pin, 0);
}

void CE_UP(NRF24L01_t *nrf)
{
    HAL_GPIO_WritePin(nrf->ce_port, nrf->ce_pin, 1);
}

uint8_t NRF24_ReadReg(NRF24L01_t *nrf, uint8_t addr)
{
    uint8_t data = 0, cmd;
    HAL_SPI_CS_Enable(nrf->spi, nrf->cs);
    HAL_SPI_Exchange(nrf->spi, &addr, &data, 1, SPI_TIMEOUT_DEFAULT);
    if (addr != STATUS)
    {
        cmd = 0xFF;
        HAL_SPI_Exchange(nrf->spi, &cmd, &data, 1, SPI_TIMEOUT_DEFAULT);
    }
    HAL_SPI_CS_Disable(nrf->spi);
    return data;
}

void NRF24_WriteReg(NRF24L01_t *nrf, uint8_t addr, uint8_t data)
{
    addr |= W_REGISTER;
    uint8_t dummy;
    HAL_SPI_CS_Enable(nrf->spi, nrf->cs);
    HAL_SPI_Exchange(nrf->spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_SPI_Exchange(nrf->spi, &data, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_SPI_CS_Disable(nrf->spi);
}

void NRF24_ReadBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan)
{
    HAL_SPI_CS_Enable(nrf->spi, nrf->cs);
    uint8_t dummy;
    HAL_SPI_Exchange(nrf->spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    dummy = 0xFF;
    for (uint8_t i=0; i<quan; i++)
    {
        HAL_SPI_Exchange(nrf->spi, &dummy, buf+i, 1, SPI_TIMEOUT_DEFAULT);
    }
    HAL_SPI_CS_Disable(nrf->spi);
}

void NRF24_WriteBuf(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan)
{
    addr |= W_REGISTER;
    HAL_SPI_CS_Enable(nrf->spi, nrf->cs);
    uint8_t dummy;
    HAL_SPI_Exchange(nrf->spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_DelayUs(1000);
    for (uint8_t i=0; i<quan; i++)
    {
        HAL_SPI_Exchange(nrf->spi, buf+i, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    }
    HAL_SPI_CS_Disable(nrf->spi);
}

void NRF24_FlushRX(NRF24L01_t *nrf)
{
    HAL_SPI_CS_Enable(nrf->spi, nrf->cs);
    uint8_t cmd = FLUSH_RX, dummy;
    HAL_SPI_Exchange(nrf->spi, &cmd, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_DelayUs(1000);
    HAL_SPI_CS_Disable(nrf->spi);
}

void NRF24_FlushTX(NRF24L01_t *nrf)
{
    HAL_SPI_CS_Enable(nrf->spi, nrf->cs);
    uint8_t cmd = FLUSH_TX, dummy;
    HAL_SPI_Exchange(nrf->spi, &cmd, &dummy, 1, SPI_TIMEOUT_DEFAULT);
    HAL_DelayUs(1000);
    HAL_SPI_CS_Disable(nrf->spi);
}

#include "xprintf.h"
void NRF24_Transmit(NRF24L01_t *nrf, uint8_t addr, uint8_t *buf, uint8_t quan)
{
    CE_DOWN(nrf);
    HAL_SPI_CS_Enable(nrf->spi, nrf->cs);
    uint8_t dummy;
    HAL_SPI_Exchange(nrf->spi, &addr, &dummy, 1, SPI_TIMEOUT_DEFAULT);//отправим адрес в шину
    HAL_DelayUs(1000);
    for (uint8_t i=0; i<quan; i++)
    {
        HAL_SPI_Exchange(nrf->spi, buf+i, &dummy, 1, SPI_TIMEOUT_DEFAULT);//отправим данные в буфер
    }
    HAL_SPI_CS_Disable(nrf->spi);
    CE_UP(nrf);
}

uint8_t NRF24L01_Send(NRF24L01_t *nrf, uint8_t *buf)
{
    uint8_t status=0, regval;
    NRF24L01_TX_Mode(nrf);
    regval = NRF24_ReadReg(nrf, NRF_CONFIG);
    regval |= (1<<PWR_UP);
    regval &= ~(1<<PRIM_RX);
    NRF24_WriteReg(nrf, NRF_CONFIG, regval);
    HAL_DelayUs(150);
    NRF24_Transmit(nrf, WR_TX_PLOAD_NO_ACK, buf, nrf->payload_width);
    CE_UP(nrf);
    HAL_DelayUs(15);
    CE_DOWN(nrf);
    // while ((status & (1<<5)) == 0)
    // {
    //     status = NRF24_ReadReg(nrf, STATUS);
    // }
    regval = NRF24_ReadReg(nrf, OBSERVE_TX);
    return regval;
}

HAL_StatusTypeDef NRF24L01_RX_data_ready(NRF24L01_t *nrf)
{
    uint8_t status = NRF24L01_ReadReg(nrf, STATUS);
    return (status & RX_DR) != 0 ? HAL_OK : HAL_BUSY;
}

HAL_StatusTypeDef NRF24L01_TX_data_sent(NRF24L01_t *nrf)
{
    uint8_t status = NRF24L01_ReadReg(nrf, STATUS);
    return (status & TX_DS) != 0 ? HAL_OK : HAL_BUSY;
}

void NRF24L01_Read(NRF24L01_t *nrf, uint8_t *buf)
{
    NRF24L01_ReadBuf(&nrf, RD_RX_PLOAD, buf, nrf->payload_width);
}