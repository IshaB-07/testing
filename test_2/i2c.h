/*
 * i2c.h
 *
 *  Created on: Sep 22, 2023
 *      Author: ishaburange
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "app.h"
#include "em_i2c.h"
#include "sl_i2cspm.h"

//Function to initialize I2C
void I2C_init();

//Function to perform write operation on the slave
void cmd_write();

//function to perform read operation on the slave
void cmd_read();

//Function for obtaining temperature sensor readings
void read_from_sensor_si7201();

//function to calculate temperature
double temp_conversion();


#endif /* SRC_I2C_H_ */
