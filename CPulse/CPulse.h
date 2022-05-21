 /*!
	 \file CPulse.h
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

#ifndef CPULSE_H_
#define CPULSE_H_

#include "Common.h"

class CPulse
{
	private:
		CS_STATE m_state;
		
	public:	
		CPulse();
		
		// TODO Add unit to all floats, example 1.82 MS
		void ConfigureOneShot(uint32_t clk, float width, CS_STATE state = LOW);
		void DisableOneShot();
		
		void SendPulse();
		
	private:
		void Configure(uint16_t width, CS_STATE state = LOW);
};


#endif /* CPULSE_H_ */