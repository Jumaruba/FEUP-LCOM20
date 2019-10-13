#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>
//#include <minix/syslib>
#include <lcom/lcf.h>
#include <minix/drivers.h>
#include <minix/com.h>


#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif


int (keyboard_subscribe)(uint8_t *bit_no);	//done
int (keyboard_unsubscribe)();				//done
int (keyboard_read)();						//done
int (keyboard_write)(uint8_t addr,uint8_t byte); 
int (keyboard_display_scans)(); 			//done
int (keyboard_handler)();					//done



#endif /* __TIMER_H */
