// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"

// Any header files included below this line should have been created by you

extern uint8_t data; 
extern uint16_t bytes[3]; 
struct packet pp; 
uint8_t counter = 0; 

int main(int argc, char *argv[]) {
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
//packet
//remote
//movimento

//keyboard read
//keyboard remote
int (mouse_test_packet)(uint32_t cnt) {
    /* To be completed */
    
    int ipc_status, r, irq_set;
    message msg;
        

    // XXX : you may need to add some code here
    if(issue_command(ENABLE_DATA)) return 1;
    if(subscribe_mouse(&irq_set)) return 1; 
    // interrupt loop
    while (cnt > 0) {
        // Get a request message.
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
        } 
        if (is_ipc_notify(ipc_status)) { // received notification
        
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:                              // hardware interrupt notification
            if (msg.m_notify.interrupts & irq_set) {    // subscribed interrupt
        
              mouse_ih(); 
              handle_sync();  
              if (counter == 3){
                parse_packet(&pp); 
                mouse_print_packet(&pp);
                cnt --; 
                counter = 0; 
              }
            }
            break;
            default:
            break;
        }
        }
    }
    
    if(unsubscribe_mouse()) return 1; 
    if(issue_command(DISABLE_DATA)) return 1; 
    return r;
}
int(mouse_test_remote)(uint16_t period, uint8_t cnt)
{

  while ( cnt > 0){
    if(issue_command(READ_DATA)) return 1; 
    mouse_ih(); 
    handle_sync(); 
    if(counter == 3){
      cnt--; 
      counter = 0; 
      parse_packet(&pp);
      mouse_print_packet(&pp);
      tickdelay(micros_to_ticks(period*1000)); 
    }
  }
  
  
  uint8_t cmd_default = minix_get_dflt_kbc_cmd_byte(); 
  if(write_kbc(CTRL_REG, WRITE_CMD)) return 1; 
  if(write_kbc(IN_BUF, cmd_default)) return 1;
  if(issue_command(SET_STREAM_MODE)) return 1; 
  if(issue_command(DISABLE_DATA)) return 1; 
  
  return 0;
}
