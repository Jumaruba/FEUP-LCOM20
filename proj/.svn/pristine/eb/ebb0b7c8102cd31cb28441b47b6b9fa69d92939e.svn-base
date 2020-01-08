#ifndef _RTC_C_
#define _RTC_C_
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <lcom/vbe.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
extern uint8_t hour; 

/**
 * @file rtc.h
 * @defgroup rtc rtc
 * @brief This file contains the rtc functions
 */


/**
 * @brief This functions reads a register from the RTC
 * 
 * @param number Value returned by the register
 * @param port Register to read the information
 * @return int 0 in case of success, 1 otherwise
 */
int read_rtc(uint8_t * number, uint8_t port); 
/**
 * @brief This function checks if the RTC is in the BCD format
 * 
 * @param bcd 1 case in RTC format, 0 otherwise
 * @return int 0 case of success, 1 otherwise
 */
int check_bcd(bool * bcd); 
/**
 * @brief Case the RTC is in the BCD format, we convert it to binary
 * 
 * @param number Number in BCD format to be converted
 */
void convert_binary(uint8_t *number); 




#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0x71 
#define HOURS               0x04 
#define MINUTES             0x02
#define MONTH               0x08
#define REG_A               0x0A
#define REG_B               0x0B 
#define BIT_DM              BIT(2)   
#define BIT_UIP             BIT(7)

#endif 
