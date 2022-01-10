/*
 * CDAC.cpp
 *
 * Created: 12/30/2021 5:04:33 AM
 * Author : Mike
 */ 

#include <avr/io.h>
#include "CDAC.h"

CDAC::CDAC(CSpi* spi)
{
	mp_spi = spi;
	
	mp_spi->ConfigureChannel(DAC_CHANNEL, &PORTA, PIN7_bm);
	mp_spi->ConfigureChannel(DAC_LATCH_CHANNEL, &PORTA, PIN3_bm);
}

uint16_t CDAC::ConvertVoltage(float volts)
{
	// digital value = (volts * resolution) / Vref
	return (volts * 4096.0) / 5.0;
}

void CDAC::SetVoltage(float volts)
{
	uint16_t dout = ConvertVoltage(volts);
		
	mp_spi->SetPinState(DAC_CHANNEL, LOW);
	mp_spi->TransferData((dout >> 8) | 0x70);
	mp_spi->TransferData(dout & 0xff);
	mp_spi->SetPinState(DAC_CHANNEL, HIGH);

	mp_spi->SetPinState(DAC_LATCH_CHANNEL, LOW);
	asm volatile("NOP");
	mp_spi->SetPinState(DAC_LATCH_CHANNEL, HIGH);
}

