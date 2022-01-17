/*
 * CTimer.cpp
 *
 * Created: 1/3/2022 12:47:20 PM
 * Author : Mike
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CTimer.h"

volatile uint32_t g_count = 0;

ISR(TCA0_OVF_vect)
{
	g_count += 1;
	
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

CTimer::CTimer()
{
	m_callback = nullptr;
	m_target = 0xffffffff;
	
	// Prescale F_CPU / 8
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc;
	
	// FRQ mode
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
	
	// CMP0 interrupt enable
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	
	// 1mS tick
	TCA0.SINGLE.PER = 1249;
	
	sei();
}

uint32_t CTimer::GetTicks()
{
	return g_count;
}

void CTimer::SetCallback(TimerCallback callback, uint32_t target)
{
	m_callback = callback;
	m_target = target;
}

void CTimer::StartTimer()
{
	g_count = 0;
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void CTimer::StopTimer()
{
	TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

void CTimer::ResetTimer()
{
	g_count = 0;
}