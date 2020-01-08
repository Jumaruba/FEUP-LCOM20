#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <lcom/vbe.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "sprite.h"
#include "graphic.h"
#include "mouse.h"
#include "vbe.h"
#include "game.h"
#include "i8042.h"
#include "textInput.h"

extern int XBOUND;
extern int YBOUND;
extern uint8_t data;
extern uint8_t *video_mem;
extern uint8_t *buffer;
extern int contador;
extern uint8_t counter;
extern struct packet pp;
extern uint32_t data_mouse; 
extern uint16_t byteArray[3];
extern uint8_t code_bytes[2]; 
extern bool day; 
extern bool finish;


//shot
static bool shot = true;            /**<@brief if true, cannon left shot. Else cannon right shot*/
bool cannon_shot = false;           /**<@brief true if needs to draw a shot*/
bool doing_shot = false;            /**<@brief true if a cannon is performing a shot, false otherwise*/
bool first  = false;
bool draw = true;
//bool player1turn = true;            /**<@brief true if it's player turn*/
float angulo = -M_PI;               /**<@brief angle between mouse and cannon*/
int hp1 = 3;                        /**<@brief life of the player 1*/
int hp2 = 3;                        /**<@brief life of the player 2*/
extern float segundos;              /**<@brief seconds that have passed since the beggining of the shot*/
static int *Mouse_x;                /**<@brief position of the mouse in the x-axis*/
static int *Mouse_y;                /**<@brief position of the mouse in the y-axis*/
//sprites 
static Sprite * mouse_sprite;       /**<@brief mouse sprite*/
static Sprite * ground;             /**<@brief ground sprite*/
static Sprite * cannon;             /**<@brief left cannon sprite*/
static Sprite * cannon2;            /**<@brief right cannon sprite*/
static Sprite * ball;               /**<@brief ball sprite*/
static Sprite * sun;                /**<@brief sun sprite*/
static Sprite * moon;               /**<@brief moon sprite*/
static Sprite * heart;              /**<@brief heart sprite*/
static Sprite * player1wins;        /**<@brief sprite saying player 1 wins*/
static Sprite * player2wins;        /**<@brief sprite saying player 2 wins*/
static Sprite * endgame;            /**<@brief sprite saying it's end of the game*/
static Sprite * space;              /**<@brief sprite saying to press space*/
static Sprite * winner;             /**<@brief sprite saying winner*/ 
/**
 * @file game.c
 * @brief This is file contains the implementation of functions to the game interface
 */

int init_gameSprite(){
    mouse_sprite = create_sprite(get_mouse_xpm(),0,0,0,0); 
    ground = create_sprite(get_game_ground_xpm(), 0,0,0,0); 
    init_sun(); 
    init_moon(); 
    init_cannon(); 
    init_cannon2();
    init_heart();
    init_1wins();
    init_2wins();
    init_endgame();
    init_space();
    winner = create_sprite(get_winner_xpm(),350, 300,0,0); 
    return 0;
}

void draw_p1_hp(){
    if(hp1 == 3)
    {
        drawSprite(heart,0,30);
        drawSprite(heart,0,100);
        drawSprite(heart,0,170);
    }
    else if(hp1 == 2){
        drawSprite(heart,0,30);
        drawSprite(heart,0,100);
    }
    else if (hp1 == 1) drawSprite(heart,0,30);
    
}
void draw_p2_hp(){
    if(hp2 == 3)
    {
        drawSprite(heart,970,30);
        drawSprite(heart,970,100);
        drawSprite(heart,970,170);
    }
    else if(hp2 == 2){
        drawSprite(heart,970,30);
        drawSprite(heart,970,100);

    }
    else if(hp2 == 1){
        drawSprite(heart,970,30);
    }
}
int gameDraw(int *mouse_x, int *mouse_y){
    
    clear_buffer(); 
    
    if(check_for_engame()){
        memvideo_cpy();  
        reset_packet(); 
        return 0; 
    }
    else{
        
        Mouse_x = mouse_x; Mouse_y = mouse_y; 
        cannon1_moviment(); 
        cannon2_moviment(); 
        
        if (day) {
            blue_background();
            draw_interface(); 
            drawSprite(sun, sun->x, sun->y); 
    
        } 
        else{
            dark_blue_background();
            draw_interface(); 
            drawSprite(moon, moon->x, moon->y); 
            
        }
        if (doing_shot || cannon_shot){
                handle_shot();                  //draw the trajetory
                cannon_shot = false; 
            } 
        memvideo_cpy();
        reset_packet(); 
        return 0; 
    }
}

void cannon2_moviment(){
    int x = cannon2->x; 
    if (cannon2->xspeed != 0) x =  cannon2->x+cannon2->xspeed;  
    if (abs(x - cannon->x) > 300) cannon2->x = x;       //restrict the distance
}

void cannon1_moviment(){
    int x = cannon->x; 
    if (cannon->xspeed != 0) x = cannon->x+cannon->xspeed; 
    if (abs(x- cannon2->x) > 300) cannon-> x = x; 
}

void handleControls(){

    if(MAKE_L_LETTER == code_bytes[0]) cannon->xspeed = -SPEED;
    else if (MAKE_R_LETTER == code_bytes[0])cannon->xspeed = SPEED;
    else if (BK_L_LETTER == code_bytes[0] || BK_R_LETTER == code_bytes[0]) cannon->xspeed = 0;   
    else if (F_DIGIT == code_bytes[0] && MAKE_R_ARROW_2 == code_bytes[1]) cannon2->xspeed = SPEED; 
    else if (F_DIGIT == code_bytes[0] && MAKE_L_ARROW_2 == code_bytes[1]) cannon2->xspeed = -SPEED;
    else if (F_DIGIT == code_bytes[0] && (BK_L_ARROW_1 == code_bytes[1] || BK_R_ARROW_1 == code_bytes[1])) cannon2->xspeed = 0;
}


void draw_ground(){
    for (int i = 0;  i< 20; i++)
        drawSprite(ground, ground->width*i,704); 
}

void draw_interface(){
    draw_ground(); 
    draw_p1_hp();
    draw_p2_hp();
    if (!shot) {
        drawSpriteInvertedRotated(cannon2, cannon2->x, cannon2->y);
        drawSprite(cannon, cannon->x, cannon->y);           //if it's not the turn, then do not rotate
    }
    else {
        drawInvertedSprite(cannon2, cannon2->x, cannon2->y);  
         drawSpriteRotated(cannon, cannon->x, cannon->y);  //if cannon can shot, then it can rotate
    }
    animateMouse(mouse_sprite, Mouse_x, Mouse_y, &pp);
    
}


void init_player1_hp(){
    heart = create_sprite(get_heart_xpm(),0,0,0,0);
}

void handle_shot(){
    uint8_t h  = sqrt(pow(cannon->width,2)+pow(cannon->height,2)); 
    if (!first) {
        first = true; 
        return; 
    }
    if(doing_shot){ 
        cal_trajetory();
        drawSprite(ball, ball->x, ball->y); 
    }

    //the ball needs to consider the angle of the cannon
    else if(shot){                       //cannon left shot 

        ball = create_sprite(get_game_ball_xpm(),cannon->x + cos(-angulo)*h - 20, cannon->y - sin(-angulo)*h + 25,BALL_SPEED,0); 
        doing_shot = true; 
        draw = true;
        //player1turn = false;
    }   
    else if(!shot){                     //cannon right shot
        doing_shot = true; 
        draw = true;
        ball = create_sprite(get_game_ball_xpm(),cannon2->x + cos(-angulo)*h + 30 ,cannon2->y - sin(-angulo)*h + 30,BALL_SPEED,0); 
        //player1turn = true;
    }

}

void cal_trajetory(){
    ball->x += cos(-angulo)*ball->xspeed*segundos;
    ball->y += -(BALL_SPEED*sin(-angulo)*segundos - GRAVITY*segundos*segundos/2);
    
    if(checkCollision()){
        destroy_sprite(ball);
        doing_shot = false;         //terminou o tiro
        shot = !shot;               //alterna o canhão
        segundos = 0; 
        contador = 0; 
        get_angle(); 
        //player1turn = !player1turn;
        return;

    }

    //destroy the ball 
    if (ball->y > 704 || ball->x > 1024 || ball->x < 0 ){
        destroy_sprite(ball);       //destroy
        doing_shot = false;         //terminou o tiro
        shot = !shot;               //alterna o canhão
        segundos = 0; 
        contador = 0; 
        get_angle();                //we need to calculate the new angle for the cannon 
        //player1turn = !player1turn;
        return;
    }
}

void get_angle(){
    if(shot) angulo = atan2((double)-*Mouse_y -cannon->y,(double)*Mouse_x - cannon->x);
    else angulo = atan2((double)-*Mouse_y -cannon2->y,(double)*Mouse_x - cannon2->x);
}


bool checkCollision(){
    if(shot){
        if(ball->x >= cannon2->x && ball->x <= cannon2->x + cannon2->width){
            if(ball->y >= cannon2->y && ball->y <= cannon2->y + cannon2->height){
                draw = false;
                hp2--;
                return true;
            }
        }
    }
    else{
        if(ball->x <= cannon->x + cannon->width && ball->x >= cannon->x){
            if(ball->y >= cannon->y && ball->y <= cannon->y + cannon->height){
                draw = false;
                hp1--;
                return true;
            }
        }
    }
    return false;
}


bool check_for_engame(){
    if(hp1 == 0){
        if(code_bytes[0] == KBC_KC_SPACE){
            restart_state();
            finish = true; 
            return false;
        }
        memset(buffer,0,get_hres()*get_vres()*get_bytes());
        drawSprite(endgame, endgame->x,endgame->y); 
        drawSprite(winner, winner->x, winner->y);
        write_string(name2, 384-total2/3,450,size2);
        drawSprite(space, space->x, space->y+200);
        return true;
    }
    if(hp2 == 0){
        if(code_bytes[0] == KBC_KC_SPACE){
            restart_state();
            finish = true; 
            return false;
        }
        memset(buffer,0,get_hres()*get_vres()*get_bytes());
        drawSprite(endgame, endgame->x,endgame->y); 
        drawSprite(winner, winner->x, winner->y); 
        write_string(name1, 384-total2/3,450,size1);
        drawSprite(space, space->x, space->y+200);
        return true;
    }
    return false;
}

void restart_state(){
    hp1 = 3;
    hp2 = 3;
    shot = true;
}






//INIT SPRITES ------------------------------------------------------------------
void init_cannon(){
    cannon = create_sprite(get_game_cannon_xpm(),0,0,0,0);   
    cannon->xspeed = 0; 
    cannon->yspeed = 0;
    cannon->x = 100; 
    cannon->y = 656; 
}   

void init_cannon2(){
    cannon2 = create_sprite(get_game_cannon_xpm(),0,0,0,0);
    cannon2->xspeed = 0; 
    cannon2->yspeed = 0; 
    cannon2-> x = 900; 
    cannon2-> y = 656; 
}

void init_sun(){
    sun = create_sprite(get_game_sun_xpm(),0,0,0,0); 
    sun->x= 800; 
    sun->y= 0; 
}

void init_moon(){
    moon = create_sprite(get_game_moon_xpm(),0,0,0,0); 
    moon->x = 800; 
    moon->y= 50; 

}

void init_heart(){
    heart = create_sprite(get_heart_xpm(),0,0,0,0);
}

void init_1wins(){
    player1wins = create_sprite(get_wins1_xpm(),0,0,0,0);
    player1wins->x = 135;
    player1wins->y = 600;
}

void init_2wins(){
    player2wins = create_sprite(get_wins2_xpm(),0,0,0,0);
    player2wins->x = 135;
    player2wins->y = 600;

}

void init_endgame(){
    endgame = create_sprite(get_end_xpm(),0,0,0,0);
    endgame->x = 135;
    endgame->y = 100;
}

void init_space(){
    space = create_sprite(get_space_xpm(),0,0,0,0);
    space->x = 190;
    space->y = 426;
}
