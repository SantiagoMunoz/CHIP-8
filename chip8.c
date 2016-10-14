/**
 *  CHIP-8 Interpreter
 *  ------------------
 *  Use this to play, learn or whatever you want.
 */

#include <string.h>
#include <stdio.h>

unsigned short opcode; //Keypad
unsigned char memory[4096]; //Memory (4kb)
unsigned char V[16]; //Registers V0-V15 plus Carry Flag
unsigned short I; //Index register
unsigned short pc; //Program counter

unsigned char screen[64*32]; //64x32 display
unsigned char key[16]; //Keypad
unsigned char draw;

unsigned char delay_timer, sound_timer; // Timer registers

unsigned short stack[16]; //stack
unsigned short sp; //stack pointer

//Main Functions
void init();        //Init interpreter
void load_file();   //Load the program
void cycle();       //Process one cpu cycle
void update_screen();        //Update the screen
void input();       //Update keyboard status

int main(int argc, char **argv){

    //Setup

    init();
    load_file();

    for(;;){

        cycle();
        if(draw){
            update_screen();
        }
        
        input();

    }
    
    return 0;
}

void init(){        
    /* Init interpreter */

    //Init memory
    memset(memory, 4096, 0x00); 
    //Load interpreter data (fontset)
    
    //Init Program Counter
    pc = 512; //Start of Program Memory 
    //

}

void load_file(){   
    //Load the program
    FILE *f;
    f = fopen("BRIX","r");
    
    fread(memory + 512, 1, 3583, f); //Read up to 3583 elements of 1 byte and store them from position 512
    fclose(f);
}
    
void cycle(){       
    /* Process one cpu cycle */

    //Fetch instruction
    opcode = memory[pc] << 8 | memory[pc + 1]; 
    //Decode instruction

    //Execute instruction
    
    //Update timers
    
}
void update_screen(){
    //Update the screen
    
}
void input(){
    //Update keyboard status

}


