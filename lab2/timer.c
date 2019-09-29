#include "i8254.h"
#include "timer.h"
#include "utils.h"
#include <stdlib.h>

int hook_id = 0; 
int counter = 0; 
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t temp;                 //stores values for LSB and MSB
  uint8_t mask_MSB_LSB = 0x0F;  //mask for 4 least significant bits
  uint16_t value;               //initial value for the timer
  uint8_t controlWord = 0x00;   //stores the control word
  uint8_t timerStatus;          //stores value for the status


  //checking possible invalid values for timer
  if (timer> 2 || timer<0){
    printf("ERR: timer_set_frequency --timer** error sending information to control register \n");
    return 1; 
  }

  //getting the configuration for timer
  if(timer_get_conf(timer, &timerStatus)!= OK){
    printf("ERR: timer_set_frequency --status** error calling timer_get_conf");
    return 2;
  }

  //calculating initial value for the timer
  value = TIMER_FREQ/freq; 

  //checking the other bits
  timerStatus = timerStatus & (mask_MSB_LSB);   //preserving the last significant bits of timer status
  controlWord = (controlWord | timerStatus) | TIMER_LSB_MSB; 

  //selecting which timer to use from function argument
  switch (timer){
    case 0: 
      controlWord |= TIMER_SEL0;
      break;
    case 1: 
      controlWord |= TIMER_SEL1; 
      break;
    case 2: 
      controlWord |= TIMER_SEL2; 
      break; 

  }

  //sending controlWorld to the timer control register for configuration
  if (sys_outb(TIMER_CTRL, controlWord) != OK){
    printf("ERR: timer_set_frequency --sys_outb** error sending information to control register \n "); 
      return 3; 
  }  
  //loading timer with value's lsb obtained by using util_get_LSB
  util_get_LSB(value,&temp); 
  if(sys_outb(TIMER_0+timer, temp) != OK){
    printf("ERR: timer_set_frequency --sys_inb** error reading information from timer register \n");
      return 4; 
  }
  //loading timer with value's msb obtained by using util_get_MSB
  util_get_MSB(value, &temp); 
  if(sys_outb(TIMER_0+timer, temp) != OK){
    printf("ERR: timer_set_frequency --sys_inb** error reading information from timer register \n");
      return 5; 
  }


  return 0;
}

//----------------------------------------------------------------------------

int (timer_subscribe_int)(uint8_t *bit_no) {
  
  *bit_no = BIT(hook_id); 

  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK){
    printf("ERR: timer_subscribe_int --sys_irqsetpolicy** error executing\n"); 
    return 1;  
  }

  /*//if add this code, it get's an error because irqsetpolicy already set the bit to 1
    if(sys_irqenable(&hook_id) != OK){
    printf("ERR: timer_subscribe_int --sys_irqenable* error executing\n"); 
    return 2;
  }*/  

  return 0;
}

//----------------------------------------------------------------------------

int (timer_unsubscribe_int)() {
 
  if(sys_irqdisable(&hook_id)!=OK){
    printf("ERR: timer_unsubscribe_int --sys_irqenable* error executing\n"); 
    return 1; 
  } 

  if ( sys_irqrmpolicy(&hook_id)!= OK){
    printf("ERR: timer_unsubscribe_int --sys_irqenable* error executing\n"); 
    return 2;

  }
  return 0; 

}

//----------------------------------------------------------------------------

void (timer_int_handler)() {
  counter++;
}

//----------------------------------------------------------------------------

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  //getting errros out of range
  if (timer>2 || timer<0){
    printf("ERR: timer_get_conf --timer** timer variable out of range \n");
    return 1; 
  }

  //READBACK command (slide 5 page 9)
  //sending information to the control register
  //selecting count, because when it's set when it's zero
  if(sys_outb(TIMER_CTRL ,TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)) != OK){
    printf("ERR: timer_get_conf --sys_outb** error sending information to control register \n"); 
    return 2; 
  }

  //READBACK reading status
  //getting information from the timer register
  uint32_t temp; 
  if(sys_inb(TIMER_0+timer,&temp) != OK){
    printf("ERR: timer_get_conf --sys_inb** error reading information from timer register");
    return 3; 
  } 

  *st = (uint8_t) temp; 

  return 0; 
}

//----------------------------------------------------------------------------

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  uint8_t timer_base;
  uint8_t timer_init;
  uint8_t timer_mode;
  uint8_t timer_nullcount;
  uint8_t timer_output;
  uint8_t initMask = 0x30,modeMask = 0x0E;
  union timer_status_field_val fieldVal;

  if (timer_get_conf(timer,&st) != OK){
    printf("ERR: timer_display_conf --timer_get_conf** error reading the timer configuration \n"); 
      return 1; 
  } 
  //Get timer characteristics by apllying a specific mask to isolate each spec
  timer_base = (st & BIT(0));
  timer_init = (st & initMask) >> 4;
  timer_mode = (st & modeMask) >> 1;
  timer_nullcount = (st & BIT(6)) >> 6;
  timer_output = (st & BIT(7)) >> 7;

  //Dealing with bit 4 that must be ignored at times.
  if(timer_mode == 7 || timer_mode == 6) timer_mode -= 4;
  //timer_print_config();

  switch(field){
    case tsf_all:
      fieldVal.byte = st;
      timer_print_config(timer,tsf_all,fieldVal);
      break;
    case tsf_initial:
      fieldVal.in_mode = timer_init;
      timer_print_config(timer,tsf_initial,fieldVal);
      break;
    case tsf_mode:
      fieldVal.count_mode = timer_mode;
      timer_print_config(timer,tsf_mode,fieldVal);
      break;
    case tsf_base:
      fieldVal.bcd = timer_base;
      timer_print_config(timer,tsf_base,fieldVal);
      break;
    default:
      printf("ERR: INVALID STATUS----TERMINATING NOW----\n");
      return 2;
      break;
  }
  return 0;

}
