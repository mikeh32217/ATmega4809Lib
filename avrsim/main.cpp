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
#include "CPulse.h"
#include "CDAC.h"

#define DEF_BUFFER_SZ	16

void TestOneShot();
void TestRepeatingPulse();
void TestDAC();

DeviceManager dmgr;
CUart uart(USART_BAUD_RATE(F_CPU, 9600), false);

CPulse* pulse = nullptr;
CDAC* dac = nullptr;
CUart* mspi = nullptr;

int main(void)
{
	char buf[] = "AVR Simulator 1.0\r\n";
	
	CCP = CCP_IOREG_gc;     // Key from table 10-1 section 10.3.5
	CLKCTRL.MCLKCTRLB = 0;  // Main Clock Control B register section 10.5.2
		
	uart.SendData(buf, strlen(buf));

	// set ref for adc to avdd
	//VREF.CTRLA |= VREF_AC0REFSEL_AVDD_gc;
	
//	TestOneShot();
//	TestDAC();
//	TestRepeatingPulse();
	
	//PORTC.DIRSET = PIN0_bm;	// MOSI
	//PORTC.DIRCLR = PIN1_bm;	// MISO
	//PORTC.DIRSET = PIN2_bm;	// Clock
	//PORTC.DIRSET = PIN3_bm;	// Enable
	//PORTC.OUTSET = PIN3_bm;
    //USART1.BAUD = USART_BAUD_RATE(F_CPU, 38400);
//
	//USART1.CTRLC = USART_CMODE_MSPI_gc;
	//USART1.CTRLB = USART_TXEN_bm;
	
	mspi = dmgr.GetMSpi();
    while (1) 
    {
		PORTC.OUTCLR = PIN3_bm;
		mspi->SendChar(0xa6);
		PORTC.OUTSET = PIN3_bm;
    }
}

void TestDAC()
{
	dac = dmgr.GetDAC();
	
	dac->SetVoltage(3.25);
}

void TestOneShot()
{
	uint16_t cnt = 0;
	
	pulse = dmgr.GetPulse();
	
	// width is calculated as; F_CPU / WIDTH
	// Example; 1ms = 20000000 * .001 = 0x4e20
	// Example; 12.8uS = 20000000 * .00000128 = 0x100
	pulse->ConfigureOneShot(.0000128f);
	
	while(cnt++ < 0xffff)
	{
		pulse->SendPulse();
		_delay_us(100);
	}
	
	pulse->DisableOneShot();	
}

void TestRepeatingPulse()
{
	pulse->ConfigureRepeatingPulse(.001f, .0000128f);
	_delay_ms(3000);
	pulse->DisableRepeatingPulse();	
}

