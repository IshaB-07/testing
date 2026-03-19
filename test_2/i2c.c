/*
 * i2c.c
 *
 *  Created on: Sep 22, 2023
 *      Author: ishaburange
 */

#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

#include "src/i2c.h"
#include "src/gpio.h"
#include "src/timers.h"

#define SI7021_DEVICE_ADDR 0x40

//Send Measure Temperature command

I2C_TransferReturn_TypeDef transferStatus;
I2C_TransferSeq_TypeDef writeSeq;
I2C_TransferSeq_TypeDef readSeq;
uint8_t cmd_data;
uint8_t read_data[2]; //only 2 bytes

void
I2C_init ()
{
  I2CSPM_Init_TypeDef I2C_Config =
      { .port = I2C0,
        .sclPort = gpioPortC,
        .sclPin = 10,
        .sdaPort = gpioPortC,
        .sdaPin = 11,
        .portLocationScl = 14,
        .portLocationSda = 16,
        .i2cRefFreq = 0,
        .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
        .i2cClhr = i2cClockHLRStandard };

  I2CSPM_Init (&I2C_Config);
}

void
cmd_write ()
{
  I2C_init();

  cmd_data = 0xF3;
    writeSeq.addr = SI7021_DEVICE_ADDR << 1;
    writeSeq.flags = I2C_FLAG_WRITE;
    writeSeq.buf[0].data = &cmd_data;
    writeSeq.buf[0].len = sizeof(cmd_data);
  //enable I2C transfer
  NVIC_EnableIRQ(I2C0_IRQn);

  transferStatus = I2C_TransferInit (I2C0, &writeSeq);


  if (transferStatus <0)
    {
      LOG_ERROR("I2CSPM_Transfer: I2C bus write of cmd= %d failed",
                (uint32_t)transferStatus);
    }
}

void
cmd_read ()
{
  I2C_init ();

  //read data
    readSeq.addr = SI7021_DEVICE_ADDR << 1;
    readSeq.flags = I2C_FLAG_READ;
    readSeq.buf[0].data = &read_data[0];
    readSeq.buf[0].len = sizeof(read_data);

    //Config NVIC to generate an IRQ for the I2C0 module
    NVIC_EnableIRQ(I2C0_IRQn);


  transferStatus = I2C_TransferInit (I2C0, &readSeq);

  if (transferStatus <0)
    {
      LOG_ERROR("I2C_Transfer: I2C bus read of cmd= %d failed",
                (uint32_t)transferStatus);
    }
}

double
temp_conversion ()
{
  uint16_t result;

  double temp_c;

  result = (read_data[0] << 8) + read_data[1];

  temp_c = (175.72 * result) / 65536;

  temp_c -= 46.85;

  return temp_c;
}

