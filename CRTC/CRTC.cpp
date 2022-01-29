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
	PORTF.OUTTGL = PIN0_bm;
    /* Clear flag by writing '1': */
    RTC.INTFLAGS = RTC_CMP_bm;	
}

CRTC::CRTC()
{
	PORTF.DIRSET = PIN0_bm;
	
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
	
	RTC.INTCTRL = RTC_CMP_bm;
	RTC.CMP = 0x1000;
	RTC.CTRLA = RTC_PRESCALER_DIV1_gc;
}

void CRTC::Start()
{
	RTC.CTRLA |= RTC_RTCEN_bm;
}

void CRTC::Stop()
{
	RTC.CTRLA &= ~RTC_RTCEN_bm;	
}