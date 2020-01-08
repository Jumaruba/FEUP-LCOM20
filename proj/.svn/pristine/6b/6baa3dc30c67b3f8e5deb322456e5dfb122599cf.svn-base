#include "rtc.h"


int read_rtc(uint8_t * number, uint8_t port){
    bool bcd = 0;           // 1 case RTC is in BCD format, 0 otherwise
    uint8_t regA = 0;       // value read from teh register_B in RTC

    //waits until the RTC is not updating anymore
    do{
        if(sys_outb(RTC_ADDR_REG, REG_A)) return 1; 
        if(sys_inb(RTC_DATA_REG, (uint32_t*) &regA)) return 1; 
    }while(regA & BIT_UIP); 

    //get the requested information 
    if (sys_outb(RTC_ADDR_REG, port)) return 1; 
    if (sys_inb(RTC_DATA_REG, (uint32_t*) number)) return 1; 
    
    //handle the format
    if(check_bcd(&bcd)) return 1; 
    if (bcd) convert_binary(number);
    return 0;        
}   

int check_bcd(bool *bcd){
    uint8_t regB = 0;                   //value read from the register_B in RTC
    if (sys_outb(RTC_ADDR_REG, REG_B)) return 1; 
    if (sys_inb(RTC_DATA_REG, (uint32_t*)&regB)) return 1; 
    
    //if DM BIT is 1, then it's in binary format
    if (regB & BIT_DM) *bcd = 0;
    else *bcd = 1; 
    return 0; 
}

void convert_binary(uint8_t * number){
    uint8_t first_part = 0x0F;          //get the number right part
    uint8_t second_part = 0xF0;         //get the number left part 
    //join the two parts 
    *number = (*number & first_part) + ((*number & second_part) >> 4) *10; 
   
}
