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

#define DAC_CHANNEL			0
#define DAC_LATCH_CHANNEL	1

class DeviceManager
{
	private:
		CSpi*	mp_spi;
		CDAC*	mp_dac;
		
	//functions
	public:
		DeviceManager();
		
		void SetDacVoltage(float volts);

}; //DeviceManager

#endif //__DEVICEMANAGER_H__
