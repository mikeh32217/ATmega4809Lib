/*
 * avrsim.cpp
 *
 * Created: 12/30/2021 5:00:42 AM
 * Author : Mike
 *
 * TODO 
 *	Add a dispose method or use destructor?
 */ 

#define F_CPU 20000000UL

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "DeviceManager.h"
#include "CUart.h"
#include "CPulse.h"
#include "CDAC.h"
#include "CADC.h"
#include "CMCP23S17.h"
#include "CTimer.h"
#include "CI2C.h"
#include "CRTC.h"

#define DEF_BUFFER_SZ	16

void TestOneShot();
void TestDAC();
void TestMSI();
void TestADC();
void TestTimer();
void TestPIO();
void TestI2C();
void TestRTC();
void TestUART();

DeviceManager dmgr;
//CUart uart(USART_BAUD_RATE(F_CPU, 9600), false);

CPulse* pulse = nullptr;
CDAC* dac = nullptr;
CUart* mspi = nullptr;
CADC* adc = nullptr;
CTimer* timer;
CMCP23S17* pio = nullptr;
CI2C* i2c = nullptr;
CRTC* rtc = nullptr;
CUart* uart = nullptr;

volatile uint8_t g_res = 0;
volatile char buf1[4];

ISR(PORTD_PORT_vect)
{
	PORTD.INTFLAGS = PIN1_bm;
	g_res = pio->ReadPort(EXP_PORTA);
}

char wbuf[] = "AVR Simulator 1.0\r\n";

int main(void)
{
	
	CCP = CCP_IOREG_gc;     // Key from table 10-1 section 10.3.5
	CLKCTRL.MCLKCTRLB = 0;  // Main Clock Control B register section 10.5.2
		
//	uart.SendData(buf, strlen(buf));

	// set ref for adc to avdd
	//VREF.CTRLA |= VREF_AC0REFSEL_AVDD_gc;
	
	TestOneShot();
//	TestDAC();
//	TestADC();
//	TestMSI();
//	TestTimer();
//	TestPIO();
//	TestI2C();
//	TestRTC();
//	TestUART();
	
    while (1) 
    {
    }
}

void TestUART()
{
	// Set TX as output
	PORTA.OUTSET = PIN0_bm;
	
	uart = dmgr.GetUart(F_CPU, 9600);
	
	uart->SendData(wbuf, strlen(wbuf));
	
	while (1)
	{
		uart->SendChar(0x30);
		_delay_ms(500);
	}
}

#define CLIENT_ADDR	0xA6

void TestI2C()
{
	volatile uint8_t outbuf[14] = { 0x00, 0x01, 'B', 'u', 'g', 'g', 'g', 'g', 'g', 'g', 'g', 'e', 'r', 's' };
	volatile uint8_t inbuf[12];
	
	i2c = dmgr.GetI2C(F_CPU);	
		
    i2c->Start(CLIENT_ADDR, I2C_DIRECTION_BIT_WRITE);
	i2c->SendData((uint8_t*)outbuf, 14);
	i2c->Stop();
	
	_delay_ms(5);

	i2c->Start(CLIENT_ADDR, I2C_DIRECTION_BIT_WRITE);
	i2c->SendData((uint8_t*)outbuf, 2);
	i2c->Stop();
		
	_delay_ms(5);

	i2c->Start(CLIENT_ADDR, I2C_DIRECTION_BIT_READ);
	i2c->ReceiveData((uint8_t*)inbuf, 12);
	i2c->Stop();
}

void TestRTC()
{
	rtc = dmgr.GetRTC();
	
	sei();
	
	rtc->Start();
}

void TestPIO()
{
	pio = dmgr.GetPIO(F_CPU);
	
//	pio->ConfigurePin(EXP_PORTA, ePIN0, EXT_OUTPUT);
	// Sets A0 as input to test pin change interrupt on PIO
	pio->ConfigurePin(EXP_PORTA, ePIN1, EXT_INPUT);
//	pio->ConfigurePort(EXP_PORTA, 0x02);
	pio->SetPullup(EXP_PORTA, ePIN1, true);
	pio->ConfigureInterrupt(EXP_PORTA, ePIN1, eFalling, eHIGH);
	pio->SetInterruptState(EXP_PORTA, ePIN1, true);
	
	PORTD.DIRCLR = PIN1_bm;
//	PORTD.DIRCLR = PIN2_bm;
	PORTD.PIN1CTRL = PORT_ISC_RISING_gc;
//	PORTD.PIN2CTRL = PORT_ISC_BOTHEDGES_gc;
			
	sei();

	g_res = pio->ReadPort(EXP_PORTA);

	while(1)
	{
//		pio->WritePin(EXP_PORTA, ePIN0, eHIGH);
//		pio->WritePort(EXP_PORTA, 0x01);
//		_delay_ms(100);
//		pio->WritePin(EXP_PORTA, ePIN0, eLOW);
//		pio->WritePort(EXP_PORTA, 0x00);
//		_delay_ms(100);
	}
}

void TestTimer()
{
	volatile uint32_t t = 0;
	
	timer = dmgr.GetTimer(F_CPU);
	
	sei();
	
	timer->StartTimer();
		
	while (1)
	{
		t = timer->GetTicks();
		_delay_ms(3000);
		timer->StopTimer();
	}
}

void TestMSI()
{
	mspi = dmgr.GetMSpi(F_CPU);
	while (1)
	{
		PORTC.OUTCLR = PIN3_bm;
		mspi->SendChar(0xa6);
		PORTC.OUTSET = PIN3_bm;
	}
}

void TestADC()
{
	volatile uint16_t res = 0;
	
	dac = dmgr.GetDAC(F_CPU, 5.0f);
	adc = dmgr.GetADC();
	adc->ConfigureChannel(0);
	dac->SetVoltage(3.75f);

	while(1)
	{
		res = adc->ReadADC(0);
//		_delay_ms(100);
	}	
}

void TestDAC()
{
	dac = dmgr.GetDAC(F_CPU, 5.0f);
	
	dac->SetVoltage(3.25);
	_delay_ms(3000);
	dac->Shutdown();
}

void TestOneShot()
{
	uint16_t cnt = 0;
	
	pulse = dmgr.GetPulse();
	
	// width is calculated as; F_CPU / WIDTH
	// Example; 1ms = 20000000 * .001 = 0x4e20
	// Example; 12.8uS = 20000000 * .00000128 = 0x100
	pulse->ConfigureOneShot(F_CPU, .0000128f);
	
	while(cnt++ < 0xffff)
	{
		pulse->SendPulse();
		_delay_us(100);
	}
	
	pulse->DisableOneShot();	
}

