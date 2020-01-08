#include <stdarg.h> // va_* macros are defined here
#include <stdint.h>
#include <stdio.h>
#include <minix/driver.h>
#include <lcom/lcf.h>
#include <sys/mman.h>
#include <stdio.h>
#include "sprite.h"
#include "graphic.h"
extern float angulo;
/** Creates a new sprite with pixmap "pic", with specified
* position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(xpm_map_t xpm, int x, int y,int xspeed, int yspeed) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    if( sp == NULL )
    return NULL;

    // read the sprite pixmap
    xpm_image_t img;
    sp->map = xpm_load(xpm, XPM_INDEXED, &img); 
    
    sp->x = x; 
    sp->y = y; 
    sp->xspeed= xspeed; 
    sp->yspeed= yspeed;

    sp->width = img.width;
    sp->height= img.height;

     

    if( sp->map == NULL ) {
    free(sp);
    return NULL;
    }


    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL )
    return;
    
    if( sp ->map )
        free(sp->map);
    
    free(sp);
    sp = NULL;
}


Sprite* SpriteInit(xpm_map_t xpm,uint16_t *xi,uint16_t *xf,uint16_t *yi,uint16_t *yf,int16_t speed,uint16_t *required){
    Sprite * res;
    int movementX;
    int movementY;
    if((*xi == *xf) && *yi == *yf){
        res = create_sprite(xpm,*xi,*yi,0,0);
        return res;
    }
    if (speed > 0){
        if (*yi == *yf){
            movementX = absO(speed)/(abs(*xf -*xi)/(*xf-*xi));
            movementY = 0;   
        }
        else{
            movementY = absO(speed)/(abs((int)*yf -*yi)/(*yf-*yi)); 
            movementX = 0;
        }
        *required = 0;
    }         
    else{ 
        if (*yi == *yf){
            movementX = (abs(*xf -*xi)/(*xf-*xi));
            movementY = 0;   
        }
        else{
            movementY = (abs(*yf -*yi)/(*yf-*yi));
            movementX = 0;
        }
        *required = absO(speed);
    }
    res = create_sprite(xpm,*xi,*yi,movementX,movementY);
    return res;
}

int animate(xpm_map_t xpm,Sprite *sp,uint16_t xf,uint16_t yf,uint16_t required,uint16_t *frame,vbe_mode_info_t *mode_info){
    int speedX = sp->xspeed;
    int speedY = sp->yspeed;
    if(speedX < 0 || speedY < 0){
        if(*frame < required) return 1;
    }

        sp->x+=speedX;
        sp->y+=speedY;
        if(speedX > 0 && (sp->x >= xf)){
            sp->x = xf;
            sp->y = yf;
        }
        else if(speedY > 0 && (sp->y >= yf )){
            sp->x = xf;
            sp->y = yf;
        }
        else if(speedX < 0 && (sp->x <= xf)) {
            sp->x = xf;
            sp->y = yf;
        }
        else if(speedY < 0 && (sp->y <= yf)) {
            sp->x = xf;
            sp->y = yf;
        }
        drawXpm(xpm,sp->x,sp->y);
    return 0;
}

int drawSprite(Sprite * sprite, int x, int y){
    int height = sprite->height; 
    int width = sprite->width; 
    for (int linha = 0; linha < height; linha++){
        for (int coluna = 0; coluna < width; coluna++){
            uint8_t color = *((uint32_t*)(sprite->map+coluna+linha*width)); 
            drawPixel(x+coluna, y+linha, color);  
        }
    }
    return 0; 
}

void animateMouse(Sprite* mouse_sprite,int *mouse_x,int*mouse_y,struct packet *pp){
    if (pp->delta_y != 0 && pp->delta_x != 0){
        if (*mouse_x+pp->delta_x >= 0 && (*mouse_x+pp->delta_x+13) <= 1024)
            *mouse_x += pp->delta_x;
        if ((*mouse_y+pp->delta_y-13) >= -768 && *mouse_y+pp->delta_y <= 0) 
            *mouse_y += pp->delta_y; 
    }
    drawSprite(mouse_sprite,*mouse_x, -*mouse_y);
    
}

void drawMenu(Sprite* name, Sprite* start, Sprite* inst, Sprite* exit, Sprite *start_animated, Sprite *inst_animated, Sprite*exit_animated, int *mouse_x, int *mouse_y) {

    drawSprite(name, 141, 100);    //130 de altura
    if((*mouse_x >=277 && *mouse_x<=(277+470)) && (-*mouse_y >=260 && -*mouse_y<=(260+126)))
        drawSprite(start_animated, 277,240); 
    else 
     drawSprite(start,277, 260);    //130+100+30 = 260, 126 de altura
    if ((*mouse_x >=367 && *mouse_x<=(367+291)) && (-*mouse_y >=600 && -*mouse_y<=(660+123)))
        drawSprite(exit_animated, 367,600); 
    else 
        drawSprite(exit,367,600);      //426+177 = 600
    if ((*mouse_x >=190 && *mouse_x<=(190+645)) && (-*mouse_y >=426 && -*mouse_y<=(426+117)))
        drawSprite(inst_animated,190,426);
    else
        drawSprite(inst,190,426);      //260+126+30 = 426, 117 de altura

}

void drawInvertedSprite( Sprite * sprite, int x, int y){
    int height = sprite->height; 
    int width = sprite->width; 
    float angulo_ = M_PI*1.1; 
    for (int linha = 0; linha < height; linha++){
        for (int coluna = 0; coluna < width; coluna++){
            uint8_t color = *((uint32_t*)(sprite->map+coluna+linha*width));
            int k = -(float)height/2.0  + coluna;
            int z = -(float)width/2.0 + linha;
            int new_x = k*cos(angulo_ - M_PI/12)+z*sin(angulo_ - M_PI/12); 
            int new_y = -(-k*sin(angulo_ - M_PI/12)+z*cos(angulo_ - M_PI/12)); 

            drawPixel(new_x + x + width/2, new_y + y +height/2, color); 
        }
    }
}

int drawSpriteRotated(Sprite * sprite, int x, int y){
    int height = sprite->height; 
    int width = sprite->width; 

    
    for (int linha = 0; linha < height; linha++){
        for (int coluna = 0; coluna < width; coluna++){
            uint8_t color = *((uint32_t*)(sprite->map+coluna+linha*width));
            int k = -(float)height/2.0 + coluna;
            int z = -(float)width/2.0 + linha;
            int new_x = k*cos(-angulo - M_PI/12)+z*sin(-angulo - M_PI/12); 
            int new_y = -k*sin(-angulo - M_PI/12)+z*cos(-angulo - M_PI/12); 

            drawPixel(new_x + x + width/2, new_y + y + height/2, color);  
        }
    }
    return 0; 
}


int drawSpriteInvertedRotated(Sprite * sprite, int x, int y){
    int height = sprite->height; 
    int width = sprite->width; 

    
    for (int linha = 0; linha < height; linha++){
        for (int coluna = 0; coluna < width; coluna++){
            uint8_t color = *((uint32_t*)(sprite->map+coluna+linha*width));
            int k = -(float)height/2.0  + coluna;
            int z = -(float)width/2.0 + linha;
            int new_x = k*cos(angulo - M_PI/12)+z*sin(angulo - M_PI/12); 
            int new_y = -(-k*sin(angulo - M_PI/12)+z*cos(angulo - M_PI/12)); 

            drawPixel(new_x + x + width/2, new_y + y +height/2, color);  
        }
    }
    return 0; 
}
