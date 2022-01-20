/*
 * CDAC.cpp
 *
 * Created: 12/30/2021 5:04:33 AM
 * Author : Mike
 */ 

#include <avr/io.h>
#include "ErrorCodes.h"
#include "CDAC.h"

CDAC::CDAC(CSpi* spi, float vref, uint8_t dac_ch, uint8_t latch)
{
	mp_spi = spi;
	m_vref = vref;
	
	m_dac_ch = dac_ch;
	m_latch = latch;
}

void CDAC::SetVoltage(float volts)
{
	uint16_t dout = (volts / m_vref) * 4096.0f;
		
	mp_spi->SetPinState(m_dac_ch, LOW);
	mp_spi->TransferData((dout >> 8) | 0x70);
	mp_spi->TransferData(dout & 0xff);
	mp_spi->SetPinState(m_dac_ch, HIGH);

	mp_spi->SetPinState(m_latch, LOW);
	asm volatile("NOP");
	mp_spi->SetPinState(m_latch, HIGH);
}

void CDAC::Shutdown()
{
	mp_spi->SetPinState(m_dac_ch, LOW);
	mp_spi->TransferData(0x0);
	mp_spi->TransferData(0x0);
	mp_spi->SetPinState(m_dac_ch, HIGH);
	
	mp_spi->SetPinState(m_latch, LOW);
	asm volatile("NOP");
	mp_spi->SetPinState(m_latch, HIGH);
}

