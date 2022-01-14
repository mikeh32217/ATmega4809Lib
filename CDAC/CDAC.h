/*
 * CDAC.h
 *
 * Created: 12/30/2021 5:05:01 AM
 *  Author: Mike
 */ 


#ifndef CDAC_H_
#define CDAC_H_

#include "CSpi.h"

#define DAC_CHANNEL			0
#define DAC_LATCH_CHANNEL	1

class CDAC
{
	private:
		CSpi*	mp_spi;
		float	m_vref;
	
	public:
		CDAC(CSpi* spi, float vref);
		
		void SetVoltage(float volts);
};

#endif /* CDAC_H_ */