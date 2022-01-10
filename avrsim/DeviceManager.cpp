/* 
* DeviceManager.cpp
*
* Created: 12/30/2021 5:14:26 AM
* Author: Mike
*/
#include <avr/io.h>
#include "DeviceManager.h"

// default constructor
DeviceManager::DeviceManager()
{
	mp_spi = new CSpi;
	mp_dac = new CDAC(mp_spi);
	mp_pulse = new CPulse();
}

void DeviceManager::SetDacVoltage(float volts)
{
	mp_dac->SetVoltage(volts);
}

void DeviceManager::ConfigureOneShot(uint16_t width, CS_STATE state /* = LOW */)
{
	mp_pulse->ConfigureOneShot(width, state);
}

void DeviceManager::ConfigureRepeatPulse(uint16_t period,
										uint16_t pwidth,
										CS_STATE state/* = LOW*/)
{
	mp_pulse->ConfigureRepeatingPulse(period, pwidth, state);
}

void DeviceManager::DisableOneShot()
{
	mp_pulse->DisableOneShot();
}

void DeviceManager::DisableRepeatingPulse()
{
	mp_pulse->DisableRepeatingPulse();
}

void DeviceManager::SendPulse()
{
	mp_pulse->SendPulse();
}


