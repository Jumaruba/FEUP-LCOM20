#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdbool.h>

#include <minix/driver.h>
#include <sys/mman.h>
#include <machine/int86.h>
#include <math.h>

typedef struct __attribute__((packed)){
    char VbeSignature[4]; 
    BCD VbeVersion[2]; 
    uint32_t OemStringPtr; 
    uint8_t Capabilities[4]; 
    uint32_t VideoModePtr; 
    uint16_t TotalMemory; 
    uint16_t OemSoftwareRev; 
    uint32_t OemVendorNamePtr; 
    uint32_t OemProductNamePtr; 
    uint32_t OemProductRevPtr; 
    uint8_t Reserved[222]; 
    uint8_t OemData[256]; 

}VbeInfoBlock; 

int init_mode(uint16_t mode);                               //function to the first test, don't map and don't check mode 
int vbeModeInfo(uint16_t mode, vbe_mode_info_t *mode_info);  
int drawPixel(uint16_t x_pos, uint16_t y_pos, uint32_t color);
uint16_t get_hres();
uint16_t get_vres(); 
uint8_t get_bits_per_pixel();
uint16_t get_GreenScreenMask(); 
uint16_t get_BlueScreenMask(); 
uint16_t get_RedScreenMask(); 
uint8_t get_bytes(); 
void getPermission();
void drawXpm(xpm_map_t xpm, uint16_t x, uint16_t y);
void eraseXpm(xpm_map_t xpm,uint16_t x, uint16_t y);
int get_controller(); 
int getControllerInfo(VbeInfoBlock  *controller_info, uint32_t* base); 
int16_t absO(int16_t x);
int drawPattern(uint16_t mode,uint8_t rect_number,uint32_t first,uint8_t step,vbe_mode_info_t *mode_info);
#endif /* __GRAPHIC_H */
