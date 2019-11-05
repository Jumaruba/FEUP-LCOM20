#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "i8042.h"
#include <stdint.h>

extern struct packet pp;


typedef enum {
	INIT,
	FIRSTL,
	VERTEX,
	SECONDL,
	FINAL
}states;

typedef enum {

	LB_UP,
	LB_DN,
	RB_UP,
	RB_DN,
	BUTTON_E,
	MOUSE_MV
}events;

typedef struct state_machine{
	double prevS;
	double actualS;
	double prevX;
	double prevY;
	double actualX;
	double actualY;
}gesture;


int mouse_subscribe_int(int *bit_no);       //subscribe the mouse
int mouse_unsubscribe_int();                //unsubscribe the mouse
void (mouse_ih)();                          //basically reads the data
int mouse_read();                           //it reads the data
void parsePacket();                         //it parses the bytesArray to the pp struct
int issueCommand(unsigned long cmd);        //it issues a command to the mouse
int kbc_write(uint8_t addr,uint8_t byte);   //it issues a command to the kbc
void handleSync();                          //it handles the three bytes
int resetKBC();								//resets kbc commmand to default
void mouse_event(events *event);

#endif
