/*
 * Timer.h
 *
 * Created on: May 9, 2012
 * Board: DRV2667EVM-CT RevA
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "CTS/structure.h"

/**
 * timerdelay - 5ms timer delay
 * @param unsigned int delay
 */
void timerdelay(unsigned int tdelay);

/**
 * sleep - sleep for a fixed time
 * @param uint16_t time
 */
void sleep(uint16_t time);

#endif /* TIMER_H_ */

