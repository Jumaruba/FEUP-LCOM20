#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <lcom/lcf.h>

/**
 * @file utils.h
 * @brief File containing some useful functions 
 */

/**
 * @brief Get the least significant bits in a value
 * @param val Value to extract the lsb
 * @return bits extracted 
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);
/**
 * @brief Extract the most significant bits in a value
 * @param val Value to extract the msb
 * @return bits extracted
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);
/**
 * @brief Function to avoid doing cast to (uint32_t *) to sys_inb every time
 * 
 */
int (util_sys_inb)(int port, uint8_t *value);
/**
 * @brief Converts a number to it's binary version in char
 * @param size Size of the number
 * @param n Number to be converted
 */
char * (to_binary)(uint32_t size,uint32_t n);
#endif
