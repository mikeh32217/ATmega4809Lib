
 /*!
	 \file CI2C.h
	 \author Mike Hankey
	 \date 5/11/2022
 
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

#ifndef CI2C_H_
#define CI2C_H_

#define TWI0_BAUD(F_CLK, F_SCL, T_RISE)	(uint8_t)((((((float)F_CLK / (float)(F_SCL)) - 10 - (((float)(F_CLK) * (T_RISE))/1000000.0))) / 2))
#define I2C_SCL_FREQ                  400000

#define I2C_DIRECTION_BIT_WRITE       0
#define I2C_DIRECTION_BIT_READ        1

class CI2C
{
	public:
		CI2C(uint32_t clk);
		
		uint8_t Start(uint8_t addr, uint8_t directionBit);
		uint8_t ReceiveData(uint8_t* data, uint8_t len);
		uint8_t SendData(uint8_t* data, uint8_t len);
		void Flush();
		void Stop();
};


#endif /* CI2C_H_ */