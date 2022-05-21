/*
 * CDAC.cpp
 *
 * Created: 12/30/2021 5:04:33 AM
 * Author : Mike
 */ 
 /*!
 \file file.x
 \author Mike Hankey
 \date 2/12/2020
 
 \copyright
 Copyright (c) 2014 Mike Hankey <mikeh32217@yahoo.com>

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use, copy,
 modify, merge, publish, distribute, sub license, and/or sell copies
 of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.

 \note This class used the MCP4921 DAC as as way to show how the
	CSPI library may be used.
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

