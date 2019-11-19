
#include "graphic.h"
#include "vbe.h"
#include "lcom/vbe.h"
#include "vbe.h"

//Uses the VBE INT 0x10 interface to set the desired graphics mode using a linear frame buffer, 
//maps VRAM to the process' address space
//initializes static global variables with the resolution of the screen, and the color depth (i.e the no. of bits per pixel). 
//Initially, you should call function vbe_get_mode_info() provided by the LCF, to get this information. Later, you can implement 
//your own version of this function.


static uint16_t hres; 
static uint16_t vres; 
static uint8_t bits_per_pixel; 
static uint8_t *video_mem; 
static uint8_t bytes; 
static uint8_t RedMaskSize; 
static uint8_t GreenMaskSize; 
static uint8_t BlueMaskSize; 
extern int XBOUND;
extern int YBOUND;
void getPermission() {

    int r;
    struct minix_mem_range mr; 
    mr.mr_base = (phys_bytes) 0x0;         //getting the base address
    mr.mr_limit = mr.mr_base + 0xFFFFF;    //getting the 1MB (1024*1024)

    //getting permission
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    return;
}

//codigo para a primeira funcao
int init_mode(uint16_t mode){   
    
    reg86_t reg; 
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

void *(vg_init)(uint16_t mode){

    getPermission();

    vbe_mode_info_t mode_info;
    if(vbeModeInfo(mode, &mode_info)) return NULL;                  //getting mode

    hres = mode_info.XResolution;                                   //setting static variables
    vres = mode_info.YResolution; 
    bits_per_pixel = mode_info.BitsPerPixel; 
    GreenMaskSize = mode_info.GreenMaskSize;                      //get mask size 
    BlueMaskSize = mode_info.BlueMaskSize; 
    RedMaskSize = mode_info.RedMaskSize; 
    bytes = ceil(bits_per_pixel/8); 

    int r = 0;
    struct minix_mem_range mr;
    unsigned int vram_size = hres*vres* ceil(bits_per_pixel/8.0);   //size of the ram
    mr.mr_base = (phys_bytes)  mode_info.PhysBasePtr;               //getting the base address
    mr.mr_limit = mr.mr_base + vram_size;                           //getting the limit
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))       //getting permission
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);  //mapping memory to get the first MB
    if(video_mem == MAP_FAILED) panic("couldn't map video memory"); //case of error

    if(init_mode(mode))
        return NULL;

    return video_mem; 
}

//obtain physical memory address 
int vbeModeInfo(uint16_t mode, vbe_mode_info_t *mode_info){
  
    reg86_t reg; 
    mmap_t info_map;
    //use liblm.a to initialize 
    //initialize low memory    
    
    //if(lm_init(true) == NULL) return 1;
    if(lm_alloc(sizeof(vbe_mode_info_t), &info_map) == NULL ) return 1; 

    phys_bytes buf; 
    buf = info_map.phys;                                            //buffer in that region of the physical address space.

    //es and ds describes general characteristics 
    // clear unused register to avoid errors 
    memset(&reg, 0, sizeof(reg));
    reg.ax = VBE_FUNCTION | VBE_MODE_INFO;                          //function call | mode info
    reg.es =  PB2BASE(buf);
    reg.di = PB2OFF(buf);
    reg.cx = mode; 
    reg.intno = INT;
    if( sys_int86(&reg) != OK )return 1;                            //sending information to the bios 
    
    //como passar a informação para mode_info 
    *mode_info = *((vbe_mode_info_t *)(info_map.virt)); 
    if (!lm_free(&info_map)) return 1;                              //freeing space
    return 0; 

}

int (vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color){
    while(height > 0){
        vg_draw_hline(x, y, width, color); 
        height--; 
        y++; 
    }
    return 0; 
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len ,uint32_t color ){
    while(len>0){
        drawPixel(x, y, color); 
        len--;
        x++; 
    }
    return 0; 
} 
int drawPixel(uint16_t x, uint16_t y, uint32_t color){
    if(x >= XBOUND || y >= YBOUND) return 0;    
    memcpy(&video_mem[((y*hres)+x)*bytes], &color, bytes); 
    return 0; 
}

uint16_t get_hres(){
    return hres; 
}
uint16_t get_vres(){
    return vres; 
}
uint16_t get_RedScreenMask(){
    return RedMaskSize;
}
uint16_t get_BlueScreenMask(){
    return BlueMaskSize; 
}
uint16_t get_GreenScreenMask(){
    return GreenMaskSize; 
}

uint8_t get_bits_per_pixel(){   
    return bits_per_pixel;
}

uint8_t get_bytes(){
    return bytes; 
}
void drawXpm(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t img;
    // get the pix map from the XPM
    uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &img);

    // copy it to graphics memory
    for (int linha = 0; linha < img.height; linha++){
        for (int coluna = 0; coluna < img.width; coluna++){
        drawPixel(x+coluna, y+linha, sprite[coluna + linha*img.width]);  
        }
    }
}

uint32_t createMask(int size){ 
    //uint32_t num = 0xFFFFFFFF;
    return 0xFF;
}
void eraseXpm(xpm_map_t xpm,uint16_t x, uint16_t y){
    xpm_image_t img;
    uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &img);
    sprite = NULL;
    for (int linha = 0; linha < img.height; linha++){
        for (int coluna = 0; coluna < img.width; coluna++){
        drawPixel(x+coluna, y+linha, 0);  
        }
    } 
}
int32_t applyMask(uint32_t color,int size){
    int fixed = 32 - size;
    color<<=fixed;
    color>>=fixed;
    return color;
}
int get_controller(){
    
    //VbeInfoBlock  controller_info; 
    uint32_t base; 
    vg_vbe_contr_info_t ctrl; 
    //if(getControllerInfo(&controller_info, base)) return 1;

    reg86_t reg; 
    mmap_t info; 

    lm_alloc(sizeof(VbeInfoBlock), &info); 
    
    VbeInfoBlock *aux = info.virt;
    aux->VbeSignature[0]= 'V';
    aux->VbeSignature[1]= 'B';
    aux->VbeSignature[2]= 'E';
    aux->VbeSignature[3]= '2';
    
    memset(&reg, 0, sizeof(reg));                                   //cleaning memset
    reg.ax = VBE_FUNCTION | CTRL_INFO;                              //function call | mode info
    reg.es =  PB2BASE(info.phys);
    reg.di = PB2OFF(info.phys);
    reg.intno = INT;
    if( sys_int86(&reg) != OK )return 1;                            //sending information to the bios 

    //memcpy(&controller_info, info.virt, info.size); 
    VbeInfoBlock *controller_info = info.virt; 
     base = ((uint32_t)info.virt - (uint32_t)info.phys);  

    memcpy(ctrl.VBESignature, controller_info->VbeSignature, 4); 
    printf("base %x\n", base); 

    ctrl.VBEVersion[0] = controller_info->VbeVersion[0]; 
    ctrl.VBEVersion[1] = controller_info->VbeVersion[1]; 


    //quando printa qualquer um destes da erro
    ctrl.OEMString = (char *)(((controller_info->OemStringPtr >> 16) <<4) +
     (controller_info->OemStringPtr & OFFSET) + base);
    //ctrl.OEMString = CONVERT(controller_info->OemStringPtr, base); 

    ctrl.VideoModeList = (uint16_t *)(((controller_info->VideoModePtr >> 16) << 4) +
     (controller_info->VideoModePtr & OFFSET) + base);
    //ctrl.VideoModeList = CONVERT(controller_info->VideoModePtr, base); 

    ctrl.TotalMemory = (uint32_t)(controller_info->TotalMemory * 64);   

    ctrl.OEMVendorNamePtr =(char*)(((controller_info->OemVendorNamePtr >> 16) << 4)+ 
    (controller_info->OemVendorNamePtr & OFFSET) + base) ;
    //ctrl.OEMVendorNamePtr = CONVERT(controller_info->OemVendorNamePtr, base); 
    ctrl.OEMProductNamePtr = (char*)(((controller_info->OemProductNamePtr >> 16) << 4) + 
    (controller_info->OemProductNamePtr & OFFSET) + base);
    //ctrl.OEMProductNamePtr = CONVERT(controller_info->OemProductNamePtr, base); 
    ctrl.OEMProductRevPtr = (char *)(((controller_info->OemProductRevPtr >> 16) << 4)+
     (controller_info->OemProductRevPtr & OFFSET) + base);
    //ctrl.OEMProductRevPtr = CONVERT(controller_info->OemProductRevPtr, base); 

    if (!lm_free(&info)) return 1;
    vg_display_vbe_contr_info(&ctrl); 
    

    return 0; 
}

int16_t absO(int16_t x){
    return x < 0 ? -x : x;
} 

int drawPattern(uint16_t mode,uint8_t rect_number,uint32_t first,uint8_t step,vbe_mode_info_t *mode_info){
    if(mode_info == NULL) return 1;
    uint16_t x,y = 0;
    uint32_t color = 0;
    uint16_t width = mode_info->XResolution / rect_number;
    uint16_t height = mode_info->YResolution / rect_number;
    uint32_t R,G,B,red,green,blue;
    uint8_t row = 0, col = 0;
    //printf("sa");
    if(mode == 0x105){
        for(uint8_t i = 0;i<rect_number;i++){
            x = 0;
            col = 0;
            for(uint8_t j = 0;j<rect_number;j++){
                //formulae given in documentation
                color =(first + (i * rect_number + col) * step) % (1 << get_bits_per_pixel());
                vg_draw_rectangle(x,y,width,height,color);
                x+=width;
                col++;
            }
            y+=height;
        }
    }
    else{
        red = applyMask(first>>mode_info->RedFieldPosition,mode_info->RedMaskSize);
        green = applyMask(first>>mode_info->GreenFieldPosition,mode_info->GreenMaskSize);
        blue = applyMask(first>>mode_info->BlueFieldPosition,mode_info->BlueMaskSize);
        for(uint8_t i = 0;i<rect_number;i++){
            x = 0;
            col = 0;
            for(uint8_t j = 0;j<rect_number;j++){
                //formula given in documentation
                R = (red + row*step)%(1<<mode_info->RedMaskSize);
                G = (green + col*step)%(1<<mode_info->GreenMaskSize);
                B = (blue + (col+row)*step)%(1<<mode_info->BlueMaskSize);
                color = (R<<mode_info->RedFieldPosition) | (G<<mode_info->GreenFieldPosition) | (B<<mode_info->BlueFieldPosition);
                vg_draw_rectangle(x,y,width,height,color);
                x+=width;
                col++;
            }
            y+=height;
            row++;
        }

    }
  return 0;
}
