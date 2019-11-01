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
	uint8_t counter = 0;
  uint8_t btcount=0; //current mouse byte->starts in position 0 i.e byteArray[0]//
    while(counter < cnt){
      if(btcount == 0){
        if(issueCommand(READ_DATA) != OK) return 1;
        mouse_ih();
        byteArray[btcount] = data;
        //if(data & BIT(3)) continue;
        btcount = 1;
      }
      if(btcount == 1){
        mouse_ih();
        byteArray[btcount] = data;
        btcount+=1;
      }
      if(btcount == 2){
        mouse_ih();
        byteArray[btcount] = data;
        btcount+=1;
      }
      if(btcount == 3){
        parsePacket();
        mouse_print_packet(&pp);
        counter++;
        btcount = 0;
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
            mouse_event(&event,state); 
            dX = pp.delta_x;
            dY = pp.delta_y;
            switch(state){
              case INIT:
                printf ("displacement: %d, x_len: %d\n", displacement, x_len); 
                if(event == LB_DN){
                  displacement = 0;
                  state = FIRSTL;
                }
                break;
              case FIRSTL:
                printf ("displacement: %d, x_len: %d\n", displacement, x_len); 
                displacement+=abs(dX);
                if(event == LB_UP && displacement>=x_len) state = VERTEX;
                else if(event == BUTTON_E) state = INIT;
                else if((dX < 0 && abs(dX) > tolerance) || (dY < 0 && abs(dY) > tolerance) || abs(dY) < abs(dX)){
                  state = INIT;
                }
                break;
              case VERTEX:
                printf ("displacement: %d, x_len: %d\n", displacement, x_len); 
                if(event == RB_DN){
                  displacement = 0;
                  state = SECONDL;
                }
                else if(event == LB_DN){
                  displacement = 0;
                  state = FIRSTL;
                }
                else if((event == MOUSE_MV) && (( abs(dX) > tolerance) || (dY < 0 && abs(dY) > tolerance) || (abs(dY) < abs(dX)))){
                  state = INIT;
                }

                else if(event == BUTTON_E) state = INIT;
                break;
              case SECONDL:
                displacement += abs(dX);
                printf ("displacement: %d, x_len: %d\n", displacement, x_len); 
                if(event == RB_UP && displacement >=  x_len) {
                  state = FINAL;
                  printf("HA\n");
                }
                else if(event != MOUSE_MV) state = INIT;
                else if((dX < 0 && abs(dX) > tolerance) || (dY < 0 && abs(dY) > tolerance) || abs(dY) < abs(dX)){
                  state = INIT;
                }
                
                break;
              default:
                break;

            } 
          }
          printf("state: %d\n", state);
        }
        break;
      default:
        break;
      }
    }
    else
    { 
      printf("VAI SE FUDER\n");
    }
  }
  printf("FINAL  \n"); 
  if (mouse_unsubscribe_int() != 0) return 1;
  if (issueCommand(REP_DISABLE) != 0)  return 1;
  printf("PICUINHA\n"); 

  return 0;
}



