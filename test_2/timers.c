/*
 * timers.c
 *
 *  Created on: Sep 13, 2023
 *      Author: ishaburange
 */

#include "src/timers.h"
#include "src/oscillators.h"

#define INCLUDE_LOG_DEBUG (1)
#include "src/log.h"

#if (LOWEST_ENERGY_MODE < 3)
#define RESOLUTION 122
#define WAINT_MIN_TIME 122

#else
#define RESOLUTION 1000
#define WAIT_MIN_TIME 1000
#endif

#define WAIT_MAX_TIME 3000000  //maximum wait time possible

uint32_t temp;

const LETIMER_Init_TypeDef letimerInitData =
  {
  false,
  true,
  true,
  false, 0, 0, letimerUFOANone, letimerUFOANone, letimerRepeatFree, 0 };

/*
 * @brief: Function to initialize the Timer
 */

void initLETIMER0 ()
{

  LETIMER_Init (LETIMER0, &letimerInitData);

  LETIMER_CompareSet (LETIMER0, 0, VALUE_TO_LOAD_COMP0); //for COPM0

  LETIMER_IntClear (LETIMER0, 0xFFFFFFFF);

  temp = LETIMER_IEN_UF ;

  LETIMER_IntEnable (LETIMER0, temp);

  LETIMER_Enable (LETIMER0, true);

  // LOG_INFO("Timer Initialized");

} //initLETIMER0 ()

void
timerWaitUs (uint32_t us_wait)
{

  uint32_t ticks_needed, ticks_done, ticks_required;

  if (us_wait < WAIT_MIN_TIME)
    {
      us_wait = WAIT_MIN_TIME;
    }
  else if (us_wait > WAIT_MAX_TIME)
    {
      us_wait = WAIT_MAX_TIME;
    }

  ticks_needed = (us_wait / RESOLUTION);
  ticks_done = LETIMER_CounterGet (LETIMER0);
  ticks_required = ticks_done - ticks_needed;

  if (ticks_done >= ticks_needed)
    {
      while ((LETIMER_CounterGet (LETIMER0)) != (ticks_required))
        ;
    }
  else
    {
      while ((LETIMER_CounterGet (LETIMER0))!= VALUE_TO_LOAD_COMP0 - (ticks_required))
        ;
    }
}

void timerWaitUs_irq (uint32_t us_wait)
{
  uint32_t ticks_needed, ticks_done, ticks_required;

  if (us_wait < WAIT_MIN_TIME)
    {
      us_wait = WAIT_MIN_TIME;
    }
  else if (us_wait > WAIT_MAX_TIME)
    {
      us_wait = WAIT_MAX_TIME;
    }

  ticks_needed = (us_wait / RESOLUTION);
  ticks_done = LETIMER_CounterGet (LETIMER0);
  ticks_required = ticks_needed - ticks_done;

  //wrapping

  if (ticks_required > VALUE_TO_LOAD_COMP0)
    {
      ticks_required = VALUE_TO_LOAD_COMP0 - (0xFFFF - ticks_required);
    }

  LETIMER_CompareSet(LETIMER0,1,ticks_required); //LOAD THE VALUE IN COMP1

  //COMP1 enable command
   LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP1);

   //clear comp1 bits
   LETIMER0->IEN &= ~(LETIMER0->IEN);

   //set comp1 bits
  LETIMER0->IEN |= LETIMER_IEN_COMP1;
}

