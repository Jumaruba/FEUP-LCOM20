#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "i8042.h"

extern struct packet pp;

int mouse_subscribe_int(int *bit_no);
int mouse_unsubscribe_int();
void (mouse_ih)();
int mouse_read(); 
void parsePacket();
int issueCommand(unsigned long cmd);
void handleSync();

#endif
