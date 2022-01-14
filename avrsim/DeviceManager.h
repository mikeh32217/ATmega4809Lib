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
#include "CUart.h"

class DeviceManager
{
	private:
		CSpi*	mp_spi;
		CDAC*	mp_dac;
		CPulse*	mp_pulse;
		CUart*	mp_mspi;
		
	//functions
	public:
		DeviceManager();
		
		CDAC* GetDAC(float vref);
		CPulse* GetPulse();
		CUart* GetMSpi(uint16_t buf_size = DEF_BUFFER_SZ);

}; //DeviceManager

#endif //__DEVICEMANAGER_H__
