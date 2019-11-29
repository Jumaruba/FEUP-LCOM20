#ifndef _MOUSE_H_
#define _MOUSE_H_

#define HOOK_ID         12
#define STATUS_REG      0x64 
#define IBF             BIT(1)
#define PAR             BIT(7)
#define TIME_OUT        BIT(6)
#define OBF             BIT(0)
#define IN_BUF          0x60
#define CTRL_REG        0x64
#define WRITE_MOUSE     0xD4
#define OUT_BUF         0x60
#define DELAY_US        20000
#define ENABLE_DATA     0xF4
#define DISABLE_DATA    0xF5
#define ACK             0xFA

int write_kbc(uint8_t addr, uint8_t cmd); 
int issue_command(uint8_t cmd); 
int read_mouse(); 
int subscribe_mouse(int *bit_no); 
int unsubscribe_mouse(); 
void handle_sync(); 
void parse_packet(struct packet *pp); 


#endif
