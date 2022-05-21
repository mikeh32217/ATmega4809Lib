 /*!
	 \file DeviceManager.h
	 \author Mike Hankey
	 \date 12/30/2021
 
	 \copyright
	 Copyright (c) 2014 Mike Hankey <mikeh32217@yahoo.com>

	 Permission is hereby granted, free of charge, to any person
	 obtaining a copy of this software and associated documentation
	 files (the "Software"), to deal in the Software without
	 restriction, including without limitation the rights to use, copy,
	 modify, merge, publish, distribute, sub license, and/or sell copies
	 of the Software, and to permit persons to whom the Software is
	 furnished to do so, subject to the following conditions:

	 The above copyright notice and this permission notice shall be
	 included in all copies or substantial portions of the Software.

	 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	 DEALINGS IN THE SOFTWARE.
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
#include "CUART.h"

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
		
		CDAC* GetDAC(uint32_t clk, float vref);
		CADC* GetADC();
		CPulse* GetPulse();
		CUart* GetMSpi(uint32_t clk, uint16_t buf_size = DEF_BUFFER_SZ);
		CUart* GetUart(uint32_t clk, uint32_t baud, uint16_t buf_size = DEF_BUFFER_SZ);
		CTimer* GetTimer(uint32_t clk);
		CI2C* GetI2C(uint32_t clk);
		CRTC* GetRTC();
		CSpi* GetSpi(uint32_t clk, uint8_t timeout_ms);
		
		CMCP23S17*GetPIO(uint32_t clk);
		void PIO_AttachInterrupt();

}; //DeviceManager

#endif //__DEVICEMANAGER_H__
