
#include "graphic.h"
#include "vbe.h"
#include "lcom/vbe.h"
#include "vbe.h"
#include "keyboard.h"
#include "i8042.h"
#include "xpm.h"

static uint16_t hres; 
static uint16_t vres; 
static uint8_t bits_per_pixel; 
 uint8_t *video_mem; 
 uint8_t *buffer; 
static uint8_t bytes; 
static uint8_t RedMaskSize; 
static uint8_t GreenMaskSize; 
static uint8_t BlueMaskSize; 
extern int XBOUND;
extern int YBOUND;
extern uint8_t data;

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
    if (color == 5) return 0;     
    memcpy(&buffer[((y*hres)+x)*bytes], &color, bytes);
    return 0; 
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

int16_t absO(int16_t x){
    return x < 0 ? -x : x;
} 
/**
void printEndGame(int player){
    drawXpm(ENDGAME, 150, 100); 
    // se player == 1 entao o jogador 1 venceu 
    if (player == 1)
        drawXpm(player1wins, 130,300);
    // se nao, jogador 2 venceu 
    else
        drawXpm(player2wins, 130,300);
    drawXpm(pressSpace, 150,500); 
}
**/

//GET XPMS --------------------------------------------------------------------------------------------

xpm_map_t get_mouse_xpm(){
    return mouse; 
}
xpm_map_t get_menu_name_xpm(){
    return NAME; 
}
xpm_map_t get_menu_start_xpm(){
    return START; 
}
xpm_map_t get_menu_inst_xpm(){
    return INST; 
}
xpm_map_t get_menu_exit_xpm(){
    return EXIT; 
}

xpm_map_t get_menu_starts_xpm(){
    return STARTs; 
}

xpm_map_t get_menu_exits_xpm(){
    return EXITS; 
}

xpm_map_t get_menu_insts_xpm(){
    return INSTS; 
}

xpm_map_t get_game_ground_xpm(){
    return ground;
}
xpm_map_t get_game_cannon_xpm(){
    return cannon;
}

xpm_map_t get_game_ball_xpm(){
    return ball; 
}

xpm_map_t get_game_sun_xpm(){
    return sun;
}

xpm_map_t get_game_moon_xpm(){
    return moon; 
}

xpm_map_t get_heart_xpm(){
    return heart;
}

xpm_map_t get_wins1_xpm(){
    return player1wins;
}

xpm_map_t get_wins2_xpm(){
    return player2wins;
}

xpm_map_t get_end_xpm(){
    return ENDGAME;
}

xpm_map_t get_instruction_xpm(){
    return INSTRUCTIONS; 
}
xpm_map_t get_space_xpm(){
    return pressSpace;
}

xpm_map_t get_dash_xpm(){
    return dash;
}

xpm_map_t get_write_xpm(){
    return WRITE;
}

xpm_map_t get_name_xpm(){
    return NAMEE;
}

xpm_map_t get_ingreis_xpm(){
    return ingreis;
}

xpm_map_t get_player1_xpm(){
    return player1;
}

xpm_map_t get_player2_xpm(){
    return player2;
}

xpm_map_t get_alphabet_xpm(char letter){
    switch (letter){
        case 'a': return a; 
        case 'b': return b; 
        case 'c': return c; 
        case 'd': return d; 
        case 'e': return e; 
        case 'f': return f; 
        case 'g': return g; 
        case 'h': return h; 
        case 'i': return i; 
        case 'j': return j; 
        case 'k': return k;
        case 'l': return l; 
        case 'm': return m; 
        case 'n': return n; 
        case 'o': return o; 
        case 'p': return p; 
        case 'q': return q; 
        case 'r': return r; 
        case 's': return s; 
        case 't': return t; 
        case 'u': return u;
        case 'v': return v; 
        case 'w': return w; 
        case 'x': return x; 
        case 'y': return y; 
        case 'z': return z; 
        default: return space;  
    }
}

xpm_map_t get_winner_xpm(){
    return WINNER; 
}
//BUFFER ----------------------------------------------------------------------------------------------
void memvideo_cpy(){
    memcpy(video_mem, buffer, hres*vres*bytes); 
}

void malloc_buffer(){
    buffer = (uint8_t *)malloc(vres*hres*bytes); 
}

void free_buffer(){
    free(buffer); 
}
void clear_buffer(){
    memset(buffer, 0, hres*vres*bytes); 
}

void blue_background(){
    memset(buffer, 3, hres*vres*bytes); 
}

void dark_blue_background(){
    memset(buffer, 17, hres*vres*bytes); 
}
//GET -------------------------------------------------------------------------------------------------
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
