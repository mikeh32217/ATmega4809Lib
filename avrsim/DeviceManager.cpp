/* 
* DeviceManager.cpp
*
* Created: 12/30/2021 5:14:26 AM
* Author: Mike
*/
#include <avr/io.h>
#include "_cplusplus.h"
#include "DeviceManager.h"

// default constructor
DeviceManager::DeviceManager()
{
	mp_spi = new CSpi;
	mp_dac = new CDAC;

	mp_spi->ConfigureChannel(DAC_CHANNEL, &PORTA, PIN7_bm);
	mp_spi->ConfigureChannel(DAC_LATCH_CHANNEL, &PORTA, PIN3_bm);
}

void DeviceManager::SetDacVoltage(float volts)
{
	uint16_t dout = mp_dac->ConvertVoltage(volts);
	
	mp_spi->SetPinState(DAC_CHANNEL, LOW);
	//PORTA.OUT &= ~PIN7_bm; // Set SS pin value to LOW
	mp_spi->TransferData((dout >> 8) | 0x70);
	mp_spi->TransferData(dout & 0xff);
	//SPI0_exchangeData(data >> 8);
	//SPI0_exchangeData(data & 0xff);
	//PORTA.OUT |= PIN7_bm; // Set SS pin value to HIGH
	mp_spi->SetPinState(DAC_CHANNEL, HIGH);

	mp_spi->SetPinState(DAC_LATCH_CHANNEL, LOW);
	//PORTA.OUT |= PIN3_bm; // Toggle LATCH
	asm volatile("NOP");
	//PORTA.OUT &= ~PIN3_bm;
	mp_spi->SetPinState(DAC_LATCH_CHANNEL, HIGH);
}


