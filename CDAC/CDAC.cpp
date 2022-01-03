/*
 * CDAC.cpp
 *
 * Created: 12/30/2021 5:04:33 AM
 * Author : Mike
 */ 

#include <avr/io.h>
#include "CDAC.h"

uint16_t CDAC::ConvertVoltage(float volts)
{
	// digital value = (volts * resolution) / Vref
	return (volts * 4096.0) / 5.0;
}

