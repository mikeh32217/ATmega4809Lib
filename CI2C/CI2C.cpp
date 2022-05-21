
 /*!
	 \file CI2C.cpp
	 \author Mike Hankey
	 \date 5/11/2022
 
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

	 \note Assumptions;
		1) Once the Start has been issued and the host has control of
			the bus it will continue transmitting or receiving unless
			an error occurs at which time the exchange terminates.
 */

#include <avr/io.h>
#include "CI2C.h"

CI2C::CI2C(uint32_t clk)
{
	TWI0.MCTRLA = TWI_WIEN_bm | TWI_ENABLE_bm;

    /* Host Baud Rate Control */
    TWI0.MBAUD = TWI0_MBAUD = TWI0_BAUD(clk, (I2C_SCL_FREQ), 0.25);
	
    /* Set bus state idle */
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
	TWI0.MSTATUS |= TWI_RIF_bm | TWI_WIF_bm;
}

uint8_t CI2C::Start(uint8_t baseAddres, uint8_t directionBit)
{
	TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm);						// clear Read and Write interrupt flags
	
	if (directionBit == I2C_DIRECTION_BIT_WRITE)
	{
		TWI0_MADDR = baseAddres & ~I2C_DIRECTION_BIT_WRITE;
		while (!(TWI0.MSTATUS & TWI_WIF_bm));
	}
	else
	{
		TWI0_MADDR = baseAddres | I2C_DIRECTION_BIT_READ;		
		while (!(TWI0.MSTATUS & TWI_RIF_bm));
	}
		
	if ((TWI0_MSTATUS & TWI_ARBLOST_bm)) 
		return 0 ;         //return 0 if bus error or arbitration lost
		
	return !(TWI0_MSTATUS & TWI_RXACK_bm);  //return 1 if slave gave an ack
}

uint8_t CI2C::ReceiveData(uint8_t* data, uint8_t len)
{
	uint8_t cnt = 0;
	
	for (uint8_t i = 0; i < len; i++)
	{
		while (!(TWI0_MSTATUS & TWI_RIF_bm));               
		*(data + i) = TWI0.MDATA;
	
		cnt++;
		
		if (i < (len - 1)) 
			TWI0_MCTRLB = (TWI_ACKACT_ACK_gc  | TWI_MCMD_RECVTRANS_gc); 
		else                
			TWI0_MCTRLB = (TWI_ACKACT_NACK_gc | TWI_MCMD_RECVTRANS_gc); 
	}
    return cnt;
}

uint8_t CI2C::SendData(uint8_t* data, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		TWI0_MDATA = *(data + i);
	
		while (!(TWI0.MSTATUS & TWI_WIF_bm));
		
		if ((TWI0_MSTATUS & TWI_RXACK_bm) != 0)
		{
			TWI0_MCTRLB |= TWI_MCMD_STOP_gc;
			return 0;
		}
		else
			TWI0_MCTRLB |= TWI_MCMD_RECVTRANS_gc;		
	}
	return 1;
}

void CI2C::Flush()
{
    TWI0_MCTRLB = TWI_FLUSH_bm;	
}

void CI2C::Stop(void)
{
	TWI0_MCTRLB = (TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc);
}


