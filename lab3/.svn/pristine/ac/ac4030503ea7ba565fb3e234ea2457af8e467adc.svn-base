#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8254.h"
#include "i8042.h"
#include "timer.h"
#include "utils.h"


extern uint8_t data; 
extern uint8_t sys_counter; 
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  message msg; 
  int r; 
  int ipc_status; 
  uint8_t irq_set; 

  if(keyboard_subscribe(&irq_set) != 0) return 1; 

  while( data != KBC_KC_ESC) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      //printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:        
          if (msg.m_notify.interrupts & irq_set) { 
            //to do if interrupt is detected
            keyboard_handler();
            keyboard_display_scans();            
          }
        break;
        default:
          break;  
      }
    } else{ 
      continue;
    }
  }
  
  kbd_print_no_sysinb(sys_counter);
  return keyboard_unsubscribe();       
}



int(kbd_test_poll)() {
  uint8_t commandByte; 

  while ( data != KBC_KC_ESC ){
    keyboard_handler(); 
    keyboard_display_scans(); 
  }

  // enable the interrupt again
  if (keyboard_write(IN_BUF, KBC_CMD_R)) return 1;        //writing we want to read the command byte
  if (keyboard_read()) return 1;                          //read the command byte
  commandByte = data;                                     //store the command byte
  commandByte|= KBC_CMD_INT;                              //set int variable to 1
  if (keyboard_write(IN_BUF, KBC_CMD_W)) return 1;;       //writing we want to write a command byte
  if (keyboard_write(WRITE_CMD, commandByte)) return 1;   //writing the command byte

  kbd_print_no_sysinb(sys_counter);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  message msg; 
  int r; 
  int ipc_status; 
  uint8_t irq_set,irq_timer0;
  int idle = n*60;

  if(keyboard_subscribe(&irq_set) != 0) return 1; 
  if(timer_subscribe_int(&irq_timer0) != 0) return 1;

  while( data != KBC_KC_ESC && counter<idle ) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:        
          if (msg.m_notify.interrupts & irq_timer0) { 
            timer_int_handler();

          }
          if(msg.m_notify.interrupts & irq_set){
            keyboard_handler();
            keyboard_display_scans();
            counter = 0; 

          }        
          
        break;
        default:
          break;  
      }
    } else{ 
      continue;
    }
  }
    if(timer_unsubscribe_int() != 0) return 1;
    if(keyboard_unsubscribe() != 0)  return 1;
    return 0;
}
