#pragma once

//APAGAR!!
#include <stdint.h>
#include "keyboard.h"


extern uint16_t *data; 
extern uint32_t inb_counter; 
/**
interrupt: while data!= KBC_KC_ESC
*/

int (kbd_test_scan)(bool assembly);
int (kbd_test_timed_scan)(uint8_t UNUSED(n)) ; 
int (kbd_test_poll)(); 
