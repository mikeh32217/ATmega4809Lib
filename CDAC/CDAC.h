/*
 * CDAC.h
 *
 * Created: 12/30/2021 5:05:01 AM
 *  Author: Mike
 */ 


#ifndef CDAC_H_
#define CDAC_H_

#include "CSpi.h"

class CDAC
{
	private:
		CSpi*	mp_spi;
		uint8_t	m_dac_ch;
		uint8_t m_latch;
		float	m_vref;
	
	public:
		CDAC(CSpi* spi, float vref, uint8_t dac_ch, uint8_t latch);
		
		void SetVoltage(float volts);
		void Shutdown();
};

#endif /* CDAC_H_ */