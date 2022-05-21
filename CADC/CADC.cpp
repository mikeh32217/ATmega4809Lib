 /*!
	 \file CADC.cpp
	 \author Mike Hankey
	 \date 1/13/2022
 
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

#include <avr/io.h>
#include "ErrorCodes.h"
#include "CADC.h"

CADC::CADC()
{
	// Because I'm running at 20Mhz I need to prescale by 16
	// Also because voltage ref so high need the sampling cap
	// Using VDD as voltage reference
	ADC0.CTRLC = ADC_PRESC_DIV16_gc
	| ADC_SAMPCAP_bm
	| ADC_REFSEL_VDDREF_gc;
		
	// Sampling average 4 times
	ADC0.CTRLB = ADC_SAMPNUM_ACC4_gc;
	
	/* ADC Enable: enabled */	
	ADC0.CTRLA |= ADC_ENABLE_bm; 
}

uint8_t CADC::ConfigureChannel(uint8_t ch)
{
	if (ch >= MAX_ADC_CHANNELS)
		return EC_INVALID_CH;
		
	switch(ch)
	{
		case 0:
			PORTE.PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc;
			break;
		case 1:
			PORTE.PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc;
			break;
		case 2:
			PORTE.PIN2CTRL |= PORT_ISC_INPUT_DISABLE_gc;
			break;
		case 3:
			PORTE.PIN3CTRL |= PORT_ISC_INPUT_DISABLE_gc;
			break;
	}
		
	return NOERROR;
}

uint16_t CADC::ReadADC(uint8_t ch)
{
	switch(ch)
	{
		case 0:
			ADC0.MUXPOS  = ADC_MUXPOS_AIN8_gc;
			break;
		case 1:
			ADC0.MUXPOS  = ADC_MUXPOS_AIN9_gc;
			break;
		case 2:
			ADC0.MUXPOS  = ADC_MUXPOS_AIN10_gc;
			break;
		case 3:
			ADC0.MUXPOS  = ADC_MUXPOS_AIN11_gc;
			break;
	}
	
	/* Start ADC conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
	
	/* Wait until ADC conversion done */
	while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)){}
		
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	
	// Because I'm averaging 4 samples per request
	//	I need to divide result by 4.
	return ADC0.RES >> 2;
}