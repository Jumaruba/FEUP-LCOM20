#include "i8042.h"
#include "i8254.h"
#include <stdlib.h>

uint32_t sys_counter = 0; 
 
uint8_t data = 0x00;
uint8_t code_bytes[2]; 
uint8_t size = 0; 
bool isTwoBytes; 
int KC_hook_id = 1; 
bool type; 

int sys_inb_cnt(port_t port, uint32_t *byte){
	sys_counter++;
	return sys_inb(port, byte); 
}

int keyboard_subscribe(uint8_t *bit_no){
	*bit_no = BIT(KC_hook_id); 
	if (sys_irqsetpolicy(KB1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &KC_hook_id ) != OK)
		return 1; 
	return 0; 
}

int keyboard_unsubscribe(){
	if (sys_irqdisable(&KC_hook_id) != OK)
		return 1; 
	if (sys_irqrmpolicy(&KC_hook_id) != OK)
		return 1; 
	return 0; 
}


int keyboard_read(){
	//first, must send a command
	uint32_t stat; 
	for (int i = 0; i < TRIES; i++){
		
		if(sys_inb_cnt(STAT_REG, &stat)) continue;		//read status, case error try again 
		
		if (stat & KBC_OBF){
			if (sys_inb_cnt(OUT_BUF, (uint32_t*)&data)) continue;	//read data, case error try again 
			if (!(stat & (KBC_PAR_ERR | KBC_TO_ERR| KBC_MOUSE))) return 0; 
			else return 1;  
		}
		tickdelay(micros_to_ticks(DELAY_US)); 
	}
	
	return 1;
	//then read the status
}


void keyboard_handler(){
	keyboard_read(); 
  }


int keyboard_write(uint8_t addr,uint8_t byte){
	uint32_t status;
	
	for(int i = 0;i<TRIES;i++){
		if(sys_inb_cnt(STAT_REG, &status) != 0) continue; //error reading status,try again
		
		if(!(status & KBC_IBF)){
			if(sys_outb(addr,byte) != 0) continue; //error writing to register,retry
			if (!(status & (KBC_PAR_ERR | KBC_TO_ERR| KBC_MOUSE))) return 0; 
			else return -1; 
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	return -1;
}

int keyboard_display_scans(uint8_t* data){
	if (*data == 0xE0){
		isTwoBytes = true; 
		code_bytes[0] = 0xe0; 
 	}

 	else if (isTwoBytes){
 		code_bytes[1] = *data;
 		type = !(*data & 0x80); 
 		size = 2; 
 		isTwoBytes = false; 	
 	}
 	else{
 		isTwoBytes = false; 
 		code_bytes[0] = *data; 
 		type = !(*data & 0x80); 
 		size = 1; 
 		
 	}
 	return 0; 

}
