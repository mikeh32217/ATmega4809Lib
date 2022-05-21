 /*!
	 \file CSpi.h
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

#ifndef CSPI_H_
#define CSPI_H_

#include "Common.h"

#define MAX_CHANNELS	5

typedef struct 
{
	bool	m_active;
	PORT_t* mp_port;
	uint8_t	m_pin;
} CHAN_INFO;

typedef struct 
{
	uint8_t	data;
	uint8_t	error;
} XFER_RESULT;

class CSpi
{
	private:
		CHAN_INFO	ms_chanInfo[MAX_CHANNELS];
		uint32_t	m_milli;
		uint32_t	m_timeout;
	
	public:
		CSpi(uint32_t clk, uint8_t timeout);
		
		bool ConfigureChannel(uint8_t ch, PORT_t* port, uint8_t pin, CS_STATE initial = HIGH);
		void SetPinState(uint8_t ch, CS_STATE state);
		XFER_RESULT TransferData(uint8_t data);
};


#endif /* CSPI_H_ */