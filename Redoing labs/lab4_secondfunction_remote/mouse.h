#ifndef _MOUSE_H_
#define _MOUSE_H_


#define HOOK_ID             12
#define OUT_BUF             0x60
#define IN_BUF              0x60
#define STATUS_REG          0x64
#define PAR                 BIT(7)
#define TIME_OUT            BIT(6)
#define IBF                 BIT(1)
#define OBF                 BIT(0)
#define INIT_CMD            0xD4
#define DELAY_US            20000
#define ACK                 0xFA
#define ENABLE_DATA         0xF4
#define DISABLE_DATA        0xF5
#define CTRL_REG            0x64
#define READ_DATA           0xEB
#define SET_STREAM_MODE     0xEA
#define WRITE_CMD           0x60

int subscribe_mouse(int *bit_no); 
int unsubscribe_mouse(); 
int issue_command(uint8_t cmd);
int write_kbc(uint8_t addr, uint8_t cmd); 
void handle_sync(); 
void parse_packet(struct packet *pp); 
int read_mouse();


#endif
