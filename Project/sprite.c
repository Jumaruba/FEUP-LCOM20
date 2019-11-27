#include <stdarg.h> // va_* macros are defined here
#include <stdint.h>
#include <stdio.h>
#include <minix/driver.h>
#include <lcom/lcf.h>
#include <sys/mman.h>
#include <stdio.h>
#include "sprite.h"
#include "graphic.h"
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
    sp = NULL; // XXX: pointer is passed by value
    // should do this @ the caller
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
    //if((sp->x + speedX + sp->width < xBound) && (sp->y + speedY + sp->height < yBound))
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
