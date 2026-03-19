/*
 * timers.h
 *
 *  Created on: Sep 13, 2023
 *      Author: ishaburange
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include "app.h"
#include "em_letimer.h"
#include "app.h"

//initialize the Timer
void initLETIMER0();

//Polling function
void timerWaitUs(uint32_t us_wait);

//Interrupt Function
void timerWaitUs_irq (uint32_t us_wait);

#endif /* SRC_TIMERS_H_ */
