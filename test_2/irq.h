/*
 * irq.h
 *
 *  Created on: Sep 13, 2023
 *      Author: ishaburange
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

#include <stdio.h>
#include "src/scheduler.h"
#include "app.h"
#include "em_i2c.h"
#include "sl_i2cspm.h"


//Interrupt handler for LETIMER0
void LETIMER0_IRQHandler(void);

//LETIMER Milliseconds timestamp
uint32_t letimerMilliseconds();

//I2C IRQ handler
void I2C0_IRQHandler(void);

#endif /* SRC_IRQ_H_ */
