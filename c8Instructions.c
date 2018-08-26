#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "c8Instructions.h"

uint8_t match_opcode(uint16_t opcode, uint16_t reg, uint16_t mask)
{
    if( (opcode & mask) == reg)
        return 1;
    return 0;
}

void c8_CLS(c8Env *env, uint16_t opcode)
{
    if(env->debug)
        printf("CLS");
    memset(env->screen, 0x00, SCREEN_W*SCREEN_H);
    env->pc += 2;
}

void c8_RET(c8Env *env, uint16_t opcode)
{
    if(env->debug)
        printf("RET");
    if(env->sp == 0)
        return;
    env->sp--;
    env->pc= env->stack[env->sp];
}

void c8_JP(c8Env *env, uint16_t opcode)
{
    if(match_opcode(opcode, 0x1000, 0xF000)){
        env->pc = (opcode & 0x0FFF);
        if(env->debug)
            printf("JP (inmediate, 0x%X)", env->pc);
        return;
    }
    if(match_opcode(opcode, 0xB000, 0xF000)){
        env->pc = env->V[0] + (opcode & 0x0FFF);
        if(env->debug)
            printf("JP (inmediate + V[0], 0x%X)", env->pc);
        return;
    }
}

void c8_CALL(c8Env *env, uint16_t opcode)
{
    if(env->debug)
        printf("CALL");
    if(env->sp >= 12)
        return;
    env->stack[env->sp] = env->pc;
    env->sp++;
    env->pc = (opcode & 0x0FFF);
}

void c8_SE(c8Env *env, uint16_t opcode)
{
    if(match_opcode(opcode, 0x3000, 0xF000)){
        // Vx == kk
        if(env->debug)
            printf("SE V%X, 0x%X", (opcode & 0x0F00)>>8, opcode & 0x00FF);
        if( env->V[(opcode & 0x0F00)>>8] == (opcode & 0x00FF) )
            env->pc += 4;
        else
            env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0x5000, 0xF000)){
        //Vx == Vy
        if(env->debug)
            printf("SE V%X, V%X", (opcode & 0x0F00)>>8, (opcode & 0x00F)>>4);
        if( env->V[(opcode & 0x0F00)>>8] == env->V[(opcode & 0x00F0)>>4] )
            env->pc += 4;
        else
            env->pc += 2;
        return;
    }
}

void c8_SNE(c8Env *env, uint16_t opcode)
{
    if(match_opcode(opcode, 0x4000, 0xF000)){

        if(env->debug)
            printf("SNE V%X, 0x%X", (opcode & 0x0F00)>>8, opcode & 0x00FF);
        if(env->V[(opcode & 0x0F00)>>8] != (opcode & 0x00FF)){
            env->pc += 4;
        }else{
            env->pc += 2;
        }
        return;
    }
    if(match_opcode(opcode, 0x9000, 0xF00F)){

        if(env->debug)
            printf("SNE V%X, V%X", (opcode & 0x0F00)>>8, (opcode & 0x00F)>>4);
        if(env->V[(opcode & 0x0F00) >> 8] != env->V[(opcode & 0x00F0) >> 4]){
            env->pc += 4;
        }else{
            env->pc += 2;
        }
        return;
    }
}

void c8_LD(c8Env *env, uint16_t opcode)
{
    if(match_opcode(opcode, 0x6000, 0xF000)){
        env->V[(opcode & 0x0F00)>>8] = (opcode & 0x0FF);
        env->pc +=2;
        return;
    }
    if(match_opcode(opcode, 0x8000, 0xF00F)){
        env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x00F0)>>4];
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xA000, 0xF000)){
        env->I = opcode & 0x0FFF;
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF033, 0xF0FF)){
        env->memory[env->I]   = env->V[(opcode & 0x0F00) >> 8] / 100;
        env->memory[env->I+1] = env->V[(opcode & 0x0F00) >> 8] % 100 / 10;
        env->memory[env->I+2] = env->V[(opcode & 0x0F00) >> 8] % 10;
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF065, 0xF0FF)){
        int i;
        for(i=0; i<= ( (opcode & 0x0F00) >> 8 ); i++)
            env->V[i] = env->memory[env->I+i];
            env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF055, 0xF0FF)){
        int i;
        for(i=0; i<= ( (opcode & 0x0F00) >> 8 ); i++)
            env->memory[env->I+i] = env->V[i];
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF029, 0xF0FF)){
        if(env->V[(opcode & 0x0F00) >> 8] < 0xF)
            env->I = 0x000 + 5*env->V[(opcode & 0x0F00) >>8];
        else
            env->I = 0x000; //Write 0 by default
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF015, 0xF0FF)){
        env->delay_timer = env->V[(opcode & 0x0F00) >> 8];
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF018, 0xF0FF)){
        env->sound_timer = env->V[(opcode & 0x0F00) >> 8];
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF007, 0xF0FF)){
        env->V[ (opcode & 0x0F00) >> 8 ] = env->delay_timer;
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF00A, 0xF0FF)){
        int i;
        while(1){
            for(i=0;i<16;i++){
                if(env->keypad[i] == 1){
                    env->V[(opcode & 0x0F00) >> 8] = i;
                    env->pc += 2;
                    return;
                }
            }
        }
        return;
    }
}

void c8_ADD(c8Env *env, uint16_t opcode)
{
    if(match_opcode(opcode, 0x7000, 0xF000)){
        env->V[(opcode & 0x0F00)>>8] += (opcode & 0x0FF);
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0x8004, 0xF00F)){
        uint16_t Vtemp = env->V[(opcode & 0x0F00)>>8];
        env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] + env->V[(opcode & 0x00F0)>>4];
        env->V[15] = (Vtemp > env->V[(opcode & 0x0F00)>>8]) ? 1 : 0;
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF01E, 0xF0FF)){
        env->I += env->V[(opcode & 0x0F00) >> 8];
        env->pc += 2;
        return;
    }
}

void c8_OR(c8Env *env, uint16_t opcode)
{
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] | env->V[(opcode & 0x00F0)>>4];
    env->pc += 2;
}

void c8_AND(c8Env *env, uint16_t opcode)
{
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] & env->V[(opcode & 0x00F0)>>4];
    env->pc += 2;
}

void c8_XOR(c8Env *env, uint16_t opcode)
{
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] ^ env->V[(opcode & 0x00F0)>>4];
    env->pc += 2;
}

void c8_SUB(c8Env *env, uint16_t opcode)
{
   env->V[15] = env->V[(opcode & 0x0F00)>>8] > env->V[(opcode & 0x00F0)>>4] ? 1 : 0;
   env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] - env->V[(opcode & 0x00F0)>>4];
   env->pc+= 2;
}

void c8_SHR(c8Env *env, uint16_t opcode)
{
    env->V[15] = (env->V[(opcode & 0x0F00)>>8] & 0x0001) != 0 ? 1 : 0;
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] >> 1; //LSR = Divide by 2
    env->pc += 2;
}

void c8_SUBN(c8Env *env, uint16_t opcode)
{
    env->V[15] = env->V[(opcode & 0x00F0)>>4] > env->V[(opcode & 0x0F00)>>8] ? 1 : 0;
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x00F0)>>4] - env->V[(opcode & 0x0F00)>>8];
    env->pc += 2;
}

void c8_SHL(c8Env *env, uint16_t opcode)
{
    env->V[15] = (env->V[(opcode & 0x0F00)>>8] & 0x8000) > 0 ? 1 : 0;
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] << 1; //LSL = Multiply by 2
    env->pc += 2;
}

void c8_RND(c8Env *env, uint16_t opcode)
{
    env->V[ (opcode & 0x0F00) >> 8 ] = (opcode & 0x00FF) + (rand() % 255);
    env->pc +=2;
}

void c8_DRW(c8Env *env, uint16_t opcode)
{
    env->V[15] = 0; //Preemptive flag reset
    uint16_t posy_orig = env->V[(opcode & 0x00F0)>>4];
    uint16_t posx_orig = env->V[(opcode & 0x0F00)>>8];
    int i,j;
    uint16_t posx, posy, Vtemp;
    for(i=0; i < (opcode & 0x000F); i++){ // i - stride y
        posy = posy_orig + i;
        if (posy > 31)  posy -= 32;
        for(j=0;j < 8; j++){ // j - stride x (Sprites are ALWAYS 8 bit wide)
            posx = posx_orig + j;
            if(posx > 63)   posx -= 64; //If something goes off the screen, draw it coming from the other side
            Vtemp = env->screen[posx+(posy*64)];   //Load contents of current pixel-> (Vx +j, Vy + i) to then calculate the override
            if( (env->memory[env->I+i] & (0x80>>j)) != 0){
                //Pixel should be set
                if(Vtemp != 0){
                    //Pixel was already set-> Reset to 0 and drive V16 up
                    env->screen[posx+(64*posy)] = 0;
                    env->V[15] = 1;
                }else{
                    env->screen[posx+(64*posy)] = 1;
                }
            }
        }
    }
    env->pc+=2;
}

void c8_SKP(c8Env *env, uint16_t opcode)
{
    if(  env->keypad[ env->V[(opcode & 0x0F00) >> 8] ] == 1  ){
        env->pc += 4;
    }else{
        env->pc +=2;
    }
}

void c8_SKNP(c8Env *env, uint16_t opcode)
{
    if(  env->keypad[ env->V[(opcode & 0x0F00) >> 8] ] == 0 ){
        env->pc += 4;
    }else{
        env->pc +=2;
    }

}
