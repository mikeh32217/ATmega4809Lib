/*
 * CSpi.h
 *
 * Created: 1/1/2022 5:40:48 PM
 *  Author: Mike
 */ 


#ifndef CSPI_H_
#define CSPI_H_

#include "Common.h"

#define MAX_CHANNELS	5

#define NOERROR		0
#define SPI_TIMEOUT	0x01

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
	
	public:
		CSpi();
		
		bool ConfigureChannel(uint8_t ch, PORT_t* port, uint8_t pin, CS_STATE initial = HIGH);
		void SetPinState(uint8_t ch, CS_STATE state);
		XFER_RESULT TransferData(uint8_t data);
};


#endif /* CSPI_H_ */