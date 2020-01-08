#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <stdint.h>
#include "vbe.h"
#include "graphic.h"
#include "sprite.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "i8042.h"
#include "i8254.h"
#include "menu.h"
#include "game.h"

extern uint8_t data;
extern uint8_t buffer;
extern uint8_t *video_mem;
extern int contador;
extern uint8_t counter;
extern struct packet pp;
uint32_t data_mouse;        /**<Contains the data read from the mouse interrupt*/ 
uint16_t byteArray[3];      /**<Contains complete package read from the mouse*/

//SPRITES -----------------------------------------
static Sprite * mouse_sprite;       
static Sprite * start; 
static Sprite * name; 
static Sprite * exit_; 
static Sprite * inst; 
static Sprite *start_animated; 
static Sprite *inst_animated; 
static Sprite *exit_animated; 
static Sprite *instruction;

/**
 * @file menu.c
 * @brief This is file contains the implementation of functions to the menu
 */

//FUNCTIONS ----------------------------------------
void init_menuSprites(){
    //sprites initialization 

    mouse_sprite = create_sprite(get_mouse_xpm(),0,0,0,0); 
    start = create_sprite(get_menu_start_xpm(), 0,0,0,0); 
    name = create_sprite(get_menu_name_xpm(),0,0,0,0); 
    exit_ = create_sprite(get_menu_exit_xpm(),0,0,0,0); 
    inst = create_sprite(get_menu_inst_xpm(),0,0,0,0); 
    
    start_animated = create_sprite(get_menu_starts_xpm(), 0,0,0,0); 
    inst_animated = create_sprite(get_menu_insts_xpm(),0,0,0,0); 
    exit_animated = create_sprite(get_menu_exits_xpm(),0,0,0,0); 

    instruction = create_sprite(get_instruction_xpm(),0,0,0,0); 
}

void menuDraw(int *mouse_x, int* mouse_y){
    clear_buffer(); 
    restart_state();
    drawMenu(name,start,inst, exit_,start_animated, inst_animated, exit_animated, mouse_x, mouse_y); 
    animateMouse(mouse_sprite, mouse_x, mouse_y, &pp);      //draw the mouse
    memvideo_cpy();                                         //parses the information from the buffer to the video mem
    reset_packet();                                         //reset the mouse struct pp
}

int check_start(int mouse_x, int mouse_y){
    if (pp.lb && (mouse_x >=277 && mouse_x<=(277+470)) && (-mouse_y >=260 && -mouse_y<=(260+126)))
        return 1; 
    return 0; 
}

int check_exit(int mouse_x, int mouse_y){
    if (pp.lb && (mouse_x >=367 && mouse_x<=(367+291)) && (-mouse_y >=600 && -mouse_y<=(660+123)))
        return 1; 
    return 0; 
}

int check_instructions(int mouse_x, int mouse_y){
    if ( pp.lb && (mouse_x >=190 && mouse_x<=(190+645)) && (-mouse_y >=426 && -mouse_y<=(426+117))){
        clear_buffer();
        drawSprite(instruction, 0,0);
        memvideo_cpy();
        return 1; 
    }
    return 0; 
}
