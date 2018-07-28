#include <stdlib.h>
#include "c8Instruction.h"

uint8_t match_opcode(uint16_t opcode, uint16_t reg, uint16_t mask)
{
    if( (opcode & mask) == reg)
        return 1;
    return 0;
}

void c8_CLS(c8Env *env, uint16_t opcode)
{
    memset(env->screen, 0x00, SCREEN_W*SCREEN_H);
    env->pc += 2;
}

void c8_RET(c8Env *env, uint16_t opcode)
{
    if(env->sp == 0)
        return;
    env->sp--;
    env->pc= stack[sp];
}

void c8_JP(c8Env *env, uint16_t opcode)
{
    if(match_opcode(opcode, 0x1000, 0xF000)){
        env->pc = (opcode & 0x0FFF);
        return;
    }
    if(match_opcode(opcode, 0xB000, 0xF000)){
        env->pc = env->V[0] + (opcode & 0x0FFF);
        return;
    }
}

void c8_CALL(c8Env *env, uint16_t opcode)
{
    if(env->sp >= 12)
        return;
    env->stack[sp] = pc;
    env->sp++;
    env->pc = (opcode & 0x0FFF);
}

void c8_SE(c8Env *env, uint16_t opcode)
{
    if(x8_match_opcode(opcode, 0x3000, 0xF000)){
        // Vx == kk
        if( env->V[(opcode & 0x0F00)>>8] == (opcode & 0x00FF) )
            env->pc += 4;
        else
            env->pc += 2;
        return;
    }
    if(x8_match_opcode(opcode, 0x5000, 0xF000)){
        //Vx == Vy
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
        if(env->V[(opcode & 0x0F00)>>8] != (opcode & 0x00FF)){
            env->pc += 4;
        }else{
            env->pc += 2;
        }
        return;
    }
    if(match_opcode(opcode, 0x9000, 0xF00F)){
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
        env->memory[I]   = (env->V[(opcode && 0x0F00) >> 8] & 0xF00 ) >> 8;
        env->memory[I+1] = (env->V[(opcode && 0x0F00) >> 8] & 0x0F0 ) >> 4;
        env->memory[I+2] = (env->V[(opcode && 0x0F00) >> 8] & 0x00F );
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF065, 0xF0FF)){
        int i;
        for(i=0; i< ( (opcode & 0x0F00) >> 8 ); i++)
            env->V[i] = env->memory[I+i];
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF055, 0xF0FF)){
        int i;
        for(i=0; i< ( (opcode & 0x0F00) >> 8 ); i++)
            env->memory[I+i] = V[i];
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
        env->V[ (opcode * 0x0F00) >> 8 ] = env->delay_timer;
        env->pc += 2;
        return;
    }
    if(match_opcode(opcode, 0xF00A, 0xF0FF)){
        int i;
        for(i=0;i<16;i++)
            if(env->key[i] == 1){
                env->V[(opcode & 0x0F00) >> 8] = i;
                env->pc += 2;
                break;
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
        env->V[16] = (Vtemp > env->V[(opcode & 0x0F00)>>8]) ? 1 : 0;
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
   env->V[16] = env->V[(opcode & 0x0F00)>>8] > env->V[(opcode & 0x00F0)>>4] ? 1 : 0;
   env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] - env->V[(opcode & 0x00F0)>>4];
   env->pc+= 2;
}

void c8_SHR(c8Env *env, uint16_t opcode)
{
    env->V[16] = (env->V[(opcode & 0x0F00)>>8] & 0x0001) > 0 ? 1 : 0;
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] >> 1; //LSR = Divide by 2
    env->pc += 2;
}

void c8_SUBN(c8Env *env, uint16_t opcode)
{
    env->V[16] = env->V[(opcode & 0x00F0)>>8] > env->V[(opcode & 0x0F00)>>4] ? 1 : 0;
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x00F0)>>8] - env->V[(opcode & 0x0F00)>>4];
    env->pc += 2;
}

void c8_SHL(c8Env *env, uint16_t opcode)
{
    env->V[16] = (env->V[(opcode & 0x0F00)>>8] & 0x8000) > 0 ? 1 : 0;
    env->V[(opcode & 0x0F00)>>8] = env->V[(opcode & 0x0F00)>>8] << 1; //LSL = Multiply by 2
    env->pc += 2;
}

void c8_RND(c8Env *env, uint16_t opcode)
{
    env->V[ (opcode & 0x0F00) >> 8 ] = (opcode & 0x00FF) + (rand() % 255);
    env->pc +=2;
}

void c8_DRAW(c8Env *env, uint16_t opcode)
{
    env->V[16] = 0; //Preemptive flag reset
    uint16_t posy_orig = env->V[(opcode & 0x00F0)>>4];
    uint16_t posx_orig = env->V[(opcode & 0x0F00)>>8];
    int i,j;
    uint16_t posx, posy, Vtemp;
    for(i=0; i < (opcode & 0x000F); i++){ // i - stride y
        posy = posy_orig + i;
        if (posy > 31)  posy -= 32;
        for(j=0;j < 8; j++){ // j - stride x (Sprites are ALWAYS 8 bit long)
            posx = posx_orig + j;
            if(posx > 63)   posx -= 63; //If something goes off the screen, draw it coming from the other side
            Vtemp = env->screen[posx+(posy*64)];   //Load contents of current pixel-> (Vx +j, Vy + i) to then calculate the override
            if( (env->memory[I+i] & (0x80>>j)) != 0){
                //Pixel should be set
                if(Vtemp != 0){
                    //Pixel was already set-> Reset to 0 and drive V16 up
                    env->screen[posx+(64*posy)] = 0;
                    env->V[16] = 1;
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
    if( ( ((opcode & 0x0F00)>>8) <16) & ( env->key[ (opcode & 0x0F00) >> 8 ] == 1 ) ){
        env->pc += 4;
    }else{
        env->pc +=2;
    }
}

void c8_SKPN(c8Env *env, uint16_t opcode)
{

    if( ( ((opcode & 0x0F00)>>8) <16) & ( env->key[ (opcode & 0x0F00) >> 8 ] == 0 ) ){
        env->pc += 4;
    }else{
        env->pc +=2;
    }

}