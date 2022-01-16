
 /*!
	 \file CMCP23S17.cpp
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

	 \note Be sure to put a .1uF capacitor across VDD to GND!
 */

#include <avr/io.h>
#include "CMCP23S17.h"

/**
 * \brief Constructor
 * 
 * \param addr - Hardware address of chip (0 - 7)
 * \param ch
 * \param spi
 * 
 * \return 
 */
CMCP23S17::CMCP23S17(uint8_t addr, uint8_t ch, CSpi* spi)
{
	m_chan = ch;
	mp_spi = spi;
	m_waddr = 0x40 | ((addr & 0x07) << 1);
	m_raddr = 0x41 | ((addr & 0x07) << 1);

	WriteReg(IOCONA, 0x18);

	ReadAllRegs();
}

/**
 * \brief Configure a pin as Input/Output and direction.
 * 
 * \param port - Port of pin to configure
 * \param pin - Pin to configure
 * \param dir - Direction, either Input or Output
 * 
 * \return void
 */
void CMCP23S17::ConfigurePin(_PORTS port, _PINS pin, _DIR dir)
{
	uint16_t mask = (1 << pin);

	if (port == EXP_PORTA)
	{
		m_regs[ACTIVEA] |= mask;

		if (dir == EXT_OUTPUT)
			m_regs[IODIRA] &= ~mask;
		else
			m_regs[IODIRA] |= mask;

		WriteReg(IODIRA, m_regs[IODIRA]);
	}
	else
	{
		m_regs[ACTIVEB] |= mask;

		if (dir == EXT_OUTPUT)
			m_regs[IODIRB] &= ~mask;
		else
			m_regs[IODIRB] |= mask;

		WriteReg(IODIRB, m_regs[IODIRB]);
	}
}

/**
 * \brief - Configure a byte using masks
 * 
 * \param port - Port to configure
 * \param active_mask = A 8 bit mask indicating which bits are active
 * \param dir_mask - Direction of bit
 * 
 * \return void
 */
void CMCP23S17::ConfigureByte(_PORTS port, uint8_t active_mask, uint8_t dir_mask)
{
	uint8_t com = 0x01;

	for (uint8_t i = 0; i < 8; i++)
	{
		if (active_mask & com)
		{
			if (dir_mask & com)
				ConfigurePin(port, (_PINS)i, EXT_INPUT);
			else
				ConfigurePin(port, (_PINS)i, EXT_OUTPUT);
		}
		com >>= 1;
	}
}

/**
 * \brief Set/Clear a single pin
 * 
 * \param port - Port to write to
 * \param pin - Pin we are going to modify
 * \param val - new value
 * 
 * \return void
 */
void CMCP23S17::WritePin(_PORTS port, _PINS pin, _STATE val)
{
	if (port == EXP_PORTA)
	{
		if (val == eHIGH)
			m_regs[GPIOA] |= (1 << pin);
		else
			m_regs[GPIOA] &= ~(1 << pin);

		WriteByte(port, m_regs[GPIOA], false);
	}
	else
	{
		if (val == eHIGH)
			m_regs[GPIOB] |= (1 << pin);
		else
			m_regs[GPIOB] &= ~(1 << pin);

		WriteByte(port, m_regs[GPIOB], false);
	}
}

/**
 * \brief Write 8 bits (byte) to port
 * 
 * \param port - PORTA or POSTB
 * \param val - Value to write
 * \param raw - raw = HIGH value is written out as it 
 *				raw = LOW the value is AND'd withe the active mask and only active bits designated
 *					as output are output.
 * 
 * \return void
 */
void CMCP23S17::WriteByte(_PORTS port, uint8_t val, bool raw)
{
	if (port == EXP_PORTA)
	{
		if (!raw)
			val &= (m_regs[ACTIVEA] & ~m_regs[IODIRA]);

		m_regs[GPIOA] = val;

		WriteReg(GPIOA, val);
	}
	else
	{
		if (!raw)
			val &= (m_regs[ACTIVEB] & ~m_regs[IODIRB]);

		m_regs[GPIOB] = val;

		WriteReg(GPIOB, val);
	}
}

/**
 * \brief Write to both ports
 * 
 * \param dval - Value to write
 * \param raw - Write in raw more?
 * 
 * \return void
 */
void CMCP23S17::WriteWord(uint16_t dval, bool raw)
{
	WriteByte(EXP_PORTA, dval & 0x00ff, raw);
	WriteByte(EXP_PORTB, (dval & 0xff00) >> 8, raw);
}

/**
 * \brief Read a byte from a port
 * 
 * \param port - Port to read from
 * 
 * \return uint8_t - Value read
 */
uint8_t CMCP23S17::ReadByte(_PORTS port)
{
	if (port == EXP_PORTA)
	{
		m_regs[GPIOA] = ReadReg(GPIOA);
		return m_regs[GPIOA];
	}
	else
	{
		m_regs[GPIOB] = ReadReg(GPIOB);
		return m_regs[GPIOB];
	}
}

/**
 * \brief - Set/Reset pullup on port pin.
 * 
 * \param port - Port to work on
 * \param pin - Pin to set/reset
 * \param state - value, set or reset
 * 
 * \return void
 */
void CMCP23S17::SetPullup(_PORTS port, _PINS pin, bool state)
{
	uint8_t res = 0;

	if (state)
		res |= (1 << pin);
	else
		res &= ~(1 << pin);

	if (port == EXP_PORTA)
		WriteReg(GPPUA, res);
	else
		WriteReg(GPPUB, res);
}

// ===============> Interrupt methods <==================== *//

/**
 * \brief - Configure interrupt
 * 
 * \param port - What port
 * \param pin - What pin
 * \param trig - Interrupt Trigger (eChange, eRising or eFalling)
 * \param level - Level interrupt pin will be when triggered
 * 
 * \return void
 */
void CMCP23S17::ConfigureInterrupt(_PORTS port, _PINS pin, _TRIGGER trig, _STATE level)
{
	uint8_t intcon = INTCONA;
	uint8_t defval = DEFVALA;

	if (port == EXP_PORTB)
	{
		intcon = INTCONB;
		defval = DEFVALB;
		if (level)
			m_regs[IOCONB] |= (1 << INTPOL);
		else
			m_regs[IOCONB] &= (1 << INTPOL);
	}
	else
	{
		if (level)
			m_regs[IOCONA] |= (1 << INTPOL);
		else
			m_regs[IOCONA] &= (1 << INTPOL);
	}

	switch (trig)
	{
		case eChange:
			m_regs[intcon] &= ~(1 << pin);
			break;
		case eRising:
			m_regs[intcon] |= (1 << pin);
			m_regs[defval] &= ~(1 << pin);
			break;
		case eFalling:
			m_regs[intcon] |= (1 << pin);
			m_regs[defval] |= ~(1 << pin);
			break;
	}

	if (port == EXP_PORTA)
	{
		WriteReg(IOCONA, m_regs[IOCONA]);
		WriteReg(INTCONA, m_regs[INTCONA]);
		WriteReg(DEFVALA, m_regs[DEFVALA]);
	}
	else
	{
		WriteReg(IOCONB, m_regs[IOCONB]);
		WriteReg(INTCONB, m_regs[INTCONB]);
		WriteReg(DEFVALB, m_regs[DEFVALB]);
	}
}

/**
 * \brief - Set/Reset interrupt state
 * 
 * \param port - What port
 * \param pin - What pin
 * \param state - What state
 * 
 * \return void
 */
void CMCP23S17::SetInterruptState(_PORTS port, _PINS pin, bool state)
{
	if (port == EXP_PORTA)
	{
		if (state)
			m_regs[GPINTENA] |= (1 << pin);
		else
			m_regs[GPINTENA] &= ~(1 << pin);

		WriteReg(GPINTENA, m_regs[GPINTENA]);
	}
	else
	{
		if (state)
			m_regs[GPINTENB] |= (1 << pin);
		else
			m_regs[GPINTENB] &= ~(1 << pin);

		WriteReg(GPINTENB, m_regs[GPINTENB]);
	}
}

/**
 * \brief - Get interrupt pins and values
 * 
 * \return InterruptInfo - A union with all the pins and values.
 */
InterruptInfo CMCP23S17::GetInterruptResults()
{
	InterruptInfo ui;

	ui.s_portAPins = ReadReg(INTFA);
	ui.s_portBPins = ReadReg(INTFB);
	ui.s_portAValue = ReadReg(INTCAPA);
	ui.s_portBValue = ReadReg(INTCAPB);

	return ui;
}

/**
 * \brief - Set/Reset INT pin as Open Drain
 * 
 * \param port - Which port
 * \param isOd - What value
 * 
 * \return void
 */
void CMCP23S17::SetIntOD(_PORTS port, bool isOd)
{
	if (port == EXP_PORTA)
	{
		if (isOd)
			m_regs[IOCONA] |= (1 << ODR);
		else
			m_regs[IOCONA] &= ~(1 << ODR);

		WriteReg(IOCONA, m_regs[IOCONA]);
	}
	else
	{
		if (isOd)
			m_regs[IOCONB] |= (1 << ODR);
		else
			m_regs[IOCONB] &= ~(1 << ODR);

		WriteReg(IOCONB, m_regs[IOCONB]);
	}
}


// ===============> Private methods <====================== *//

/**
 * \brief Write to the device (Low Level)
 * 
 * \param reg_addr - Register to write to
 * \param op_code - Op code to write to register
 * 
 * \return void
 */
void CMCP23S17::WriteReg(uint8_t reg_addr, uint8_t op_code)
{
	mp_spi->SetPinState(m_chan, LOW);

	mp_spi->TransferData(m_waddr);
	mp_spi->TransferData(reg_addr);
	mp_spi->TransferData(op_code);

	mp_spi->SetPinState(m_chan, HIGH);
}

/**
 * \brief - Read from the device (Low Level)
 * 
 * \param reg_addr - Register to read from
 * 
 * \return uint8_t - Results of read
 */
uint8_t CMCP23S17::ReadReg(uint8_t reg_addr)
{
	XFER_RESULT res;

	mp_spi->SetPinState(m_chan, LOW);

	mp_spi->TransferData(m_raddr);
	mp_spi->TransferData(reg_addr);
	res = mp_spi->TransferData(0xff);

	mp_spi->SetPinState(m_chan, HIGH);

	return res.data;	
}

/**
 * \brief - Read all registers in a sequential manner
 * 
 * \return void
 *
 * \note Stores results of read in the local register array.
 */
void CMCP23S17::ReadAllRegs()
{
	XFER_RESULT res;
	
	mp_spi->SetPinState(m_chan, LOW);

	mp_spi->TransferData(m_raddr);
	mp_spi->TransferData(0xff);

	for (uint8_t i = 0; i < 22; i++)
	{
		res = mp_spi->TransferData(0xff);
		m_regs[i] = res.data;
	}

	mp_spi->SetPinState(m_chan, HIGH);
}