
/*!
	\file CMCP23S17.h
	\author Mike Hankey
	\date 9/30/2018

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

#ifndef CMCP23S17_H_
#define CMCP23S17_H_

#include "CSpi.h"

#define ODR		0x02
#define INTPOL	0x01

typedef enum 
{
	EXP_PORTA,
	EXP_PORTB
} _PORTS;

typedef enum
{
	EXT_INPUT,
	EXT_OUTPUT
} _DIR;

typedef enum
{
	eLOW,
	eHIGH
} _STATE;

typedef enum 
{
    IODIRA,     
	IODIRB,
    IPOLA,      
	IPOLB,
    GPINTENA,   
	GPINTENB,
    DEFVALA,    
	DEFVALB,
    INTCONA,    
	INTCONB,
    IOCONA,     
	IOCONB,
    GPPUA,      
	GPPUB,
    INTFA,      
	INTFB,
    INTCAPA,    
	INTCAPB,
    GPIOA,      
	GPIOB,
    OLATA, 
	OLATB
} _REGS;

typedef enum
{
	ePIN0,
	ePIN1,
	ePIN2,
	ePIN3,
	ePIN4,
	ePIN5,
	ePIN6,
	ePIN7
} _PINS;

typedef enum
{
	eRising,
	eFalling,
	eChange
} _TRIGGER;

typedef union 
{
	uint32_t u_results;
	uint8_t	s_portAPins;
	uint8_t	s_portBPins;
	uint8_t	s_portAValue;
	uint8_t	s_portBValue;
} InterruptInfo;

class CMCP23S17 {
	
	private:
		CSpi*		mp_spi;
		uint8_t		m_chan;
		uint8_t		m_raddr;
		uint8_t		m_waddr;
		uint8_t		m_regs[24];

	public:
		CMCP23S17(uint8_t addr, uint8_t chan, CSpi* p_spi);

		void ConfigurePin(_PORTS port, _PINS pin, _DIR dir);
		void ConfigurePort(_PORTS port, uint8_t dir_mask);

		void ConfigureInterrupt(_PORTS port, _PINS pin, _TRIGGER trig, _STATE level);
		void SetInterruptState(_PORTS port, _PINS pin, bool state);
		InterruptInfo GetInterruptResults();

		void WritePin(_PORTS port, _PINS pin, _STATE val);
		void WritePort(_PORTS port, uint8_t val);

		void SetPullup(_PORTS port, _PINS pin, bool state);
		void SetIntOD(_PORTS port, bool isOd);

		uint8_t ReadPort(_PORTS port);

		uint8_t ReadReg(uint8_t reg_addr);
	private:
		void WriteReg(uint8_t reg_addr, uint8_t op_code);

		void ReadAllRegs();
};


#endif /* CMCP23S17_H_ */