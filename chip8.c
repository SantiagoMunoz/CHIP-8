/**
 *  CHIP-8 Interpreter
 *  ------------------
 *  Use this to play, learn or whatever you want.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
unsigned char sp; //stack pointer

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
            draw = false;
        }
        
        input();

    }
    
    return 0;
}

void init(){        
    /* Init interpreter */

    //Init memory and registers
    memset(memory, 4096, 0x00);
    memset(V, 16, 0x00);
    //Load interpreter data (fontset)
    
    //Init Program Counter
    pc = 512; //Start of Program Memory 
    
    //Init Stack Pointer
    sp = 0;
}

void load_file(){   
    //Load the program
    FILE *f;
    f = fopen("BRIX","r");
    
    fread(memory + 512, 1, 3583, f); //Read up to 3583 elements of 1 byte and store them from position 512
    fclose(f);
}
    
void cycle(){       
    unsigned char Vtemp = 0x00;
    /* Process one cpu cycle */

    //Fetch instruction
    opcode = memory[pc] << 8 | memory[pc + 1]; 
    //Decode instruction + Execute
    switch( (opcode & 0xF0000) >> 12){
        case 0x0:
            //Either 0nnn, 00E0 or 00EE
            if(opcode == 0x00E0){
                //00E0 - Clear the display
                memset(screen, 64*32, 0x00);                
                draw = true;
                pc += 2;
            }
            if(opcode == 0x00EE){
                //00EE - Return from a subroutine
                if(sp > 0){
                    sp--;
                    pc= stack[sp];
                }
            }
            //Instruction 0nnn is deliberately ignored
            break;
        case 0x1:
            //1nnn - jump to 0xnnn
            pc = (opcode & 0x0FFF);
            break;
        case 0x2:
            //2nnn - call subroutine at nnn
            if(sp < 12){
                stack[sp] = pc;
                sp++;
            }
            break;
        case 0x3:
            //3xkk - skip next instruction if Vx == kk
            if( V[(opcode & 0x0F00)>>8] == (opcode & 0x00FF) ){
                pc += 4;
            }else{
                pc += 2;
            }
            break;
        case 0x4:
            //4xkk - skip next instruction if Vx != kk
            if( V[(opcode & 0x0F00)>>8] != (opcode & 0x00FF) ){
                pc += 4;
            }else{
                pc += 2;
            }
            break;
        case 0x5:
            //5xy0 - skip next instruction if Vx == Vy
            if( V[(opcode & 0x0F00)>>8] == V[opcode & 0x00F0] ){
                pc += 4;
            }else{
                pc += 2;
            }
            break;
        case 0x6:
            //6xkk - set Vx = kk
            V[(opcode & 0x0F00)>>8] = opcode & 0x0FF;
            pc +=2;
            break;
        case 0x7:
            //7xkk - Add kk to Vx, then store in Vx
            V[(opcode & 0x0F00)>>8] += opcode & 0x0FF;
            pc +=2;
            break;
        case 0x8:
            if((opcode & 0x000F) == 0x0){
                //8xy0 - Set Vx = Vy
                V[(opcode & 0x0F00)>>8] += V[(opcode & 0x00F0)>>4];
            }
            if((opcode & 0x000F) == 0x1){
                //8xy1 - Set Vx = Vx | Vy
                V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] | V[(opcode & 0x00F0)>>4];
            }
            if((opcode & 0x000F) == 0x2){
                //8xy2 - Set Vx = Vx & Vy
                V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] & V[(opcode & 0x00F0)>>4];
            }
            if((opcode & 0x000F) == 0x3){
                //8xy3 - Set Vx = Vx XOR Vy
                V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] ^ V[(opcode & 0x00F0)>>4];
            }
            if((opcode & 0x000F) == 0x4){
                //8xy4 - Set Vx = Vx + Vy with carry on V16 
                Vtemp = V[(opcode & 0x0F00)>>8];
                V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] + V[(opcode & 0x00F0)>>4];
                if(Vtemp > V[(opcode & 0x0F00)>>8]) V[16] = 1;  else  V[16] = 0;
            }
            if((opcode & 0x000F) == 0x5){
                //8xy5 - Set Vx = Vx - Vy with NOT borrow on V16 
                if(V[(opcode & 0x0F00)>>8] > V[(opcode & 0x00F0)>>4]) V[16] = 1; else V[16] = 0;
                V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] - V[(opcode & 0x00F0)>>4];
            }
            if((opcode & 0x000F) == 0x6){
                //8xy6 - If LSB of Vx is 1, set Vf and LSR Vx 
                if( (V[(opcode & 0x0F00)>>8] & 0x0001) > 0 ) V[16] = 1; else V[16] = 0;
                V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] >> 1; //LSR = Divide by 2
            }
            if((opcode & 0x000F) == 0x7){
                //8xy7 - Set Vx = Vy - Vx with NOT borrow on V16 
                if(V[(opcode & 0x00F0)>>8] > V[(opcode & 0x0F00)>>4]) V[16] = 1; else V[16] = 0;
                V[(opcode & 0x00F0)>>8] = V[(opcode & 0x00F0)>>8] - V[(opcode & 0x0F00)>>4];

            }
            if((opcode & 0x000F) == 0xE){
                //8xy6 - If MSB of Vx is 1, set Vf and LSR Vx 
                if( (V[(opcode & 0x0F00)>>8] & 0x8000) > 0 ) V[16] = 1;
                V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] << 1; //LSL = Multiply by 2
            }
            pc +=2;
            break;
        case 0x9:
            //9xy0 - Skip next instruction if Vx != Vy
            if( V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
                pc +=4;
            }else{
                pc +=2;
            }
            break;
        case 0xA:
            //Annn - Set I to nnn
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB:
            //Bnnn - Jump to V0 + nnn
            pc = V[0] + (opcode & 0x0FFF);
            break;
        case 0xC:
            //Cxkk - Set Vx = kk + random number between 0 and 255
            V[ (opcode & 0x0F00) >> 8 ] = (opcode & 0x00FF) + (rand() % 255);
            pc +=2;
            break;
        case 0xD:
            //Dxyn - Display n bit sprite starting at memory position given by I, coordinates (Vx,Vy) and then set VF to collision in case a pixel gets deleted.
            V[16] = 0; //Preemptive flag reset
            

            break;
        default;
            //Not implemented (yet)
            break;

    }
    
    //Update timers
    if(sound_timer > 0){
        --sound_timer;
        if(sound_timer == 0){
            //Play sound
            
        }
    }
    if(delay_timer > 0) --delay_timer; 
}
void update_screen(){
    //Update the screen
    
}
void input(){
    //Update keyboard status

}


