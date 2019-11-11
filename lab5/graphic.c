#include <lcom/lcf.h>
#include <stdio.h>
#include <stdbool.h>

#include <minix/driver.h>
#include <sys/mman.h>
#include <machine/int86.h>
#include <math.h> 
#include "graphic.h"
#include "vbe.h"


//Uses the VBE INT 0x10 interface to set the desired graphics mode using a linear frame buffer, 
//maps VRAM to the process' address space
//initializes static global variables with the resolution of the screen, and the color depth (i.e the no. of bits per pixel). 
//Initially, you should call function vbe_get_mode_info() provided by the LCF, to get this information. Later, you can implement 
//your own version of this function.


static uint16_t hres; 
static uint16_t vres; 
static uint8_t bits_per_pixel; 

int init_mode(uint16_t mode){  
    reg86_t reg;
    int r;
    struct minix_mem_range mr;

    
    mr.mr_base = (phys_bytes) 0x00;         //getting the base address
    mr.mr_limit = mr.mr_base + 0x100000;    //getting the 1MB (1024*1024)

    //getting permission
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
 
    memset(&reg, 0, sizeof(reg));               // clear unused register to avoid errors 
    reg.ax = VBE_FUNCTION | VBE_SET_MODE;       // VBE call, function 02 -- set VBE mode
    reg.bx = BIT(14) | mode;                    // set bit 14: linear framebuffer
    reg.intno = INT;                   
    
    //sending info to the bios
    if( sys_int86(&reg) != OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0; 
}

//obtain physical memory address 
int vbeModeInfo(uint16_t mode, vbe_mode_info_t *mode_info){
    mmap_t info_map;
    phys_bytes buf;
    reg86_t reg; 

    //use liblm.a to initialize 
    //initialize low memory 
    if(lm_init(false) == NULL) return 1; 
    if(lm_alloc(sizeof(vbe_mode_info_t), &info_map) == NULL ) return 1; 

    memset(&reg, 0, sizeof(reg));               // clear unused register to avoid errors 
    reg.ax = VBE_FUNCTION | VBE_MODE_INFO;      //function call | mode info
    
    buf = info_map.phys;                        //buffer in that region of the physical address space.

    //es and ds describes general characteristics 
    reg.es =  PB2BASE(buf);
    reg.ds = PB2OFF(buf);
    reg.cx = mode; 
    reg.intno = INT;
    if( sys_int86(&reg) != OK )return 1;        //sending information to the bios 

    if (!lm_free(&info_map)) return 1;          //freeing space
    return 0; 

}


uint16_t get_hres(){
    return hres; 
}

uint16_t get_vres(){
    return vres; 
}

uint8_t get_bits_per_pixel(){
    return bits_per_pixel;
}
