/* 
* DeviceManager.cpp
*
* Created: 12/30/2021 5:14:26 AM
* Author: Mike
*/
#define F_CPU 20000000UL

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
}

CDAC* DeviceManager::GetDAC(float vref)
{
	if (mp_dac == nullptr)
	{
		mp_spi = GetSpi();			
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

CUart* DeviceManager::GetMSpi(uint16_t buf_size/* = DEF_BUFFER_SZ*/)
{
	if (mp_mspi == nullptr)
		mp_mspi = new CUart(USART_BAUD_RATE(F_CPU, 9600), true, buf_size);
		
	return mp_mspi;
}

CMCP23S17*DeviceManager::GetPIO()
{
	if(mp_pio == nullptr)
	{
		mp_spi = GetSpi();
		mp_spi->ConfigureChannel(PIO_CHANNEL, &PORTD, PIN0_bm);
		
		mp_pio = new CMCP23S17(PIO_ADDRESS, PIO_CHANNEL, mp_spi);
	}
	
	return mp_pio;
}

CSpi* DeviceManager::GetSpi()
{
	if(mp_spi== nullptr)
		mp_spi = new CSpi();
	
	return mp_spi;
}

CTimer* DeviceManager::GetTimer()
{
	if(mp_timer == nullptr)
		mp_timer = new CTimer();
	
	return mp_timer;
}

