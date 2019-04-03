/*
 * ScanKeys.h
 *
 * Created: 14/2/2016 8:27:08 μμ
 *  Author: George
 */ 


#ifndef SCANKEYS_H_
#define SCANKEYS_H_
#include <inttypes.h>
#define Pinkeys PINC
#define ddrkeys DDRC
#define portkeys PORTC
#define enter PINC1
#define up PINC2
#define down PINC3

void init_keys();
uint8_t _key(uint8_t value, uint8_t pace);


#endif /* SCANKEYS_H_ */