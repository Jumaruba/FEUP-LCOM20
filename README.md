3lab 

//functions for the first lab
int kbd_test_scan(bool asm)
int kbd_test_poll()

//functions for the second lab
int kbd_test_timed_scan(uint8_t idle)

interrupt handler:  interrupt handler to read the scancode from the KBC
Two-byte long scancodes usually use 0xE0 as their first byte. 
