/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits. 
 */
typedef struct {
  int x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  int xspeed, yspeed;  /**< current speeds in the x and y direction */
  uint8_t *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(xpm_map_t xpm, int x, int y,int xspeed, int yspeed);

/** Animate the sprite "fig" according to its attributes in memory,
 * whose address is "base".  The animation detects the screen borders
 * and change the speed according; it also detects collision with
 * other objects, including the player pad. Collisions with the screen
 * border generates a perfect reflection, while collision with other
 * objects generates a random perturbation in the object speed. Other
 * strategies can be devised: use quasi-elastic collision based on the
 * objects "mass" and speed, generate spin effect based on amount of
 * tangential speed direction relative to the object center of
 * "mass"...  Returns the kind of collision detected, RIGHT_HIT or
 * LEFT_HIT, if a collision with the players pad (WHITE colored!) is
 * detected.
 */
int animate_sprite(Sprite *fig, char *base);

/** The "fig" sprite is erased from memory whose address is "base"
 * and used resources released.
 */
void destroy_sprite(Sprite *sp);

void move_cursor(Sprite *fig, int xstep, int ystep, char *base);

Sprite* SpriteInit(xpm_map_t xpm,uint16_t *xi,uint16_t *xf,uint16_t *yi,uint16_t *yf,int16_t speed,uint16_t *required);
int animate(xpm_map_t xpm,Sprite *sp,uint16_t xf,uint16_t yf,uint16_t required,uint16_t *frame,vbe_mode_info_t *mode_info);
/**
 * @brief Draw the mouse on it's position
 * 
 * @param mouse_sprite Mouse sprite
 * @param mouse_x Position of the mouse on x-axis
 * @param mouse_y Position of the mouse on y-axis
 * @param pp Struct with mouse actual information
 */
void animateMouse(Sprite* mouse_sprite,int *mouse_x,int*mouse_y,struct packet *pp); 
/**
 * @brief Simply draw a sprite in the buffer
 * 
 * @param sprite Sprite to be drawn
 * @param x position of it on x-axis
 * @param y position of it on y-axis
 * @return int 0 in case of success, 1 otherwise 
 */
int drawSprite(Sprite * sprite, int x, int y); 
/**
 * @brief Draw the initial menu
 * 
 * @param name NAME of the project sprite
 * @param start START sprite
 * @param inst INSTRUCTION sprite
 * @param exit EXIT sprite
 * @param start_animated START sprite, but bigger
 * @param inst_animated INSTRUCTION sprite, but bigger
 * @param exit_animated EXIT sprite, but bigger
 * @param mouse_x Position of the mouse on x-axis
 * @param mouse_y Position of the mouse on y-axis
 */
void drawMenu(Sprite* name, Sprite* start, Sprite* inst, Sprite* exit, Sprite *start_animated, Sprite *inst_animated, Sprite*exit_animated, int *mouse_x, int *mouse_y); 
/**
 * @brief Draw in the buffer a mirror sprite, related to the y-axis 
 * 
 * @param sprite Sprite to be inversed
 * @param x Position on x-axis
 * @param y Position on y-axis
 */
void drawInvertedSprite(Sprite * sprite, int x, int y); 
/**
 * @brief Draw a rotate sprite using matrix of rotation
 * 
 * @param sprite Sprite to be rotated
 * @param x Position on the x-axis
 * @param y Position of y-axis
 * @return int 0 in case of success, 1 otherwise
 */
int drawSpriteRotated(Sprite * sprite, int x, int y);
/**
 * @brief Draw in the buffer a mirror sprite, that is also rotate using a matrix of rotation
 * 
 * @param sprite Sprite that will be inverted and rotated
 * @param x It's position on x-axis
 * @param y It's position on y-axis
 * @return int 0 in case of success, 1 otherwise
 */
int drawSpriteInvertedRotated(Sprite * sprite, int x, int y); 
/** @} end of sprite */

#endif
