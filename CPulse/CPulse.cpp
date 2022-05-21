 /*!
	 \file CPulse.cpp
	 \author Mike Hankey
	 \date 1/3/2022
 
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
	 
	 \note Output a pulse of user defined width to PORTA.2.  Is an example of using 
		TIMERB0 and the Event System.
		When TIMERB0's Compare value is reached an event is fired but
		it's not automatically configured to do anything. So when the
		Stobe is fired it causes the firing of the event to generate a 
		system event which causes the One Shot pulse to be output to
		PORTA.Pin2.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CPulse.h"

CPulse::CPulse()
{
	// Set output pin for Timer
	PORTA.DIRSET = PIN2_bm;
}

void CPulse::ConfigureOneShot(uint32_t clk, float width, CS_STATE state /* = LOW */)
{
	// Calculates width of the pulse
	uint16_t w = (uint16_t)((float)clk * width);
	
	Configure(w, state);
}
									
void CPulse::SendPulse()
{
	// Software events on Channel n are issued by writing a ‘1’ to the
	//	STROBE[n] bit
	EVSYS.STROBE = 0x01;
}

void CPulse::DisableOneShot()
{
	// Disable  the Compare/Capture Output Enable
	TCB0.CTRLB &= ~TCB_CCMPEN_bm;

	// Disable timer
	TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void CPulse::Configure(uint16_t width, CS_STATE state/* = LOW*/)
{
	// Route Event Channel 0 to TCB0
	EVSYS.USERTCB0 = EVSYS_CHANNEL_CHANNEL0_gc;

	// Timeout Check mode check value
	TCB0.CCMP = width;
	
	// Enables the input capture event for Single-Shot Count mode
	TCB0.EVCTRL = TCB_CAPTEI_bm;
	
	// Declare Single Shot mode and enable output
	TCB0.CTRLB = TCB_CNTMODE_SINGLE_gc | TCB_CCMPEN_bm;

	// Use CLK_PER and  enable timer
	TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm;
	
	// Default initial state is LOW, set to HIGH per user
	if (state == HIGH)
		TCB0.CTRLB = TCB_CCMPINIT_bm;
}







