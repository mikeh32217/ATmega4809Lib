/*
 * CADC.h
 *
 * Created: 1/13/2022 3:52:08 PM
 *  Author: Mike
 */ 


#ifndef CADC_H_
#define CADC_H_

#define MAX_ADC_CHANNELS	4

class CADC
{
	private:
	
	public:
		CADC();
		
		uint8_t	ConfigureChannel(uint8_t ch);
		uint16_t ReadADC(uint8_t ch);		
};

#endif /* CADC_H_ */