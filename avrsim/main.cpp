/*
 * avrsim.cpp
 *
 * Created: 12/30/2021 5:00:42 AM
 * Author : Mike
 */ 

#include <avr/io.h>
#include "DeviceManager.h"

static DeviceManager dmgr;

int main(void)
{
	dmgr.SetDacVoltage(2.75);
	
		
	//PORTA.DIR |= PIN7_bm;
	//PORTA.OUT |= PIN7_bm;

    /* Replace with your application code */
    while (1) 
    {
    }
}

