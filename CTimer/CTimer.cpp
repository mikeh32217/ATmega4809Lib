/*!
	 \file CTimer.cpp
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
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "CTimer.h"

volatile uint32_t g_millis = 0;

ISR(TCA0_OVF_vect)
{
	g_millis += 1;
	
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

CTimer::CTimer(uint32_t clk)
{
	m_callback = nullptr;
	m_target = 0xffffffff;
	
	// Prescale F_CPU / 16
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc;
	
	// NORMAL mode
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
	
	// CMP0 interrupt enable
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	
	uint32_t cnt = (clk / (2 * 16 * 1000)) - 1;
	// 1mS tick
	TCA0.SINGLE.PER = cnt;
}

uint32_t CTimer::GetTicks()
{
	return g_millis;
}

void CTimer::SetCallback(TimerCallback callback, uint32_t target)
{
	m_callback = callback;
	m_target = target;
}

void CTimer::StartTimer()
{
	g_millis = 0;
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void CTimer::StopTimer()
{
	TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

void CTimer::ResetTimer()
{
	g_millis = 0;
}