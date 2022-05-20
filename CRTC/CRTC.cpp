/*
 * CRTC.cpp
 *
 * Created: 1/27/2022 2:07:15 PM
 * Author : Mike
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CRTC.h"

volatile uint16_t g_cnt = 0;

ISR(RTC_CNT_vect)
{
	g_cnt = RTC.CNT;
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
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;
	while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm)
	{
		;
	}
	
	// Se to use external oscillator
	temp = CLKCTRL.XOSC32KCTRLA;
	temp &= ~CLKCTRL_SEL_bm;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;
	
	// Enable
	temp = CLKCTRL.XOSC32KCTRLA;
	temp |= CLKCTRL_ENABLE_bm;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = temp;
	while (RTC.STATUS > 0)
	{
		;
	}
	
	
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