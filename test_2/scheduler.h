/*
 * schedular.h
 *
 *  Created on: Sep 22, 2023
 *      Author: ishaburange
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <stdio.h>
#include "app.h"

enum{
  evtUF = 0x01,
  evtCOMP1 = 0x02,
  evtI2C = 0x04
};

//Scheduler routine to return 1 event to the main code and clear that event
uint32_t getNextEvent();

//Scheduler routine to set a scheduler event
void schedulerSetEventUF();

void schedulerSetEventCOMP1();

void schedulerSetEventI2C();

void temperature_state_machine(uint32_t event);

#endif /* SRC_SCHEDULER_H_ */
