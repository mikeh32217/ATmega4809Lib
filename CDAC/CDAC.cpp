/*
 * CDAC.cpp
 *
 * Created: 12/30/2021 5:04:33 AM
 * Author : Mike
 */ 

#include <avr/io.h>
#include "CDAC.h"

CDAC::CDAC(CSpi* spi, float vref)
{
	mp_spi = spi;
	m_vref = vref;
	
	mp_spi->ConfigureChannel(DAC_CHANNEL, &PORTA, PIN7_bm);
	mp_spi->ConfigureChannel(DAC_LATCH_CHANNEL, &PORTA, PIN3_bm);
}

void CDAC::SetVoltage(float volts)
{
	uint16_t dout = (volts / m_vref) * 4096.0f;
		
	mp_spi->SetPinState(DAC_CHANNEL, LOW);
	mp_spi->TransferData((dout >> 8) | 0x70);
	mp_spi->TransferData(dout & 0xff);
	mp_spi->SetPinState(DAC_CHANNEL, HIGH);

	mp_spi->SetPinState(DAC_LATCH_CHANNEL, LOW);
	asm volatile("NOP");
	mp_spi->SetPinState(DAC_LATCH_CHANNEL, HIGH);
}

