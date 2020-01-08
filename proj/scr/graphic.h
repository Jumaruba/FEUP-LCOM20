#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdbool.h>
#include <minix/driver.h>
#include <sys/mman.h>
#include <machine/int86.h>
#include <math.h>

/**
 *
 * @param mode
 * @return
 */
int init_mode(uint16_t mode);
int vbeModeInfo(uint16_t mode, vbe_mode_info_t *mode_info);  
int drawPixel(uint16_t x, uint16_t y, uint32_t color);
uint16_t get_hres();
uint16_t get_vres(); 
uint8_t get_bits_per_pixel();
uint16_t get_GreenScreenMask(); 
uint16_t get_BlueScreenMask(); 
uint16_t get_RedScreenMask(); 
uint8_t get_bytes(); 
void drawXpm(xpm_map_t xpm, uint16_t x, uint16_t y);
void eraseXpm(xpm_map_t xpm,uint16_t x, uint16_t y);
void printEndGame(int player);
int16_t absO(int16_t x); 
xpm_map_t get_mouse_xpm(); 
int drawPixel_buffer(uint16_t x, uint16_t y, uint32_t color, uint8_t * buffer); 
void memvideo_cpy();
void malloc_buffer();
void free_buffer();
void clear_buffer();  
void blue_background();
void dark_blue_background();
//SPRITES 
xpm_map_t get_menu_name_xpm();
xpm_map_t get_menu_start_xpm();
xpm_map_t get_menu_inst_xpm();
xpm_map_t get_menu_exit_xpm();
xpm_map_t get_menu_starts_xpm(); 
xpm_map_t get_menu_exits_xpm(); 
xpm_map_t get_menu_insts_xpm(); 
xpm_map_t get_game_ground_xpm(); 
xpm_map_t get_game_cannon_xpm(); 
xpm_map_t get_game_ball_xpm();
xpm_map_t get_game_sun_xpm();
xpm_map_t get_game_moon_xpm();
xpm_map_t get_heart_xpm();
xpm_map_t get_wins1_xpm();
xpm_map_t get_wins2_xpm();
xpm_map_t get_end_xpm();
xpm_map_t get_instruction_xpm();
xpm_map_t get_space_xpm();
xpm_map_t get_dash_xpm();
xpm_map_t get_write_xpm();
xpm_map_t get_name_xpm();
xpm_map_t get_ingreis_xpm();
xpm_map_t get_player1_xpm();
xpm_map_t get_player2_xpm();
xpm_map_t get_alphabet_xpm(char letter);
xpm_map_t get_winner_xpm(); 
#endif /* __GRAPHIC_H */
