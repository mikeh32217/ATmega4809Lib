/*
 * CASC.cpp
 *
 * Created: 1/13/2022 3:51:39 PM
 * Author : Mike
 */ 

#include <avr/io.h>
#include "CADC.h"

CADC::CADC()
{
	/* Disable digital input buffer on AIN8 (PE0) */
	PORTE.PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc;
		
	// Because I'm running at 20Mhz I need to prescale by 16
	// Also because voltage ref so high need the sampling cap
	// Using VDD as voltage reference
	ADC0.CTRLC = ADC_PRESC_DIV16_gc
	| ADC_SAMPCAP_bm
	| ADC_REFSEL_VDDREF_gc;
		
	// Sampling average 4 times
	ADC0.CTRLB = ADC_SAMPNUM_ACC4_gc;
	
	/* Select ADC channel */
	ADC0.MUXPOS  = ADC_MUXPOS_AIN8_gc;
	
	/* ADC Enable: enabled */	
	ADC0.CTRLA |= ADC_ENABLE_bm; 
}

uint16_t CADC::ReadADC()
{
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