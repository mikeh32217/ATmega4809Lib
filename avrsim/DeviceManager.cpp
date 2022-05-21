 /*!
	 \file DeviceManager.cpp
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
	 
	 \note Use the factory pattern to manage the various classes
	 in this library.
 */
 
#include <avr/io.h>
#include "ErrorCodes.h"
#include "DeviceManager.h"

// default constructor
DeviceManager::DeviceManager()
{
	mp_spi = nullptr;
	mp_dac = nullptr;
	mp_pulse = nullptr;
	mp_mspi = nullptr;
	mp_timer = nullptr;
	mp_i2c = nullptr;
}

CI2C* DeviceManager::GetI2C(uint32_t clk)
{
	if (mp_i2c == nullptr)
		mp_i2c = new CI2C(clk);
	
	return mp_i2c;
}

CDAC* DeviceManager::GetDAC(uint32_t clk, float vref)
{
	if (mp_dac == nullptr)
	{
		mp_spi = GetSpi(clk, 2);			
		mp_spi->ConfigureChannel(DAC_CHANNEL, &PORTA, PIN7_bm);
		mp_spi->ConfigureChannel(DAC_LATCH_CHANNEL, &PORTA, PIN3_bm);

		mp_dac = new CDAC(mp_spi, vref, DAC_CHANNEL, DAC_LATCH_CHANNEL);
	}
	
	return mp_dac; 
}

CADC* DeviceManager::GetADC()
{
	if (mp_adc == nullptr)
		mp_adc = new CADC();
	
	return mp_adc;	
}

CPulse* DeviceManager::GetPulse()
{
	if (mp_pulse == nullptr)
		mp_pulse = new CPulse();

	return mp_pulse; 
}

CMCP23S17*DeviceManager::GetPIO(uint32_t clk)
{
	if(mp_pio == nullptr)
	{
		mp_spi = GetSpi(clk, 2);
		mp_spi->ConfigureChannel(PIO_CHANNEL, &PORTD, PIN0_bm);
		
		mp_pio = new CMCP23S17(PIO_ADDRESS, PIO_CHANNEL, mp_spi);
	}
	
	return mp_pio;
}

CSpi* DeviceManager::GetSpi(uint32_t clk, uint8_t timeout_ms)
{
	if(mp_spi== nullptr)
		mp_spi = new CSpi(clk, timeout_ms);
	
	return mp_spi;
}

CTimer* DeviceManager::GetTimer(uint32_t clk)
{
	if(mp_timer == nullptr)
		mp_timer = new CTimer(clk);
	
	return mp_timer;
}

CRTC* DeviceManager::GetRTC()
{
	if (mp_rtc == nullptr)
		mp_rtc = new CRTC();

	return mp_rtc;
}

CUart* DeviceManager::GetMSpi(uint32_t clk, uint16_t buf_size/* = DEF_BUFFER_SZ*/)
{
	if (mp_mspi == nullptr)
		mp_mspi = new CUart(USART_BAUD_RATE(clk, 9600), true, buf_size);
	
	return mp_mspi;
}

CUart* DeviceManager::GetUart(uint32_t clk, uint32_t baud, uint16_t buf_size/* = DEF_BUFFER_SZ*/)
{
	if (mp_mspi == nullptr)
		mp_mspi = new CUart(USART_BAUD_RATE(clk, baud), false, buf_size);
	
	return mp_mspi;
}



