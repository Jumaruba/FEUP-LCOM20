// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include "vbe.h"
#include "graphic.h"
#include "sprite.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "i8042.h"
#include "i8254.h"
//#include "xpm.h"


// Any header files included below this line should have been created by you
int XBOUND=100000;
int YBOUND=100000;
extern uint8_t data; 
extern int contador;
uint8_t counter = 0;
extern struct packet pp;
uint32_t data_mouse; 
uint16_t byteArray[3];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

static int print_usage() {
  printf("Usage: <mode - hex>\n");

  return 1;
}

int(proj_main_loop)(int argc, char *argv[]) {
  uint8_t *mem_video; 
  mem_video = vg_init(INDEX_MODE);
  
  if (mem_video == NULL) return 1;  
   
  //drawMenu(mem_video); 
  //drawCannon_right(10); 
  //drawCannon_left(1000);

  message msg; 
  int r; 
  int ipc_status; 
  uint8_t irq_set;
  int irq_set_mouse; 
  int mouse_x = 0, mouse_y = 0; 
  

  if(keyboard_subscribe(&irq_set) != 0) return 1;                 //interrupt upon keyboard use
  if(mouse_subscribe_int(&irq_set_mouse)) return 1;                //interrupt upon mouse use

  while( data != KBC_KC_ESC) { 
    if(issueCommand(READ_DATA) != OK) return 1;
    handleSync(); 
    if(counter == 3){
        parsePacket();
        if (mouse_x+pp.delta_x >= 0 && mouse_x+pp.delta_x <= 1024)
          mouse_x += pp.delta_x;
        if (mouse_y+pp.delta_y >= -768 && mouse_y+pp.delta_y <= 0) 
          mouse_y += pp.delta_y; 
        counter++;
        counter = 0;
        drawMouse(mouse_x, -mouse_y); 
        tickdelay(micros_to_ticks(2*1000));
      }

    //vg_draw_rectangle(0, 0,1024, 768, 0);

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:        
          if (msg.m_notify.interrupts & irq_set) { 
            keyboard_handler();
          }
          
        break;
        default:
          break;  
      }
    } else{ 
      continue;
    }
  }
  
  if(keyboard_unsubscribe()) return 1;    
  if(mouse_unsubscribe_int()) return 1; 
  if(vg_exit()) return 1; 
  return 0;

  //
  // if you're interested, try to extend the command line options so that the usage becomes:
  // <mode - hex> <minix3 logo  - true|false> <grayscale - true|false> <delay (secs)>
  //
  bool const minix3_logo = true;
  bool const grayscale = false;
  uint8_t const delay = 5;
  uint16_t mode;

  if (argc != 1)
    return print_usage();

  // parse mode info (it has some limitations for the sake of simplicity)
  if (sscanf(argv[0], "%hx", &mode) != 1) {
    printf("%s: invalid mode (%s)\n", __func__, argv[0]);

    return print_usage();
  }

  return proj_demo(mode, minix3_logo, grayscale, delay);
}

