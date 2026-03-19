/*
 * oscillators.c
 *
 *  Created on: Sep 13, 2023
 *      Author: ishaburange
 */

#include "src/oscillators.h"
#include "em_letimer.h"

#define INCLUDE_LOG_DEBUG (1)
#include "src/log.h"

/*
 * @brief: Function to initialize the Oscillator
 */

void initOscillators()
{
  int check_freq; //clock freq
  int check_timer_freq; // timer freq

  if (LOWEST_ENERGY_MODE == 3)
    {
      CMU_OscillatorEnable (cmuOsc_ULFRCO, true, true);
      CMU_ClockSelectSet (cmuClock_LFA, cmuSelect_ULFRCO);
      CMU_ClockEnable (cmuClock_LETIMER0, true);
      CMU_ClockDivSet (cmuClock_LETIMER0, cmuClkDiv_1);

      check_freq = CMU_ClockFreqGet (cmuClock_LFA);
      check_timer_freq = CMU_ClockFreqGet(cmuClock_LETIMER0);


    }
  else
    {
      CMU_OscillatorEnable (cmuOsc_LFXO, true, true);
      CMU_ClockSelectSet (cmuClock_LFA, cmuSelect_LFXO);
      CMU_ClockEnable (cmuClock_LETIMER0, true);
      CMU_ClockDivSet (cmuClock_LETIMER0, cmuClkDiv_4);

      check_freq = CMU_ClockFreqGet (cmuClock_LFA);
      check_timer_freq = CMU_ClockFreqGet(cmuClock_LETIMER0);

    }
}
