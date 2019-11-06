// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "utils.h"

uint16_t byteArray[3];
struct packet pp;
uint32_t data;
uint8_t counter = 0;
extern int contador; 
extern bool canPrint; 

// Any header files included below this line should have been created by you

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt)
{
  int r;
  int irq_set; 
  int ipc_status;
  message msg;

  if (issueCommand(REP_ENABLE) != 0) return 1;

  if (mouse_subscribe_int(&irq_set) != 0) return 1;


  while (cnt > 0)

  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set)
        { 
          mouse_ih();
          handleSync();
          if(counter == 3){
            counter = 0; 
            cnt--; 
            parsePacket();
            mouse_print_packet(&pp);
          }

        }
        break;
      default:
        break;
      }
    }
    else
    { 
    }
  }

  if (mouse_unsubscribe_int() != 0) return 1;
  if (issueCommand(REP_DISABLE) != 0)  return 1;

  return 0;
}

//remote mode
int(mouse_test_remote)(uint16_t period, uint8_t cnt)
{
  uint8_t  counter_1 = 0; 
    while(counter_1 < cnt){
      mouse_ih();
      if(issueCommand(READ_DATA) != OK) return 1;
      handleSync(); 
      if(counter == 3){
        counter_1++;
        parsePacket();
        mouse_print_packet(&pp);
        counter++;
        counter = 0;
        tickdelay(micros_to_ticks(period*1000));
      }
    }
    if(issueCommand(SET_STREAM_MODE) != OK) return 1;
    if(issueCommand(REP_DISABLE) != OK) return 1;
  
    return resetKBC();

}

int(mouse_test_async)(uint8_t idle_time)
{

  int r;
  int irq_set; 
  int ipc_status;
  uint8_t irq_set_timer0; 
  message msg;
  int idle =idle_time* 60; 

  if (issueCommand(REP_ENABLE) != 0) return 1;
  if (mouse_subscribe_int(&irq_set) != 0) return 1;
  if (timer_subscribe_int(&irq_set_timer0)!= 0) return 1; 


  while (contador < idle)

  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set_timer0){
          timer_int_handler();                          //increment the contador
        }
        if (msg.m_notify.interrupts & irq_set)
        { 
          mouse_ih();
          handleSync();
          if(counter == 3){
            counter = 0; 
            parsePacket();
            mouse_print_packet(&pp);
            
          }
          contador = 0; 
        }
        break;
      default:
        break;
      }
    }
    else
    { 
    }
  }
  if (timer_unsubscribe_int() != 0) return 1; 
  if (mouse_unsubscribe_int() != 0) return 1;
  if (issueCommand(REP_DISABLE) != 0)  return 1;
  return 0; 
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance)
{

  int r;
  int irq_set; 
  int ipc_status;
  message msg;
  states state = INIT; 
  gesture machine;
  machine.prevS = 0;
  machine.prevX = 0.0;
  machine.prevY = 0.0;
  events event;
  int8_t dX,dY;
  int32_t total_x = 0, total_y = 0; 
  uint16_t displacement = 0;
  if (issueCommand(REP_ENABLE) != 0) return 1;

  if (mouse_subscribe_int(&irq_set) != 0) return 1;


  while (state != FINAL)

  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set)
        { 
          mouse_ih();
          handleSync();
          if(counter == 3){
            counter = 0; 
            parsePacket();
            mouse_print_packet(&pp);
            mouse_event(&event); 
            dX = pp.delta_x;
            dY = pp.delta_y;
            total_x += abs(dX); 
            total_y += abs(dY); 
            switch(state){
              case INIT: 
                if(event == LB_DN){
                  displacement = 0; 
                  state = FIRSTL;
                }
                break;

              case FIRSTL:
                displacement+=abs(dX);
                if(event == LB_UP && displacement>=x_len) state = VERTEX;
                else if(event == LB_UP  && displacement <  x_len) state = INIT;
                else if(event == RB_UP || event == BUTTON_E) state = INIT;
                else if((dX < 0 && abs(dX) > tolerance) || (dY < 0 && abs(dY) > tolerance) || (abs(total_y) < abs(total_x))) state = INIT;
                break;
              case VERTEX:
                if(event == RB_DN){
                  displacement = 0;
                  state = SECONDL;
                }
                else if(event == LB_DN){
                  displacement = 0;
                  state = FIRSTL;
                }
                else if((event == MOUSE_MV) && (( abs(dX) > tolerance) || (abs(dY) > tolerance)))
                  state = INIT;
                
                else if(event == BUTTON_E) state = INIT;
                break;

              case SECONDL:
                displacement += abs(dX);
                if(event == RB_UP  && displacement >=  x_len) state = FINAL;
                else if(event == RB_UP  && displacement <  x_len) state = INIT;
                else if(event == BUTTON_E) state = INIT; 
                else if((dX < 0 && abs(dX) > tolerance) || (dY < 0 && abs(dY) > tolerance) || abs(total_y) < abs(total_x)) state = INIT;
                
                break;
              default:
                break;

            } 
          }
        }
        break;
      default:
        break;
      }
    }
    else
    { 
    }
  }
  if (mouse_unsubscribe_int() != 0) return 1;
  if (issueCommand(REP_DISABLE) != 0)  return 1;

  return 0;
}



