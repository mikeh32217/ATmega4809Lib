 /*!
	 \file CRTC.cpp
	 \author Mike Hankey
	 \date 1/27/2022
 
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
	 
	 \note THIS MODULE IS A WORK IN PROGRESS.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CRTC.h"

volatile uint16_t g_ticks = 0;

ISR(RTC_CNT_vect)
{
	g_ticks = RTC.CNT;
	
	// Pulse the output port to trigger an external interrupt
	PORTF.OUTSET = PIN2_bm;
	PORTF.OUTCLR = PIN2_bm;
	
    /* Clear flag by writing '1': */
    RTC.INTFLAGS = RTC_OVF_bm;	
}

CRTC::CRTC()
{
	PORTF.DIRSET = PIN2_bm;
	
	// Disable to set external oscillator
	uint8_t temp;
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_ENABLE_bm;
	
	// Use the external 32K xtal
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;
	
	// Wait for the clock to settle
	while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm);
	
	// Set to use external oscillator
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_SEL_bm;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;
	
	// Enable
	temp = CLKCTRL.XOSC32KCTRLA;
	temp |= CLKCTRL_ENABLE_bm;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;
	while (RTC.STATUS > 0);
	
	
	RTC.CLKSEL = RTC_CLKSEL_TOSC32K_gc;
	
	RTC.INTCTRL = RTC_OVF_bm;
	RTC.PER = 0x0020;
	RTC.CTRLA = RTC_PRESCALER_DIV1_gc | RTC_RTCEN_bm | RTC_RUNSTDBY_bm;
}

void CRTC::Start()
{
	RTC.CTRLA |= RTC_RTCEN_bm;
}

void CRTC::Stop()
{
	RTC.CTRLA &= ~RTC_RTCEN_bm;	
}