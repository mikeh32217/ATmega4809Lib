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
		
		// TODO Add unit to all floats, example 1.82 MS
		void ConfigureOneShot(float width, CS_STATE state = LOW);
		void DisableOneShot();
		
		void SendPulse();
		
	private:
		void ConfigureOneShot(uint16_t width, CS_STATE state = LOW);
};


#endif /* CPULSE_H_ */