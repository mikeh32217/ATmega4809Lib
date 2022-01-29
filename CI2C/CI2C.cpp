/*
 * CI2C.cpp
 *
 * Created: 1/26/2022 9:53:20 AM
 * Author : Mike
 */ 

#include <avr/io.h>
#include "CI2C.h"

CI2C::CI2C()
{
    TWI0.MBAUD = (uint8_t)TWI0_BAUD(I2C_SCL_FREQ, 0);

    TWI0.MCTRLA = TWI_ENABLE_bm;
    
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

uint8_t CI2C::Start(uint8_t addr, uint8_t dir)
{
	uint8_t res = 0;
	
	// Issues a Start and depending on results will
	//	shift out address.  Bit 0 is the R/W direction bit.
    TWI0.MADDR = (addr << 1) + dir;
	
	// Check status register for results;
	//	Bit 3 - ARBLOST Arbitration lost
	//	Bit 2 - BUSERR Illegal bus error
	res = TWI0.MSTATUS & I2C_STATUS_MASK_gc;
	if (res != 0)
	{
		SendCommand(TWI_MCMD_STOP_gc);
		return 1;
	}
	
	while (!(TWI_RXACK_bm & TWI0.MSTATUS)){}	
		
	return res;
}

void CI2C::SendCommand(uint8_t cmd)
{
	TWI0.MCTRLB |=  cmd;
}

uint8_t CI2C::ReceiveData(void)
{
    while  (!(TWI_RIF_bm & TWI0.MSTATUS)) {}

    return TWI0.MDATA;	
}

void CI2C::SendData(uint8_t addr, uint8_t dir)
{
    TWI0.MADDR = (1 << addr) + dir;
    while (!(TWI_RXACK_bm & TWI0.MSTATUS)) {}	
}
