#include <lcom/lcf.h>
#include <lcom/lab2.h>
#include <stdbool.h>
#include <stdint.h>

extern int counter; 
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}




int (timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  //Checks if timer variable is within desired range[0,2]
  if (timer>2 || timer<0){
    //printf("ERR: timer_get_conf --timer** timer variable out of range \n");
    return 1; 
  }

  //allocates memory for status word
  uint8_t *st = (uint8_t*) malloc(sizeof(st));
  if(st == NULL){
    //printf("COULD NOT ALLOCATE MEMORY\n");
    return 1;
  } 
  //gets status word from timer
  if (timer_get_conf(timer, st) != 0){
    //printf("ERR: timer_test_read_config --timer_get_conf** Impossible to read config \n"); 
    return 2; 
  } 
  //Checks if information was displayed properly(no errors returned)
  if(timer_display_conf(timer, *st, field) != 0){
    //printf("ERR: timer_test_read_config --timer_display_conf** Impossible to display config \n"); 
    return 3; 
  } 

  return 0;
}

int (timer_test_time_base)(uint8_t timer, uint32_t freq) {
  //Only calls timer_set_frequency() (commentary really necessary? =D)
  if(timer_set_frequency(timer, freq) != 0){
    //printf("ERR: timer_test_time_base --timer_set_frequency** Impossible to set frequency \n"); 
    return 1; 
  }
  
  return 0; 
}

int (timer_test_int)(uint8_t time) {
if(time < 0 || time > 255){
  //printf("ERR: timer_test_int --time** error from negative value");
  return 7;
}
int time_freq = time * 60; //Standard frequency = 60Hz
int ipc_status;	
message msg;		
int r;  
uint8_t irq_set; 		//mask: Since hook_id=0,sets first bit to 1 for TIMER_0
//Sets irq_set(hook_id) bit to 1
if (timer_subscribe_int(&irq_set)!= 0){ 
	//printf("ERR: timer_test_int --timer_subscribe_int** impossible to subscribe \n"); 
	return 1; 
} 

while( counter < time_freq) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      //printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 				
          if (msg.m_notify.interrupts & irq_set) { 
            //to do if interrupt is detected
            timer_int_handler(); 
            if(counter %60 == 0){
              timer_print_elapsed_time();
            }           
          }
        break;
        default:
          break; 	
      }
    } else { 
        continue;
      }
}
	if (timer_unsubscribe_int() != 0){
		//printf("ERR: timer_test_int --timer_unsubscribe_int** impossible to unsubscribe \n"); 
		return 2; 
	}

	return 0;

 }

	
