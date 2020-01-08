#ifndef _GAME_H_
#define _GAME_H_

/**
 * @file game.h
 * @defgroup game game
 * @brief This is file contains the definition of functions to the game interface
 */

/**
 * @brief Initializes the sprites
 * 
 */
int init_gameSprite();
/**
 * @brief Draw the game and parses the information to the mem_video
 * 
 * @param mouse_x Actual location of the mouse on x-axis
 * @param mouse_y Actual location of the mouse on y-axis
 * @return int 0 upon end
 */
int gameDraw(int *mouse_x, int *mouse_y); 
/**
 * @brief Handle the movement of the cannons setting the xspeed variable and yspeed for each cannon
 * 
 */
void handleControls(); 
/**
 * @brief Draws the ground
 * 
 */
void draw_ground();
/**
 * @brief Updates the position of the canonon 1, but doesn't draw
 * 
 */
void cannon1_moviment(); 
/**
 * @brief Updates the position of the cannon 2, but doesn't draw
 * 
 */
void cannon2_moviment(); 
/**
 * @brief Draw the mouse, cannons and the hearts in the buffer
 * 
 */
void draw_interface(); 
/**
 * @brief Draw the hp for player 1
 * 
 */
void draw_player1_hp();
/**
 * @brief Draw the hp for player 2
 * 
 */
void draw_p2_hp();
/**
 * @brief Check if there is a collision in both cannons. If there is, update the hp
 * 
 * @return true in case of collision
 * @return false case there isn't a collision
 */
bool checkCollision();
/**
 * @brief Checks the hp of each cannon. If a hp is zero, than the game is over
 * 
 * @return true Game is over
 * @return false Game is not over
 */
bool check_for_engame();
/**
 * @brief Draw the cannon shot and handles the turn
 * 
 */
void handle_shot();  
/**
 * @brief Calculates the trajectory of the shot
 * 
 */
void cal_trajetory(); 
/**
 * @brief Get the angle between the mouse and cannon of the turn 
 * 
 */
void get_angle();

/**
 * @brief Restart game states for new match
 * 
 */
void restart_state();

//SPRITES INITIALIZATION
void init_cannon();             /**<@brief initializes cannon left sprite*/
void init_cannon2();            /**<@brief initializes cannon right sprite*/
void init_sun();                /**<@brief initializes the sun sprite*/
void init_moon();               /**<@brief initializes the moon sprite*/
void init_heart();              /**<@brief initializes the hearts sprites to the hp*/
void init_1wins();              /**<@brief initializes sprite saying that the player 1 has won*/
void init_2wins();              /**<@brief initializes sprite saying that the player 2 has won*/
void init_endgame();            /**<@brief initializes sprite saying that the game is over*/
void init_space();              /**<@brief initializes sprite saying to press space*/

//MACROS
#define SPEED               5   /**<@brief this macro defines the speed of the cannons*/
#define BALL_SPEED          35  /**<@brief this macro defines the speed of the ball in the x-axis*/
#define GRAVITY             70  /**<@brief this macro defines the gravity*/

#define F_DIGIT             0xe0
#define ARROW               0x2a/**<@brief first byte for the arrows make code*/
#define MAKE_R_ARROW_2      0x4d/**<@brief second byte for the right arrow make code*/
#define MAKE_L_ARROW_2      0x4b/**<@brief second byte for the left arrow make code*/
#define SHOT                0x39/**<@brief this macro is the code for the mouse left buttom*/

#define BK_L_ARROW_1        0xcb/**<@brief first byte of the brake code for teh left arrow*/
#define BK_R_ARROW_1        0xcd/**<@brief first byte of the brake code for the right arrow*/

#define MAKE_L_LETTER       0x1e/**<@brief make code for the letter a*/
#define BK_L_LETTER         0x9e/**<@brief brake code for the letter a*/
#define MAKE_R_LETTER       0x20/**<@brief make code for the letter d*/
#define BK_R_LETTER         0xa0/**<@brief break code for the letter d*/

#endif
