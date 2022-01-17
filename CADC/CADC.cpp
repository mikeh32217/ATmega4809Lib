/*
 * CASC.cpp
 *
 * Created: 1/13/2022 3:51:39 PM
 * Author : Mike
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
		
	return EC_OK;
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