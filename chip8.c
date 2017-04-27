/**
 *  CHIP-8 Interpreter
 *  ------------------
 *  Use this to play, learn or whatever you want.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>

#define SDL_WHITE  0xFFFFFFFF
#define SDL_BLACK  0x00000000

#define FRAME_DELAY_MS 1000/30

unsigned short opcode; //Keypad
unsigned char memory[4096]; //Memory (4kb)
unsigned char V[16]; //Registers V0-V15 plus Carry Flag
unsigned short I; //Index register
unsigned short pc; //Program counter

unsigned char screen[64*32]; //64x32 display
unsigned char key[16]; //Keypad
unsigned char draw;

SDL_Window *window;
SDL_Renderer *ren;
SDL_Texture *tex;
SDL_Event event;
uint32_t pixels[256*128]; // 64*32 upscaled 4/1 to 265*128

unsigned char delay_timer, sound_timer; // Timer registers

unsigned short stack[16]; //stack
unsigned char sp; //stack pointer

//Main Functions
void init();                        // Init interpreter
int load_file(char *filename);      // Load the program
void cycle();                       // Process one cpu cycle
void update_screen();               // Update the screen
void input();                       // Update keyboard status

unsigned char running;

int main(int argc, char **argv){

uint32_t local_time;
    //Setup
    running = 1;

    if(argc < 2){
        printf("Usage: chip8 <filename>\n");
        return 1;
    }

    init();
    if(load_file(argv[1])){
        return 1;
    }

    memset(screen, 0x00, 64*32);

    draw = 1;
    while(running == 1){
        int i;
        if(draw){
            update_screen();
            draw = 0;
        }
        for(i=0; i<FRAME_DELAY_MS/2;i++){
            input();
            SDL_Delay(1);
        }
        cycle();
        for(i=0; i<FRAME_DELAY_MS/2;i++){
            input();
            SDL_Delay(1);
        }
    }
    SDL_Quit();
    return 0;
}

void init(){
    int i;
    /* Init interpreter */
    //Init SDL

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        printf("SDL Init error!\n");
    window = SDL_CreateWindow("Chip-8",100, 100, 256, 128, SDL_WINDOW_SHOWN); //Each screen pixel is 4 pixel wide
    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 256, 128);
    //Init memory and registers
    memset(memory, 0x00, 4096);
    memset(V, 0x00, 16);
    memset(screen, 0x00, 64*32);
    //Load interpreter data (fontset)
    //0
    memory[0x000] = 0xF0;
    memory[0x001] = 0x90;
    memory[0x002] = 0x00;
    memory[0x003] = 0x00;
    memory[0x004] = 0xF0;
    //1
    memory[0x005] = 0x20;
    memory[0x006] = 0x60;
    memory[0x007] = 0x20;
    memory[0x008] = 0x20;
    memory[0x009] = 0x70;
    //2
    memory[0x00A] = 0xF0;
    memory[0x00B] = 0x10;
    memory[0x00C] = 0xF0;
    memory[0x00D] = 0x80;
    memory[0x00E] = 0xF0;
    //3
    memory[0x00F] = 0xF0;
    memory[0x010] = 0x10;
    memory[0x011] = 0xF0;
    memory[0x012] = 0x10;
    memory[0x013] = 0xF0;
    //4
    memory[0x014] = 0x90;
    memory[0x015] = 0x90;
    memory[0x016] = 0xF0;
    memory[0x017] = 0x10;
    memory[0x018] = 0x10;
    //5
    memory[0x019] = 0xF0;
    memory[0x01A] = 0x80;
    memory[0x01B] = 0xF0;
    memory[0x01C] = 0x10;
    memory[0x01D] = 0xF0;
    //6
    memory[0x01E] = 0xF0;
    memory[0x01F] = 0x80;
    memory[0x020] = 0xF0;
    memory[0x021] = 0x90;
    memory[0x022] = 0xF0;
    //7
    memory[0x023] = 0xF0;
    memory[0x024] = 0x10;
    memory[0x025] = 0x20;
    memory[0x026] = 0x40;
    memory[0x027] = 0x40;
    //8
    memory[0x028] = 0xF0;
    memory[0x029] = 0x90;
    memory[0x02A] = 0xF0;
    memory[0x02B] = 0x90;
    memory[0x02C] = 0xF0;
    //9
    memory[0x02D] = 0xF0;
    memory[0x02E] = 0x90;
    memory[0x02F] = 0xF0;
    memory[0x030] = 0x10;
    memory[0x031] = 0xF0;
    //A
    memory[0x032] = 0xF0;
    memory[0x033] = 0x90;
    memory[0x034] = 0xF0;
    memory[0x035] = 0x90;
    memory[0x036] = 0x90;
    //B
    memory[0x037] = 0xF0;
    memory[0x038] = 0x90;
    memory[0x039] = 0xE0;
    memory[0x03A] = 0x90;
    memory[0x03B] = 0xE0;
    //C
    memory[0x03C] = 0xF0;
    memory[0x03D] = 0x90;
    memory[0x03E] = 0xF0;
    memory[0x03F] = 0x10;
    memory[0x040] = 0xF0;
    //D
    memory[0x041] = 0xE0;
    memory[0x042] = 0x90;
    memory[0x043] = 0x90;
    memory[0x044] = 0x90;
    memory[0x045] = 0xE0;
    //E
    memory[0x046] = 0xF0;
    memory[0x047] = 0x80;
    memory[0x048] = 0xF0;
    memory[0x049] = 0x80;
    memory[0x04A] = 0xF0;
    //F
    memory[0x04B] = 0xF0;
    memory[0x04C] = 0x80;
    memory[0x04D] = 0xF0;
    memory[0x04E] = 0x80;
    memory[0x04F] = 0x80;

    //Init Program Counter
    pc = 512; //Start of Program Memory

    //Init Stack Pointer
    sp = 0;
}

int load_file(char *filename){
    //Load the program
    FILE *f;
    printf("Loading program ");
    printf(filename);
    printf("\n");
    f = fopen(filename,"rb");
    if (f == NULL){
        printf("Unable to read program file\n");
        return 1;
    }

    fread(memory + 512, 1, 3583, f); //Read up to 3583 elements of 1 byte and store them from position 512
    fclose(f);
    return 0;
}

void cycle(){
    unsigned char Vtemp = 0x00;
    unsigned char i = 0, j =0;
    unsigned short posx = 0, posy = 0;
    unsigned short posx_orig = 0, posy_orig = 0;
    /* Process one cpu cycle */

    //Fetch instruction
    opcode = memory[pc] << 8 | memory[pc + 1];
    //Decode instruction + Execute
    switch( (opcode & 0xF000) >> 12){
        case 0x0:
            //Either 0nnn, 00E0 or 00EE
            if(opcode == 0x00E0){
                //00E0 - Clear the display
                draw = 1;
                memset(screen, 0x00, 64*32);
                pc += 2;
            }else if(opcode == 0x00EE){
                //00EE - Return from a subroutine
                if(sp > 0){
                    sp--;
                    pc= stack[sp];
                }
            }else{
                //Instruction 0nnn is deliberately ignored
            }
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
                pc = (opcode & 0x0FFF);
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
            if( V[(opcode & 0x0F00)>>8] == V[(opcode & 0x00F0)>>4] ){
                pc += 4;
            }else{
                pc += 2;
            }
            break;
        case 0x6:
            //6xkk - set Vx = kk
            V[(opcode & 0x0F00)>>8] = (opcode & 0x0FF);
            pc +=2;
            break;
        case 0x7:
            //7xkk - Add kk to Vx, then store in Vx
            V[(opcode & 0x0F00)>>8] += (opcode & 0x0FF);
            pc +=2;
            break;
        case 0x8:
            // 0x8xyZ where Z defines the instruction
            switch (opcode & 0x000F){
                case 0x0:
                    //8xy0 - Set Vx = Vy
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x00F0)>>4];
                    break;
                case 0x1:
                    //8xy1 - Set Vx = Vx | Vy
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] | V[(opcode & 0x00F0)>>4];
                    break;
                case 0x2:
                    //8xy2 - Set Vx = Vx & Vy
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] & V[(opcode & 0x00F0)>>4];
                    break;
                case 0x3:
                    //8xy3 - Set Vx = Vx XOR Vy
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] ^ V[(opcode & 0x00F0)>>4];
                    break;
                case  0x4:
                    //8xy4 - Set Vx = Vx + Vy with carry on V16
                    Vtemp = V[(opcode & 0x0F00)>>8];
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] + V[(opcode & 0x00F0)>>4];
                    V[16] = (Vtemp > V[(opcode & 0x0F00)>>8]) ? 1 : 0;
                    break;
                case 0x5:
                    //8xy5 - Set Vx = Vx - Vy with NOT borrow on V16
                    V[16] = V[(opcode & 0x0F00)>>8] > V[(opcode & 0x00F0)>>4] ? 1 : 0;
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] - V[(opcode & 0x00F0)>>4];
                    break;
                case  0x6:
                    //8xy6 - If LSB of Vx is 1, set Vf and LSR Vx
                    V[16] = (V[(opcode & 0x0F00)>>8] & 0x0001) > 0 ? 1 : 0;
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] >> 1; //LSR = Divide by 2
                    break;
                case 0x7:
                    //8xy7 - Set Vx = Vy - Vx with NOT borrow on V16
                    V[16] = V[(opcode & 0x00F0)>>8] > V[(opcode & 0x0F00)>>4] ? 1 : 0;
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x00F0)>>8] - V[(opcode & 0x0F00)>>4];
                    break;
                case 0xE:
                    //8xy6 - If MSB of Vx is 1, set Vf and LSR Vx
                    V[16] = (V[(opcode & 0x0F00)>>8] & 0x8000) > 0 ? 1 : 0;
                    V[(opcode & 0x0F00)>>8] = V[(opcode & 0x0F00)>>8] << 1; //LSL = Multiply by 2
                    break;
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
            posy_orig = V[(opcode & 0x00F0)>>4];
            posx_orig = V[(opcode & 0x0F00)>>8];
            for(i=0; i < (opcode & 0x000F); i++){ // i - stride y
                posy = posy_orig + i;
                if (posy > 31)  posy -= 32;
                for(j=0;j < 8; j++){ // j - stride x (Sprites are ALWAYS 8 bit long)
                    //If something goes off the screen, draw it coming from the other side
                    posx = posx_orig + j;
                    if(posx > 63)   posx -= 63;
                    Vtemp = screen[posx+(posy*64)];   //Load contents of current pixel-> (Vx +j, Vy + i) to then calculate the override
                    if( (memory[I+i] & (0x80>>j)) != 0){
                        //Pixel should be set
                        if(Vtemp != 0){
                            //Pixel was already set-> Reset to 0 and drive V16 up
                            screen[posx+(64*posy)] = 0;
                            V[16] = 1;
                        }else{
                            screen[posx+(64*posy)] = 1;
                        }
                    }
                }
            }
            draw = 1;
            pc+=2;
            break;
        case 0xE:
            if( (opcode & 0x000F) == 0xE ){
                //Ex9E - Skip next instruction if key at Vx is pressed
                if( ( ((opcode & 0x0F00)>>8) <16) & ( key[ (opcode & 0x0F00) >> 8 ] == 1 ) ){
                    pc += 4;
                }else{
                    pc +=2;
                }
            }
            if( (opcode & 0x000F) == 0x1 ){
                //ExA1 - Skip next instruction if key at Vx is not pressed
                if( ( ((opcode & 0x0F00)>>8) <16) & ( key[ (opcode & 0x0F00) >> 8 ] == 0 ) ){
                    pc += 4;
                }else{
                    pc +=2;
                }
            }
            break;
        case 0xF:
            if((opcode & 0x00FF) == 0x0007){
                //Fx0A - Vx = Value of delay timer
                V[ (opcode * 0x0F00) >> 8 ] = delay_timer;
            }
            if((opcode & 0x00FF) == 0x000A){
                //Fx0A  - Wait for a key press, then store the value of the key in Vx
                i=16;    //Used to register if a key was pressed
                while(i==18)
                    for(j=0;j<16;j++)
                        if(key[j] == 1) i=j;     //Inherently higher value keys are prioritary
                V[(opcode & 0x0F00) >> 8] = key[j];
            }
            if((opcode & 0x00FF) == 0x0015){
                //Fx15 - Set DT to the value of Vx
                delay_timer = V[(opcode & 0x0F00) >> 8];
            }
            if((opcode & 0x00FF) == 0x0018){
                //Fx18 - Set ST to the value os Vx
                sound_timer = V[(opcode & 0x0F00) >> 8];
            }
            if((opcode & 0x00FF) == 0x001E){
                //Fx1E - Add I and Vx, then store in I
                I += V[(opcode & 0x0F00) >> 8];
            }
            if((opcode & 0x00FF) == 0x0029){
                //Fx29 - Set I = Location in memory of hex sprite of value Vx
                if(V[(opcode & 0x0F00) >> 8] < 0xF)
                    I = 0x000 + 5*V[(opcode & 0x0F00) >>8];
                else
                    I = 0x000; //Write 0 by default
            }
            if((opcode & 0x00FF) == 0x0033){
                //Fx33 - Store BCD of Vx at locations I, I+1 and I+2
                memory[I]   = (V[(opcode && 0x0F00) >> 8] & 0xF00 ) >> 8;
                memory[I+1] = (V[(opcode && 0x0F00) >> 8] & 0x0F0 ) >> 4;
                memory[I+2] = (V[(opcode && 0x0F00) >> 8] & 0x00F );
            }
            if((opcode & 0x00FF) == 0x0055){
                //Fx55 - Dump registers V0 to Vx in memory starting from memory position given by I
                for(i=0; i< ( (opcode & 0x0F00) >> 8 ); i++)
                    memory[I+i] = V[i];
            }
            if((opcode & 0x00FF) == 0x0065){
                //Fx65 - Load registers from memory starting from position given by I
                for(i=0; i< ( (opcode & 0x0F00) >> 8 ); i++)
                    V[i] = memory[I+i];
            }
            pc +=2;
            break;
        default:
            printf("Instruction not recognized\n");
            pc+=2;
            break;
    }

    //Update timers
    if(sound_timer > 0){
        --sound_timer;
        if(sound_timer == 0){
            //Play sound
            printf("\a");
        }
    }

    if(delay_timer > 0) --delay_timer;
    return;
}

void update_screen(){
    //Update the screen

    unsigned char i, j, ii,jj;
    SDL_Rect m_pixel;
    m_pixel.w = 4;
    m_pixel.h = 4;

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    for(j=0;j<32;j++){
        for(i=0;i<64;i++){
            if(screen[j*64+i] != 0){
                m_pixel.x = 4*i;
                m_pixel.y = 4*j;
                SDL_RenderFillRect(ren, &m_pixel);
            }
        }
    }
    SDL_RenderPresent(ren);
}

void input(){
    //Update keyboard status
    while( (SDL_PollEvent(&event)) ){
        switch(event.type){
            case SDL_KEYDOWN:
                //key pressed
            case SDL_KEYUP:
                //key released
                if(event.key.keysym.sym == SDLK_0){
                        key[0x0] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_1){
                        key[0x1] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_2){
                        key[0x2] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_3){
                        key[0x3] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_4){
                        key[0xC] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_q){
                        key[0x4] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_w){
                        key[0x5] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_e){
                        key[0x6] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_r){
                        key[0xD] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_a){
                        key[0x7] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_s){
                        key[0x8] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_d){
                        key[0x9] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_f){
                        key[0xE] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_z){
                        key[0xA] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_x){
                        key[0x0] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_c){
                        key[0xB] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                if(event.key.keysym.sym == SDLK_v){
                        key[0xF] = ( event.type == SDL_KEYDOWN ? 1 : 0 );
                }
                break;
            case SDL_QUIT:
                running = 0;
                break;
        } //end switch
    } //end while
}
