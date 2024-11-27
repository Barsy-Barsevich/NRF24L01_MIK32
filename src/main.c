#include "mik32_hal_usart.h"
#include "mik32_hal_spi.h"
#include "mik32_hal_gpio.h"
#include "mik32_hal_scr1_timer.h"
#include "xprintf.h"
#include "string.h"

// #include "nrf.h"
#include "NRF24L01.h"

/*---------------------------------------------------------------------------------
 * MISO - 0.0
 * MOSI - 0.1
 * SCK  - 0.2
 * CS   - 0.4
 * CE   - 2.7
 *-------------------------------------------------------------------------------*/


USART_HandleTypeDef husart0;
USART_HandleTypeDef husart1;

SPI_HandleTypeDef hspi0;
static void SystemClock_Config(void);
static void USART0_Init();
static void USART1_Init();
static void SPI0_Init();
static void GPIO_Init();

void HAL_DelayUs(uint32_t us)
{
    HAL_Time_SCR1TIM_DelayUs(us);
}
void HAL_DelayMs(uint32_t ms)
{
    HAL_Time_SCR1TIM_DelayMs(ms);
}

//#define NRF_LISTENING
//#define NRF_TRANSMITTING

//#define RECEIVE
#define TRANSMIT

NRF24L01_t nrf;

#ifdef RECEIVE
int main()
{
    SystemClock_Config();
    HAL_Time_SCR1TIM_Init();
    USART0_Init();
    SPI0_Init();
    GPIO_Init();

    nrf.ce_pin = GPIO_PIN_7;
    nrf.ce_port = GPIO_2;
    nrf.cs = SPI_CS_0;
    nrf.payload_width = 2;
    nrf.pipe = PIPE_1;
    nrf.rx_addr = 0xB3B4B5B6F1;
    nrf.tx_addr = 0xB3B4B5B6F1;
    nrf.rf.channel = 76;
    nrf.rf.datarate = RF_DataRate_1Mbps;
    nrf.rf.power = no_attenuation;
    nrf.address_width = 3;
    nrf.spi = &hspi0;

    NRF24L01_Init(&nrf);

    uint8_t dt_reg = 0;
    uint8_t buf1[10];
    while (1)
    {
        HAL_DelayMs(1000);
        dt_reg = NRF24L01_ReadReg(&nrf, NRF_CONFIG);
        xprintf("NRF_CONFIG: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, EN_AA);
        xprintf("EN_AA: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, EN_RXADDR);
        xprintf("EN_RXADDR: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, STATUS);
        xprintf("STATUS: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, RF_SETUP);
        xprintf("RF_SETUP: 0x%02X\n", dt_reg);
        NRF24L01_ReadBuf(&nrf, TX_ADDR, buf1, 3);
        xprintf("TX_ADDR: 0x%02X, 0x%02X, 0x%02X\n",buf1[0],buf1[1],buf1[2]);
        NRF24L01_ReadBuf(&nrf, RX_ADDR_P1, buf1, 3);
        xprintf("RX_ADDR: 0x%02X, 0x%02X, 0x%02X\n",buf1[0],buf1[1],buf1[2]);

        //NRF24L01_ReadBuf(&nrf, RD_RX_PLOAD, buf1, 3);
        NRF24L01_Read(&nrf, buf1);
        xprintf("Read: %02X-%02X-%02X\n\n", buf1[0], buf1[1], buf1[2]);
    }
}
#endif
#ifdef TRANSMIT
int main()
{
    SystemClock_Config();
    HAL_Time_SCR1TIM_Init();
    USART0_Init();
    SPI0_Init();
    GPIO_Init();

    nrf.ce_pin = GPIO_PIN_7;
    nrf.ce_port = GPIO_2;
    nrf.cs = SPI_CS_0;
    nrf.payload_width = 2;
    nrf.pipe = PIPE_0;
    nrf.rx_addr = 0xB3B4B5B6F1;
    nrf.tx_addr = 0xB3B4B5B6F1;
    nrf.rf.channel = 76;
    nrf.rf.datarate = RF_DataRate_1Mbps;
    nrf.rf.power = no_attenuation;
    nrf.address_width = 3;
    nrf.spi = &hspi0;

    NRF24L01_Init(&nrf);

    uint8_t dt_reg = 0;
    uint8_t buf1[10];
    while (1)
    {
        HAL_DelayMs(500);
        xprintf("Trans-zhmans\n");
        uint8_t lula[] = {0x34, 0x02};
        uint8_t status = NRF24L01_Send(&nrf, lula);
        xprintf("status: %02X\n", status);
        HAL_DelayMs(500);
        dt_reg = NRF24L01_ReadReg(&nrf, NRF_CONFIG);
        xprintf("NRF_CONFIG: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, EN_AA);
        xprintf("EN_AA: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, EN_RXADDR);
        xprintf("EN_RXADDR: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, STATUS);
        xprintf("STATUS: 0x%02X\n", dt_reg);
        dt_reg = NRF24L01_ReadReg(&nrf, RF_SETUP);
        xprintf("RF_SETUP: 0x%02X\n", dt_reg);
        NRF24L01_ReadBuf(&nrf, TX_ADDR, buf1, 3);
        xprintf("TX_ADDR: 0x%02X, 0x%02X, 0x%02X\n",buf1[0],buf1[1],buf1[2]);
        NRF24L01_ReadBuf(&nrf, RX_ADDR_P0, buf1, 3);
        xprintf("RX_ADDR: 0x%02X, 0x%02X, 0x%02X\n\n",buf1[0],buf1[1],buf1[2]);

        NRF24L01_WriteReg(&nrf, STATUS, 0x10);
    }
}
#endif



static void SystemClock_Config(void)
{
    PCC_InitTypeDef PCC_OscInit = {0};

    PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL;
    PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
    PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED;
    PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K;
    PCC_OscInit.AHBDivider = 0;
    PCC_OscInit.APBMDivider = 0;
    PCC_OscInit.APBPDivider = 0;
    PCC_OscInit.HSI32MCalibrationValue = 128;
    PCC_OscInit.LSI32KCalibrationValue = 128;
    PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO;
    PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;
    HAL_PCC_Config(&PCC_OscInit);
}


static void USART0_Init()
{
    husart0.Instance = UART_0;
    husart0.transmitting = Enable;
    husart0.receiving = Enable;
    husart0.frame = Frame_8bit;
    husart0.parity_bit = Disable;
    husart0.parity_bit_inversion = Disable;
    husart0.bit_direction = LSB_First;
    husart0.data_inversion = Disable;
    husart0.tx_inversion = Disable;
    husart0.rx_inversion = Disable;
    husart0.swap = Disable;
    husart0.lbm = Disable;
    husart0.stop_bit = StopBit_1;
    husart0.mode = Asynchronous_Mode;
    husart0.xck_mode = XCK_Mode3;
    husart0.last_byte_clock = Disable;
    husart0.overwrite = Disable;
    husart0.rts_mode = AlwaysEnable_mode;
    husart0.dma_tx_request = Disable;
    husart0.dma_rx_request = Disable;
    husart0.channel_mode = Duplex_Mode;
    husart0.tx_break_mode = Disable;
    husart0.Interrupt.ctsie = Disable;
    husart0.Interrupt.eie = Disable;
    husart0.Interrupt.idleie = Disable;
    husart0.Interrupt.lbdie = Disable;
    husart0.Interrupt.peie = Disable;
    husart0.Interrupt.rxneie = Disable;
    husart0.Interrupt.tcie = Disable;
    husart0.Interrupt.txeie = Disable;
    husart0.Modem.rts = Disable; //out
    husart0.Modem.cts = Disable; //in
    husart0.Modem.dtr = Disable; //out
    husart0.Modem.dcd = Disable; //in
    husart0.Modem.dsr = Disable; //in
    husart0.Modem.ri = Disable;  //in
    husart0.Modem.ddis = Disable;//out
    husart0.baudrate = 9600;
    HAL_USART_Init(&husart0);
}


static void USART1_Init()
{
    husart1.Instance = UART_1;
    husart1.transmitting = Enable;
    husart1.receiving = Enable;
    husart1.frame = Frame_8bit;
    husart1.parity_bit = Disable;
    husart1.parity_bit_inversion = Disable;
    husart1.bit_direction = LSB_First;
    husart1.data_inversion = Disable;
    husart1.tx_inversion = Disable;
    husart1.rx_inversion = Disable;
    husart1.swap = Disable;
    husart1.lbm = Disable;
    husart1.stop_bit = StopBit_1;
    husart1.mode = Asynchronous_Mode;
    husart1.xck_mode = XCK_Mode3;
    husart1.last_byte_clock = Disable;
    husart1.overwrite = Disable;
    husart1.rts_mode = AlwaysEnable_mode;
    husart1.dma_tx_request = Disable;
    husart1.dma_rx_request = Disable;
    husart1.channel_mode = Duplex_Mode;
    husart1.tx_break_mode = Disable;
    husart1.Interrupt.ctsie = Disable;
    husart1.Interrupt.eie = Disable;
    husart1.Interrupt.idleie = Disable;
    husart1.Interrupt.lbdie = Disable;
    husart1.Interrupt.peie = Disable;
    husart1.Interrupt.rxneie = Disable;
    husart1.Interrupt.tcie = Disable;
    husart1.Interrupt.txeie = Disable;
    husart1.Modem.rts = Disable; //out
    husart1.Modem.cts = Disable; //in
    husart1.Modem.dtr = Disable; //out
    husart1.Modem.dcd = Disable; //in
    husart1.Modem.dsr = Disable; //in
    husart1.Modem.ri = Disable;  //in
    husart1.Modem.ddis = Disable;//out
    husart1.baudrate = 115200;
    HAL_USART_Init(&husart1);
}


static void SPI0_Init(void)
{
    __HAL_PCC_SPI_0_CLK_ENABLE();
    hspi0.Instance = SPI_0;
    /* Режим SPI */
    hspi0.Init.SPI_Mode = HAL_SPI_MODE_MASTER;
    /* Настройки */
    hspi0.Init.CLKPhase = SPI_PHASE_OFF;
    hspi0.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi0.Init.ThresholdTX = 4;
    /* Настройки для ведущего */
    hspi0.Init.BaudRateDiv = SPI_BAUDRATE_DIV64;
    hspi0.Init.Decoder = SPI_DECODER_NONE;
    hspi0.Init.ManualCS = SPI_MANUALCS_ON;
    hspi0.Init.ChipSelect = SPI_CS_0;      
    if (HAL_SPI_Init(&hspi0) != HAL_OK)
    {
    }
    __HAL_SPI_ENABLE(&hspi0);
}


static void GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_2_CLK_ENABLE();
    __HAL_PCC_GPIO_IRQ_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(GPIO_2, &GPIO_InitStruct);
}