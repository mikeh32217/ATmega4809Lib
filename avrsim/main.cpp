/*
 * avrsim.cpp
 *
 * Created: 12/30/2021 5:00:42 AM
 * Author : Mike
 */ 
#define F_CPU 20000000UL

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "DeviceManager.h"
#include "CUart.h"

#define USART0_BAUD_RATE(xtal, baud) ((float)(xtal * 64 / (16 * (float)baud)) + 0.5)
#define DEF_BUFFER_SZ	16

void TestOneShot();
void TestRepeatingPulse();
void TestDAC();

DeviceManager dmgr;
CUart uart(USART0_BAUD_RATE(F_CPU, 9600));

int main(void)
{
	char buf[] = "AVR Simulator 1.0\r\n";
	
	CCP = CCP_IOREG_gc;     // Key from table 10-1 section 10.3.5
	CLKCTRL.MCLKCTRLB = 0;  // Main Clock Control B register section 10.5.2
		
	uart.SendData(buf, strlen(buf));

	// set ref for adc to avdd
	//VREF.CTRLA |= VREF_AC0REFSEL_AVDD_gc;
	
	TestOneShot();
//	TestDAC();
	TestRepeatingPulse();
	
    while (1) 
    {
    }
}

void TestDAC()
{
	dmgr.SetDacVoltage(3.25);
}

void TestOneShot()
{
	uint16_t cnt = 0;
	
	// width is calculated as; F_CPU / WIDTH
	// Example; 1ms = 20000000 * .001 = 0x4e20
	// Example; 12.8uS = 20000000 * .00000128 = 0x100
	dmgr.ConfigureOneShot(0x0100);
	
	while(cnt++ < 0xffff)
	{
		dmgr.SendPulse();
		_delay_us(100);
	}
	
	dmgr.DisableOneShot();	
}

void TestRepeatingPulse()
{
	dmgr.ConfigureRepeatPulse(0x4F20, 0x100);
	_delay_ms(3000);
	dmgr.DisableRepeatingPulse();	
}

