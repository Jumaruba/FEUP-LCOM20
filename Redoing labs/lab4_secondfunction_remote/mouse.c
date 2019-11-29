
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"

int hook_id = 12; 
uint16_t data = 0; 
uint8_t bytes[3]; 
extern uint8_t counter; 

int subscribe_mouse(int *bit_no){
  *bit_no = BIT(hook_id); 
  if(sys_irqsetpolicy(HOOK_ID, IRQ_EXCLUSIVE | IRQ_REENABLE, &hook_id)) return 1; 
  return 0;
}

int unsubscribe_mouse(){
  if(sys_irqrmpolicy(&hook_id)) return 1; 
  return 0;
}

int write_kbc(uint8_t addr, uint8_t cmd){
  uint32_t status; 
    for (int i = 0; i< 20; i++){
    if(sys_inb(STATUS_REG, &status)) return 1; 
    if(status & IBF) continue; 
    if ((status &TIME_OUT) | (status & PAR)) return 1; 
    if(sys_outb(addr, cmd)) return 1; 
    return 0; 
  }
  return 0; 
}

int issue_command(uint8_t cmd){
  uint32_t ack;
  do{
    if(write_kbc(CTRL_REG, INIT_CMD)) return 1; 
    if(write_kbc(IN_BUF, cmd)) return 1; 
    tickdelay(micros_to_ticks(DELAY_US)); 
    if(sys_inb(OUT_BUF, &ack)!= OK) return 1; 
  }while (ack!= ACK); 
  return 0; 
}

int read_mouse(){
  uint32_t status; 
  for (int i = 0; i< 20; i++){
    if(sys_inb(STATUS_REG, &status)) return 1; 
    if(!(status & OBF)) continue; 
    if ((status &TIME_OUT) | (status & PAR)) return 1; 
    if(sys_inb(OUT_BUF, (uint32_t *) &data)) return 1; 
    return 0; 
  }
  return 1; 
}

void (mouse_ih)(){
  read_mouse(); 
}

void handle_sync(){
  if((BIT(3) & data) && counter == 0){
    bytes[0] = data;
    counter++; 
  }
  else if(counter == 1){
    bytes[1] = data; 
    counter++; 
  }
  else if(counter == 2){
    bytes[2] = data; 
    counter ++; 
  }
}

void parse_packet(struct packet *pp){
  for (int i = 0; i< 3; i++)
    pp->bytes[i] = bytes[i]; 
  pp->lb = bytes[0] & BIT(0) ? true: false; 
  pp->rb = bytes[0] & BIT(1) ? true: false;
  pp->mb = bytes[0] & BIT(2) ? true: false;
  pp-> x_ov = bytes[0] & BIT(6) ? true: false;
  pp-> y_ov = bytes[0] & BIT(7) ? true: false;
  pp-> delta_x = bytes[0] & BIT(4)? 0xFF00 | bytes[1] : bytes[1]; 
  pp-> delta_y = bytes[0] & BIT(5)? 0xFF00 | bytes[2] : bytes[2];  
}