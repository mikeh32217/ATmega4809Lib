/*
 * CSpi.cpp
 *
 * Created: 1/1/2022 5:40:24 PM
 * Author : Mike
 */ 

#include <avr/io.h>
#include <string.h>
#include "CSpi.h"

CSpi::CSpi()
{
	memset(ms_chanInfo, 0, sizeof(this->ms_chanInfo));
	
	PORTA.DIR |= PIN4_bm; /* Set MOSI pin direction to output */
	PORTA.DIR &= ~PIN5_bm; /* Set MISO pin direction to input */
	PORTA.DIR |= PIN6_bm; /* Set SCK pin direction to output */
		
	/*
	 NOTE: Because I delay setting the SS pin, if the following
	 SSD Client Select Disable is not set in CTRLB then as soon
	 as the SPI is enabled the pin being LOW (or unknown state)
	 causes the SPI to go into SLAVE mode immediately.
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
	
	res.error = NOERROR;
	
	SPI0.DATA = data;
// TODO Add timeout code here
	while (!(SPI0.INTFLAGS & SPI_IF_bm))
	{
		;
	}
	
	res.data = SPI0.DATA;
	
	return res;
}

