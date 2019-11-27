#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdbool.h>

#include <minix/driver.h>
#include <sys/mman.h>
#include <machine/int86.h>
#include <math.h>

int init_mode(uint16_t mode);
int vbeModeInfo(uint16_t mode, vbe_mode_info_t *mode_info);  
int drawPixel(uint16_t x_pos, uint16_t y_pos, uint32_t color);
uint16_t get_hres();
uint16_t get_vres(); 
uint8_t get_bits_per_pixel();
uint16_t get_GreenScreenMask(); 
uint16_t get_BlueScreenMask(); 
uint16_t get_RedScreenMask(); 
uint8_t get_bytes(); 
void drawXpm(xpm_map_t xpm, uint16_t x, uint16_t y);
void eraseXpm(xpm_map_t xpm,uint16_t x, uint16_t y);
/**
 * @brief prints the end game
 * @param player 1 if player 1 wins, 2 if player 2 wins
 */
void printEndGame(int player);
int16_t absO(int16_t x); 
/**
 * @brief draw the main menu
 */
void drawMenu(uint8_t *mem_video) ;
/**
 * @brief draw the right cannon
 */
void drawCannon_right(int x);
/**
 * @brief draw the left cannon
 */
void drawCannon_left(int x); 
/**
 * @brief draw the xpm inverted
 */
void mirror_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);  
/**
 * @brief draw the mouse on its position
 * @param x position on x-axis
 * @param y position on y-axis
 * 
 */
void drawMouse(uint16_t x, uint16_t y); 
#endif /* __GRAPHIC_H */
