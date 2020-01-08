#include "textInput.h"


void askname1(){
    clear_buffer();
    drawXpm(get_write_xpm(),300,15);
    drawXpm(get_player2_xpm(),100,190);
    drawXpm(get_ingreis_xpm(),700,190);
    drawXpm(get_name_xpm(),300,350);
    if (letter == 'D') {
        size1 --; //delete  
        total1-=50;
    }
    else if (letter != 'A'){
        name1[size1] = letter; 
        total1+=50; 
        size1 ++;
    }
    write_string(name1, 384-total1/3 ,650,size1);
    memvideo_cpy(); 
}

void askname2(){
    clear_buffer();
    drawXpm(get_write_xpm(),300,15);
    drawXpm(get_player1_xpm(),100,190);
    drawXpm(get_ingreis_xpm(),700,190);
    drawXpm(get_name_xpm(),300,350);
    if (letter == 'D') {
        size2 --; //delete  
        total2 -= 50; 
    }
    else if (letter != 'A'){
        name2[size2] = letter; 
        total2+= 50;
        size2 ++;
    }
    
    write_string(name2, 384-total2/3,650,size2);
    memvideo_cpy(); ; 
}

void write_string(char* s, int x, int y, int size){
    for (int i = 0; i < size; i++) {
        drawXpm(get_alphabet_xpm(s[i]), x+ 50*i ,y); 
    }
}

char get_letter(){
    
    if (data == A) return 'a'; 
    if (data == B) return 'b'; 
    if (data == C) return 'c'; 
    if (data == D) return 'd'; 
    if (data == E) return 'e'; 
    if (data == F) return 'f';
    if (data == G) return 'g';
    if (data == H) return 'h';
    if (data == I) return 'i';
    if (data == J) return 'j';
    if (data == K) return 'k';
    if (data == L) return 'l';
    if (data == M) return 'm';
    if (data == N) return 'n';
    if (data == O) return 'o';
    if (data == P) return 'p';
    if (data == Q) return 'q';
    if (data == R) return 'r';
    if (data == S) return 's';
    if (data == T) return 't';
    if (data == U) return 'u';
    if (data == V) return 'v';
    if (data == W) return 'w';
    if (data == X) return 'x';
    if (data == Y) return 'y';
    if (data == Z) return 'z';
    
    if (data == KBC_KC_SPACE)   return ' ';
    if (data == BCK_SPACE)      return 'D';
   
    return 'A';


}
