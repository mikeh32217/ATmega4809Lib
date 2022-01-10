/* 
* DeviceManager.h
*
* Created: 12/30/2021 5:14:26 AM
* Author: Mike
*/


#ifndef __DEVICEMANAGER_H__
#define __DEVICEMANAGER_H__

#include "CDAC.h"
#include "CSpi.h"
#include "CPulse.h"

class DeviceManager
{
	private:
		CSpi*	mp_spi;
		CDAC*	mp_dac;
		CPulse*	mp_pulse;
		
	//functions
	public:
		DeviceManager();
		
		void SetDacVoltage(float volts);
		
		void ConfigureOneShot(uint16_t width, CS_STATE state = LOW);
		void DisableOneShot();
		void SendPulse();
		
		void ConfigureRepeatPulse(uint16_t period,
								uint16_t pwidth,
								CS_STATE state = LOW);
		void DisableRepeatingPulse();
}; //DeviceManager

#endif //__DEVICEMANAGER_H__
