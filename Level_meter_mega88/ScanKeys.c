/*
 * ScanKeys.c
 *
 * Created: 14/2/2016 8:26:44 μμ
 *  Author: George
 */ 
#include "ScanKeys.h"
#include <avr/io.h>
#define F_CPU 4000000ul
#include <util/delay.h>



void init_keys()
{
	ddrkeys&=~(1<<enter|1<<up|1<<down); //INPUT
	portkeys|=(1<<enter|1<<up|1<<down); //pullup
}

uint8_t _key(uint8_t value, uint8_t pace)
{
	uint8_t temp = value;
	if (!(Pinkeys &_BV(up)))
	{
		_delay_ms(100);
		if (!(Pinkeys & _BV(up)))
		{
			temp+=pace;
		}
	}
	else if (!(Pinkeys & _BV(down)))
	{
		_delay_ms(100);
		if (!(Pinkeys & _BV(down)))
		{
			temp-=pace; //test for git
		}
	}
	return temp;
}
