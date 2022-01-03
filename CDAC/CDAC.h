/*
 * CDAC.h
 *
 * Created: 12/30/2021 5:05:01 AM
 *  Author: Mike
 */ 


#ifndef CDAC_H_
#define CDAC_H_

class CDAC
{
	public:
		CDAC(){}
		
		uint16_t ConvertVoltage(float volts);	
};

#endif /* CDAC_H_ */