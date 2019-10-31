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
  //we don't need to subscribe, because we are sending a command to the kbc telling it to send information from the mouse
  while (cnt > 0){
    tickdelay(micros_to_ticks(period*1000));        //reads the information upon a period. Dealay is in ms*10^(-3)
    if(issueCommand(READ_DATA)) return 1;           //request to read the three bytes
    cnt--; 
    while (counter < 3){  
      if (mouse_read()) return 1;                   //information goes to data variable
      handleSync();                                 //store information at byteArray
    }
    counter = 0; 
    parsePacket();                                  //store the information at the struct pp 
    mouse_print_packet(&pp);                        //prints the data
  }
  if(issueCommand(SET_STREAM_MODE)) return 1;       //set the stream mode again
  if(issueCommand(REP_DISABLE)) return 1;           //ensure that data report is disabled
  uint8_t default_cmd = minix_get_dflt_kbc_cmd_byte();  //getting the default command
  if(kbc_write(IN_BUF, KBC_CMD_W)) return 1;        //tell the KBC we want to write a command
  if(kbc_write(OUT_BUF, default_cmd)) return 1;     //send the default command byte to the kbc
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


