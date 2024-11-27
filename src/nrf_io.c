// #include "nrf.h"

// SPI_HandleTypeDef hspi0;
// static void SPI0_Init();
// static void GPIO_Init();


// void NRF_SPI_Init()
// {
//     SPI0_Init();
//     HAL_SPI_CS_Disable(&hspi0);
//     GPIO_Init();
//     HAL_GPIO_WritePin(GPIO_2, GPIO_PIN_7, 0);
// }

// void NRF_readRegister(uint8_t addr, uint8_t* buf, uint8_t quan)
// {
//     uint8_t dummy, data;
//     HAL_SPI_CS_Enable(&hspi0, SPI_CS_0);
//     data = NRF_COM_R_REGISTER | (addr & 0b11111);
//     HAL_SPI_Exchange(&hspi0, &data, &dummy, 1, SPI_TIMEOUT_DEFAULT);
//     //dummy is containing status data now
//     data = 0xFF;
//     for (uint8_t i=0; i<quan; i++)
//     {
//         HAL_SPI_Exchange(&hspi0, &data, buf+i, 1, SPI_TIMEOUT_DEFAULT);
//     }
//     HAL_SPI_CS_Disable(&hspi0);
// }

// void NRF_writeRegister(uint8_t addr, uint8_t buf)
// {
//     uint8_t dummy, data;
//     HAL_SPI_CS_Enable(&hspi0, SPI_CS_0);
//     data = NRF_COM_W_REGISTER | (addr & 0b11111);
//     HAL_SPI_Exchange(&hspi0, &data, &dummy, 1, SPI_TIMEOUT_DEFAULT);
//     //dummy is containing status data now
//     HAL_SPI_Exchange(&hspi0, &buf, &dummy, 1, SPI_TIMEOUT_DEFAULT);
//     HAL_SPI_CS_Disable(&hspi0);
// }

// void NRF_readPayload(uint8_t* buf, uint8_t quan)
// {
//     uint8_t dummy, data;
//     HAL_SPI_CS_Enable(&hspi0, SPI_CS_0);
//     data = NRF_COM_R_RX_PAYLOAD;
//     HAL_SPI_Exchange(&hspi0, &data, &dummy, 1, SPI_TIMEOUT_DEFAULT);
//     //dummy is containing status data now
//     data = 0xFF;
//     for (uint8_t i=0; i<quan; i++)
//     {
//         HAL_SPI_Exchange(&hspi0, &data, buf+i, 1, SPI_TIMEOUT_DEFAULT);
//     }
//     HAL_SPI_CS_Disable(&hspi0);
// }

// void NRF_writePayload(uint8_t* buf, uint8_t quan)
// {
//     uint8_t dummy, data;
//     HAL_SPI_CS_Enable(&hspi0, SPI_CS_0);
//     data = NRF_COM_W_TX_PAYLOAD;
//     HAL_SPI_Exchange(&hspi0, &data, &dummy, 1, SPI_TIMEOUT_DEFAULT);
//     //dummy is containing status data now
//     for (uint8_t i=0; i<quan; i++)
//     {
//         HAL_SPI_Exchange(&hspi0, buf+i, &dummy, 1, SPI_TIMEOUT_DEFAULT);
//     }
//     HAL_SPI_CS_Disable(&hspi0);
// }

// uint8_t NRF_getStatus()
// {
//     uint8_t dummy, data;
//     HAL_SPI_CS_Enable(&hspi0, SPI_CS_0);
//     data = NRF_COM_NOP;
//     HAL_SPI_Exchange(&hspi0, &data, &dummy, 1, SPI_TIMEOUT_DEFAULT);
//     HAL_SPI_CS_Disable(&hspi0);
//     return dummy;
// }


// void NRF_CE_UP()
// {
//     HAL_GPIO_WritePin(GPIO_2, GPIO_PIN_7, 1);
// }


// void NRF_CE_DOWN()
// {
//     HAL_GPIO_WritePin(GPIO_2, GPIO_PIN_7, 0);
// }



// static void SPI0_Init(void)
// {
//     __HAL_PCC_SPI_0_CLK_ENABLE();
//     hspi0.Instance = SPI_0;
//     /* Режим SPI */
//     hspi0.Init.SPI_Mode = HAL_SPI_MODE_MASTER;
//     /* Настройки */
//     hspi0.Init.CLKPhase = SPI_PHASE_ON;
//     hspi0.Init.CLKPolarity = SPI_POLARITY_HIGH;
//     hspi0.Init.ThresholdTX = 4;
//     /* Настройки для ведущего */
//     hspi0.Init.BaudRateDiv = SPI_BAUDRATE_DIV64;
//     hspi0.Init.Decoder = SPI_DECODER_NONE;
//     hspi0.Init.ManualCS = SPI_MANUALCS_ON;
//     hspi0.Init.ChipSelect = SPI_CS_0;      
//     if (HAL_SPI_Init(&hspi0) != HAL_OK)
//     {
//     }
//     __HAL_SPI_ENABLE(&hspi0);
// }


// static void GPIO_Init()
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     __HAL_PCC_GPIO_0_CLK_ENABLE();
//     __HAL_PCC_GPIO_1_CLK_ENABLE();
//     __HAL_PCC_GPIO_2_CLK_ENABLE();
//     __HAL_PCC_GPIO_IRQ_CLK_ENABLE();

//     GPIO_InitStruct.Pin = GPIO_PIN_7;
//     GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
//     GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
//     HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);
// }