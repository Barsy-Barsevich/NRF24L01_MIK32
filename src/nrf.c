#include "nrf.h"



void NRF_begin(void)
{
    // Initialize SPI bus
    NRF_SPI_Init();

    NRF_CE_DOWN();

    // Must allow the radio time to settle else configuration bits will not necessarily stick.
    // This is actually only required following power up but some settling time also appears to
    // be required after resets too. For full coverage, we'll always assume the worst.
    // Enabling 16b CRC is by far the most obvious case if the wrong timing is used - or skipped.
    // Technically we require 4.5ms + 14us as a worst case. We'll just call it 5ms for good measure.
    // WARNING: Delay is based on P-variant whereby non-P *may* require different timing.
    HAL_DelayMs(5);

    // Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
    // WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
    // sizes must never be used. See documentation for a more complete explanation.
    NRF_writeRegister(NRF_SETUP_RETR, (0b0100 << NRF_SETUP_RETR_ARD) | (0b1111 << NRF_SETUP_RETR_ARC));

    // Restore our default PA level
    //setPALevel( RF24_PA_MAX ) ;

    // Determine if this is a p or non-p RF24 module and then
    // reset our data rate back to default value. This works
    // because a non-P variant won't allow the data rate to
    // be set to 250Kbps.
    //if( setDataRate( RF24_250KBPS ) )
    //{
    //    p_variant = true ;
    //}
  
    // Then set the data rate to the slowest (and most reliable) speed supported by all
    // hardware.
    //setDataRate( RF24_1MBPS ) ;

    // Initialize CRC and request 2-byte (16bit) CRC
    //setCRCLength(RF24_CRC_16);
  
    // Disable dynamic payloads, to match dynamic_payloads_enabled setting
    NRF_writeRegister(NRF_DYNPD, 0);

    // Reset current status
    // Notice reset and flush is the last thing we do
    NRF_writeRegister(NRF_STATUS, 1<<NRF_CONFIG_MASK_RX_DR | 1<<NRF_CONFIG_MASK_TX_DS | 1<<NRF_CONFIG_MASK_MAX_RT);

    // Set up default configuration.  Callers can always change it later.
    // This channel should be universally safe and not bleed over into adjacent
    // spectrum.
    NRF_writeRegister(NRF_RF_CH, 76);
}


void NRF_startListening()
{
    HAL_GPIO_WritePin(GPIO_2, GPIO_PIN_7, 0);
    NRF_writeRegister(NRF_CONFIG, (1<<NRF_CONFIG_PWR_UP) | (1<<NRF_CONFIG_PRIM_RX));
    HAL_GPIO_WritePin(GPIO_2, GPIO_PIN_7, 1);
}


void NRF_stopListening()
{
    NRF_writeRegister(NRF_CONFIG, (1<<NRF_CONFIG_PWR_UP));
    HAL_GPIO_WritePin(GPIO_2, GPIO_PIN_7, 0);
}


void NRF_transmit(uint8_t* payload, uint8_t quan)
{
    NRF_writePayload(payload, quan);
    NRF_CE_UP();
    HAL_DelayMs(10);
    NRF_CE_DOWN();
}


bool NRF_dataReadyCheck()
{
    uint8_t status = NRF_getStatus();
    bool res = (status & (1<<NRF_STATUS_RX_DR)) != 0;
    //if (res) NRF_writeRegister(NRF_STATUS, (1<<NRF_STATUS_RX_DR));
    return res;
}


void NRF_dataRead(uint8_t* buf)
{
    while(!NRF_dataReadyCheck());
    NRF_readPayload(buf, 32);
}
