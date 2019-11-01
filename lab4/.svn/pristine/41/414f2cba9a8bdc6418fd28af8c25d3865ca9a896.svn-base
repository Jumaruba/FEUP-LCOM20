#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_


#define KB1_IRQ			1
#define IRQ12_MOUSE     12

//Packets
#define LB              BIT(0)
#define RB              BIT(1)
#define MB              BIT(2)
#define NEG_X           BIT(4)
#define NEG_Y           BIT(5)
#define X_OVFL          BIT(6)
#define Y_OVFL          BIT(7)

//commands KBC
#define KBC_CMD_R 		0x20		/**< @brief command to read*/
#define KBC_CMD_W 		0x60		/**< @brief command to write*/
#define CMD_REG			0x64


#define OUT_BUF         0x60
#define IN_BUF	 	    0x64

#define CMD_INIT        0xD4 	

//mouse stats commands
#define ACK             0xFA	
#define NACK            0xFE

#define KBC_KC_ESC 		0x81

//register read and write
#define STAT_REG        0x64 

//mouse commands
#define MOUSE_ENABLE    0xA8 
#define WRITE_TO_MOUSE  0xD4 
#define READ_DATA       0xEB
#define REP_ENABLE    	0xF4
#define REP_DISABLE   	0xF5
#define SET_STREAM_MODE 0xEA

//Status register macros 
#define KBC_PAR_ERR		BIT(7)		/**< @brief parity error bit--	check if there is an error of parity*/
#define KBC_TO_ERR		BIT(6)		/**< @brief timeout error bit*/
#define KBC_MOUSE 		BIT(5)		/**< @brief mouse bit */
#define KBC_IBF			BIT(1)		/**< @brief --input bufffer bit-- to check if it's full*/
#define KBC_OBF			BIT(0) 		//Outbuffer full


#define KBC_KC_ESC 		0x81
#define KBC_CMD_INT		BIT(0)

#define TRIES	 		20
#define DELAY_US    	20000
#endif /* _LCOM_I8042_H */
