/*
 * irq.c
 *
 *  Created on: Sep 13, 2023
 *      Author: ishaburange
 */
// ref : Lec-5  PG:34
#include "src/scheduler.h"
#include "src/irq.h"
#include "em_core.h"
#include "src/timers.h"
#include "app.h"
#include "em_i2c.h"
#include "sl_i2cspm.h"

#define INCLUDE_LOG_DEBUG (1)
#include "src/log.h"

int rollover_cnt = 0;
uint32_t milliseconds = 0;
/*
 * @brief: Function to initialize the LETIMER0_IRQHandler
 */

void
LETIMER0_IRQHandler (void)
{
  /*
   * Step 1: determine source of interrupt
   * Step 2: clear source of interrupt
   * Step 3: perform Work
   */

  //critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  //step 1
  uint32_t source = LETIMER_IntGetEnabled (LETIMER0);

  //clear the interrupt or the pending interrupt
  LETIMER_IntClear (LETIMER0, source);
  //critical section
  CORE_EXIT_CRITICAL();

  if (source & LETIMER_IF_UF)
    {
      //set schedular event
      schedulerSetEventUF();

      //critical section
      CORE_DECLARE_IRQ_STATE;
      CORE_ENTER_CRITICAL();

      rollover_cnt++;

      //critical section
      CORE_EXIT_CRITICAL();

    }
  else if (source & LETIMER_IF_COMP1)
    {
      //Disable the COMP1 interrupt after the requested amount of time expires
      LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP1);

      //call schedular for comp1 event
      schedulerSetEventCOMP1();
    }
  milliseconds = (rollover_cnt * 3000) + ((((ACTUAL_CLK_FREQ - LETIMER_CounterGet(LETIMER0))/ACTUAL_CLK_FREQ) * 3000));
}

uint32_t letimerMilliseconds ()
{
  return milliseconds;
}

void I2C0_IRQHandler(void) {

  I2C_TransferReturn_TypeDef transferStatus;

  transferStatus = I2C_Transfer(I2C0);

  if(transferStatus == i2cTransferDone){
      //I2C done
      schedulerSetEventI2C();
  }

  if(transferStatus < 0)
  {
      LOG_ERROR("I2C FAILED : %d",(uint32_t)transferStatus);
  }
}

