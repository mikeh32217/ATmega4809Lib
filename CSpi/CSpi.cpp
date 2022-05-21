 /*!
	 \file CSpi.cpp
	 \author Mike Hankey
	 \date 1/1/2022
 
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
 */

#include <avr/io.h>
#include <string.h>
#include "ErrorCodes.h"
#include "CSpi.h"

CSpi::CSpi(uint32_t clk, uint8_t timeout_ms)
{
	m_milli = .001 / (1 / clk);
	m_timeout = timeout_ms * m_milli;
	
	memset(ms_chanInfo, 0, sizeof(this->ms_chanInfo));
	
	PORTA.DIR |= PIN4_bm; /* Set MOSI pin direction to output */
	PORTA.DIR &= ~PIN5_bm; /* Set MISO pin direction to input */
	PORTA.DIR |= PIN6_bm; /* Set SCK pin direction to output */
		
	/*
	 NOTE: Because I delay setting the SS pin, if the following
	 SSD Client Select Disable is not set in CTRLB then as soon
	 as the SPI is enabled the pin being LOW (or unknown state)
	 causes the SPI to go into CLIENT mode immediately.
	*/
	SPI0.CTRLB = SPI_SSD_bm;
	SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm;
}

void CSpi::SetPinState(uint8_t ch, CS_STATE state)
{
	if (state)
		ms_chanInfo[ch].mp_port->OUTSET = ms_chanInfo[ch].m_pin;
	else
		ms_chanInfo[ch].mp_port->OUTCLR = ms_chanInfo[ch].m_pin;	
}

bool CSpi::ConfigureChannel(uint8_t ch, PORT_t* port, uint8_t pin, CS_STATE initial/* = HIGH*/)
{
	if (ch >= MAX_CHANNELS || ms_chanInfo[ch].m_active)
		return false;
		
	ms_chanInfo[ch].m_active = true;
	ms_chanInfo[ch].mp_port = port;
	ms_chanInfo[ch].m_pin = pin;

	port->DIRSET = pin;
	
	if (initial == HIGH)
		SetPinState(ch, HIGH);
	else
		SetPinState(ch, LOW);
			
	return true;
}

XFER_RESULT CSpi::TransferData(uint8_t data)
{
	XFER_RESULT res;
	uint32_t cnt = 0;
	
	res.error = NOERROR;
	
	SPI0.DATA = data;
	
	while (!(SPI0.INTFLAGS & SPI_IF_bm))
	{
		if (cnt++ >= m_timeout)
		{
			res.error = SPI_TIMEOUT;
		}
	}
	
	res.data = SPI0.DATA;
	
	return res;
}

