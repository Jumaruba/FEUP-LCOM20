#include "mouse.h"
#include <lcom/lcf.h>
#include <stdio.h>

extern uint16_t byteArray[3];
extern uint32_t data;
extern int counter; 
int mouse_hook_id = 12;

int mouse_subscribe_int(int *bit_no)
{
  (*bit_no) = BIT(mouse_hook_id);
  if (sys_irqsetpolicy(IRQ12_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) return 1;
  return 0;
}

int mouse_unsubscribe_int()
{
  if (sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
  return 0;
}

void(mouse_ih)(void)
{
  mouse_read(); 
}

//issue command to the kbc
int kbc_write(uint8_t addr,uint8_t byte){
	uint32_t status;
	
	for(int i = 0;i<TRIES;i++){
		if(sys_inb(STAT_REG, &status) != 0) continue; //error reading status,try again
		
		while((status & KBC_IBF)) {};                 //wait until it's not full
		if(sys_outb(addr,byte) != 0) continue;        //error writing to register,retry
		if (!(status & (KBC_PAR_ERR | KBC_TO_ERR))) return 0; 
		else return -1; 
    
	}

	return -1;
}


int mouse_read(){
  uint32_t status; 
  for (int i = 0; i< 20; i++){
    if(sys_inb(STAT_REG, &status) != OK ) continue;
    if (!(status & KBC_OBF)) continue; 
    if ((status & (KBC_TO_ERR|KBC_PAR_ERR))) continue; 
    if (sys_inb(OUT_BUF, &data) != OK ) continue; 
    return 0; 
  }
  return 1;
}

void parsePacket()
{
  for (int i = 0; i< 3; i++)
    pp.bytes[i] = byteArray[i]; 

  pp.lb = (byteArray[0]&LB) ? true: false; 
  pp.rb = (byteArray[0]&RB) ? true: false;
  pp.mb = (byteArray[0]&MB) ? true: false;
  pp.x_ov = (byteArray[0]&X_OVFL) ? true: false;
  pp.y_ov = (byteArray[0]&Y_OVFL) ? true: false;
  pp.delta_x = (byteArray[0]&NEG_X) ? (0xFF00 |byteArray[1]) : byteArray[1];
  pp.delta_y = (byteArray[0]&NEG_Y) ? (0xFF00 |byteArray[2]) : byteArray[2];
}

int keyboard_read(){
	//first, must send a command
	uint32_t stat; 
	for (int i = 0; i < TRIES; i++){
		
		if(sys_inb(STAT_REG, &stat)) continue;		//read status, case error try again 
		
		if (stat & KBC_OBF){
			if (sys_inb(OUT_BUF, &data)) continue;	//read data, case error try again 
			if (!(stat & (KBC_PAR_ERR | KBC_TO_ERR))) return 0; 
			else return 1;  
		}
		tickdelay(micros_to_ticks(DELAY_US)); 
	}
	
	return 1;
	//then read the status
}

//issue a command to the mouse
int issueCommand(unsigned long cmd)
{
  uint32_t ack;

  do
  {
    if (kbc_write(IN_BUF, CMD_INIT) != OK)  return 1;
    if (kbc_write(OUT_BUF, cmd) != OK) return 1;

    tickdelay(micros_to_ticks(DELAY_US));
    if (sys_inb(OUT_BUF, &ack) != OK) return 1;

  } while (ack != ACK);

  return 0;
}

void handleSync(){
  if ((data & BIT(3)) && counter == 0)
      {
        byteArray[0] = data;
        counter++; 
      }

  else if (counter == 1)
    {
      byteArray[1] = data;
      counter++; 
    }

    else if (counter == 2)
    {
      byteArray[2] = data;
      counter++;
    }

}

int resetKBC(){
  uint8_t cmd = minix_get_dflt_kbc_cmd_byte();
  if(kbc_write(CMD_REG,KBC_CMD_W) != OK) return 1;
  if(kbc_write(OUT_BUF,cmd) != OK) return 1;
  return 0;
}




void mouse_event(events *event){
  if(pp.lb && !pp.mb && !pp.rb) *event = LB_DN; 
  else if(!pp.lb && !pp.mb && !pp.rb && *event == LB_DN) *event = LB_UP; 
  else if(!pp.lb && !pp.mb && pp.rb) *event = RB_DN; 
  else if(!pp.lb && !pp.mb && !pp.rb && *event == RB_DN) *event = RB_UP; 
  else if (!pp.lb && pp.mb && !pp.rb ) *event = BUTTON_E; 
  else if(!pp.lb && !pp.mb && !pp.rb && *event == BUTTON_E) *event = BUTTON_E; 
  else *event = MOUSE_MV; 
  return;
}
