// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"

uint16_t byteArray[3];
struct packet pp;
uint32_t data;
uint8_t counter = 0;
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
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    { /* received notification */
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set)
        { 
          /* subscribed interrupt */
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
        break; /* no other notifications expected: do nothing */
      }
    }
    else
    { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != 0) return 1;
  if (issueCommand(REP_DISABLE) != 0)  return 1;

  return 0;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt)
{
	return 0; 
}

int(mouse_test_async)(uint8_t idle_time)
{

  return 0;
}

//int(mouse_test_gesture)()
//{
/* To be completed */
//printf("%s: under construction\n", __func__);
//return 1;
//}


