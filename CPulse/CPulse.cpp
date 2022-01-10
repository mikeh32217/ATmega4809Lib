/*
 * CPulse.cpp
 *
 * Created: 1/3/2022 12:25:01 PM
 * Author : Mike
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CPulse.h"

CPulse::CPulse()
{
	// Set output pin for Timer
	PORTA.DIRSET = PIN2_bm;
}

void CPulse::ConfigureOneShot(uint16_t width, CS_STATE state/* = LOW*/)
{
	// Route Event Channel 0 to TCB0
	EVSYS.USERTCB0 = EVSYS_CHANNEL_CHANNEL0_gc;

	TCB0.CCMP = width;
	
	// Enables the input capture event
	TCB0.EVCTRL = TCB_CAPTEI_bm;
	
	// Declare Single Shot mode and enable output
	TCB0.CTRLB = TCB_CNTMODE_SINGLE_gc | TCB_CCMPEN_bm;

	// Use CLK_PER and  enable timer
	TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm;
	
	// Default initial state is LOW
	if (state == HIGH)
		TCB0.CTRLB = TCB_CCMPINIT_bm;
}

void CPulse::DisableOneShot()
{
	TCB0.CTRLB &= ~TCB_CCMPEN_bm;

	// Use CLK_PER and  enable timer
	TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void CPulse::SendPulse()
{
	EVSYS.STROBE = 0x01;
}

void CPulse::ConfigureRepeatingPulse(uint16_t period,
									uint16_t pwidth,
									CS_STATE state/* = LOW*/)
{
	ConfigureTimerA(period);
	
	// Route Event Channel 0 to TCB0
	EVSYS.CHANNEL0 = EVSYS_GENERATOR_TCA0_CMP0_gc;

	ConfigureOneShot(pwidth, state);
}

void CPulse::DisableRepeatingPulse()
{
	DisableOneShot();
	
	TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

void CPulse::ConfigureTimerA(uint16_t period)
{
	// NOTE: Add pulse width to this value
	TCA0.SINGLE.CMP0BUF = period;
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_FRQ_gc;
	TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
}
