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
#include "CMCP23S17.h"

class DeviceManager
{
	private:
		CSpi*	mp_spi;
		CDAC*	mp_dac;
		CPulse*	mp_pulse;
		CUart*	mp_mspi;
		CMCP23S17* mp_pio;
		
	//functions
	public:
		DeviceManager();
		
		CDAC* GetDAC(float vref);
		CPulse* GetPulse();
		CUart* GetMSpi(uint16_t buf_size = DEF_BUFFER_SZ);
		CMCP23S17*GetPIO(uint8_t addr, uint8_t chan);
		
	private:
		CSpi* GetSpi();

}; //DeviceManager

#endif //__DEVICEMANAGER_H__
