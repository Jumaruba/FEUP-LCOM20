#ifndef _H_PROJ_
#define _H_PROJ_

#include "rtc.h"

/**
 * @file proj.h
 * @defgroup proj proj
 * @brief This is file contains the definition of project main function
 */

/**
 * @brief enum defining states of the proj
 */
typedef enum{
  MENU,
  GAME,
  NAME1, 
  NAME2, 
  INST,
  PAUSE
}window; 

/**
 * @brief Clear memory when exit
 * 
 * @return int 0 upon success, 1 otherwise 
 */
int exit_(); 
/**
 * @brief Checks if it's day or not 
 * 
 */
void isDay();


#endif
