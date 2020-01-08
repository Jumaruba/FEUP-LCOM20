#ifndef _TEXTINPUT_H_
#define _TEXTINPUT_H_

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <stdint.h>
#include "graphic.h"
#include "sprite.h"
#include "i8042.h"


extern char name1[20]; 
extern char name2[20]; 
extern uint8_t code_bytes[2]; 
extern char letter; 
extern int size1; 
extern int size2; 
extern uint8_t data; 
extern int total1; 
extern int total2; 

/**
 * @file textInput.h
 * @defgroup textInput textInput
 * @brief This file contains the fuctions textInput.h
 */

/**
 * @brief This functions writes and asks the name of the player 2 
 * 
 */
void askname1();                                        
/**
 * @brief This functions writes and asks the name of the player 1
 * 
 */
void askname2();                                       
/**
 * @brief This function write a string
 * 
 * @param s String to be written
 * @param x Place in the x-axis to write it
 * @param y Place in the y-axis to write it
 * @param size Size of the char 
 * @return size in pixel of the string
 */
void write_string(char* s, int x, int y, int size);    
/**
 * @brief Get the letter from the keyboard
 * 
 * @return char* Letter returned
 */
char get_letter(); 

//LETTERS 

#define A                   0x1e
#define A_b                 0x9e
#define B                   0x30
#define B_b                 0xb0
#define C                   0x2e
#define C_b                 0xae
#define D                   0x20 
#define D_b                 0xa0
#define E                   0x12 
#define E_b                 0x92
#define F                   0x21
#define F_b                 0xa1
#define G                   0x22
#define G_b                 0xa2
#define H                   0x23
#define H_b                 0xa3
#define I                   0x17 
#define I_b                 0x97   
#define J                   0x24 
#define J_b                 0xa4
#define K                   0X25 
#define K_b                 0xa5
#define L                   0x26
#define L_b                 0xa6
#define M                   0x32
#define M_b                 0xb2
#define N                   0x31
#define N_b                 0xb1
#define O                   0x18
#define O_b                 0x98
#define P                   0x19
#define P_b                 0x99
#define Q                   0x10
#define Q_b                 0x90
#define R                   0x13
#define R_b                 0xb0
#define S                   0x1f
#define S_b                 0x9f
#define T                   0x14 
#define T_b                 0x94
#define U                   0x16
#define U_b                 0x96
#define V                   0x2f
#define V_b                 0xaf
#define W                   0x11
#define W_b                 0x91
#define X                   0x2d
#define X_b                 0xad
#define Y                   0x15
#define Y_b                 0x95
#define Z                   0x2c 
#define Z_b                 0xac
#define KBC_ENTER           0x1c
#define KBC_KC_SPACE_B      0xb9
#define BCK_SPACE           0x0e

#endif 
