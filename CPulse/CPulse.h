/*
 * CPulse.h
 *
 * Created: 1/3/2022 12:25:28 PM
 *  Author: Mike
 */ 


#ifndef CPULSE_H_
#define CPULSE_H_

#include "Common.h"

class CPulse
{
	private:
		CS_STATE m_state;
		
	public:	
		CPulse();
		
		void ConfigureOneShot(uint16_t width, CS_STATE state = LOW);
		void ConfigureOneShot(float width, CS_STATE state = LOW);
		void DisableOneShot();
		void SendPulse();
		
		void ConfigureRepeatingPulse(uint16_t period,
									uint16_t pwidth, 
									CS_STATE state = LOW);
		void ConfigureRepeatingPulse(float period,
									float pwidth,
									CS_STATE state = LOW);
		void DisableRepeatingPulse();
		
	private:
		void ConfigureTimerA(uint16_t period);
		void ConfigureTimerB(uint16_t pwidth, CS_STATE state = LOW );
};


#endif /* CPULSE_H_ */