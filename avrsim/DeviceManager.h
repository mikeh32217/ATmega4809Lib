/* 
* DeviceManager.h
*
* Created: 12/30/2021 5:14:26 AM
* Author: Mike
*/


#ifndef __DEVICEMANAGER_H__
#define __DEVICEMANAGER_H__

#include "CDAC.h"
#include "CADC.h"
#include "CSpi.h"
#include "CPulse.h"
#include "CUart.h"
#include "CMCP23S17.h"
#include "CTimer.h"
#include "CI2C.h"
#include "CRTC.h"

class DeviceManager
{
	private:
		CSpi*	mp_spi;
		CDAC*	mp_dac;
		CADC*	mp_adc;
		CPulse*	mp_pulse;
		CUart*	mp_mspi;
		CMCP23S17* mp_pio;
		CTimer*	mp_timer;
		CI2C*	mp_i2c;
		CRTC*	mp_rtc;
		
	//functions
	public:
		DeviceManager();
		
		CDAC* GetDAC(float vref);
		CADC* GetADC();
		CPulse* GetPulse();
		CUart* GetMSpi(uint16_t buf_size = DEF_BUFFER_SZ);
		CTimer* GetTimer();
		CI2C* GetI2C();
		CRTC* GetRTC();
		
		CMCP23S17*GetPIO();
		void PIO_AttachInterrupt();
		
	private:
		CSpi* GetSpi();

}; //DeviceManager

#endif //__DEVICEMANAGER_H__
