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
		float	m_vref;
	
	public:
		CDAC(CSpi* spi, float vref);
		
		void SetVoltage(float volts);
		void Shutdown();
};

#endif /* CDAC_H_ */