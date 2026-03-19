/*
 * schedular.c
 *
 *  Created on: Sep 22, 2023
 *      Author: ishaburange
 */

#include "src/scheduler.h"
#include "src/i2c.h"
#include "src/log.h"
#include "src/gpio.h"

typedef enum uint32_t {
  STATE0_IDLE,
  STATE1_TIMERWAIT, //DELAY
  STATE2_WRITEI2C,  //WRITE TO THE I2C
  STATE3_TIMERWAIT, //DELAY
  STATE4_READI2C,   //READ I2C
  MY_STATES_NUM
}states_t;

uint32_t myEvents;

uint32_t getNextEvent ()
{
  int32_t theEvent;

  if (myEvents & evtUF)
    {
      theEvent = evtUF;
    }

  else if (myEvents & evtCOMP1)
    {
      theEvent = evtCOMP1;
    }
  else if(myEvents & evtI2C)
    {
      theEvent = evtI2C;
    }

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  myEvents &= ~(theEvent); //clear the bits

  CORE_EXIT_CRITICAL();

  return (theEvent);

}

void schedulerSetEventUF ()
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  myEvents |= evtUF; //set the event

  CORE_EXIT_CRITICAL();

}

void schedulerSetEventCOMP1()
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  myEvents |= evtCOMP1;

  CORE_EXIT_CRITICAL();
}

void schedulerSetEventI2C()
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  myEvents |= evtI2C;

  CORE_EXIT_CRITICAL();
}
void temperature_state_machine(uint32_t event){

  states_t currentState ;
  static states_t nextState = STATE0_IDLE;
  currentState = nextState;

  switch (currentState){

    case STATE0_IDLE:
      nextState = STATE0_IDLE; //default

      //underflow flag check
      if(event & evtUF){

          //underflow flag set
          sensorSetOn();

          timerWaitUs_irq(80000);

          nextState = STATE1_TIMERWAIT;
      }
      break;

    case STATE1_TIMERWAIT:
       nextState = STATE1_TIMERWAIT; //default

       if(event & evtCOMP1)
         {
           cmd_write();

           //sleeps to EM1 while waiting for the I2C transfer to complete
           sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);

           nextState = STATE2_WRITEI2C;
         }
       break;

    case STATE2_WRITEI2C:
      nextState = STATE2_WRITEI2C; //default

      if(event & evtI2C){
          //remove EM1
          sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);

          timerWaitUs_irq(10800);

          nextState = STATE3_TIMERWAIT;
      }
      break;

    case STATE3_TIMERWAIT:
      nextState = STATE3_TIMERWAIT;

      if(event & evtCOMP1){

          cmd_read();

          //sleeps to EM1 while waiting for the I2C transfer to complete.
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);

          nextState = STATE4_READI2C;

      }
      break;

    case STATE4_READI2C:
      nextState = STATE4_READI2C;

      if(event & evtI2C){
          //remove EM1
          sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);

          //disable sensor
          sensorSetOff();

          //disable NVIC
          NVIC_DisableIRQ(I2C0_IRQn);

          //display temperature on serial monitor
          LOG_INFO("TEMPERATURE %f\n\r",temp_conversion());

          nextState = STATE0_IDLE;
      }
      break;

    default : //error
      LOG_ERROR("DEFAULT CASE, OUT OF BOUNDS\n\r");

      break;

  }

}
