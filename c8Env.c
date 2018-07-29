#include <stdio.h>
#include <string.h>
#include "c8Env.h"

void c8Env_init(c8Env *env)
{
    memset(env, 0x00,sizeof(c8Env));
    //Copy builtins to memory
    //0
    env->memory[0x000] = 0xF0;
    env->memory[0x001] = 0x90;
    env->memory[0x002] = 0x00;
    env->memory[0x003] = 0x00;
    env->memory[0x004] = 0xF0;
    //1
    env->memory[0x005] = 0x20;
    env->memory[0x006] = 0x60;
    env->memory[0x007] = 0x20;
    env->memory[0x008] = 0x20;
    env->memory[0x009] = 0x70;
    //2
    env->memory[0x00A] = 0xF0;
    env->memory[0x00B] = 0x10;
    env->memory[0x00C] = 0xF0;
    env->memory[0x00D] = 0x80;
    env->memory[0x00E] = 0xF0;
    //3
    env->memory[0x00F] = 0xF0;
    env->memory[0x010] = 0x10;
    env->memory[0x011] = 0xF0;
    env->memory[0x012] = 0x10;
    env->memory[0x013] = 0xF0;
    //4
    env->memory[0x014] = 0x90;
    env->memory[0x015] = 0x90;
    env->memory[0x016] = 0xF0;
    env->memory[0x017] = 0x10;
    env->memory[0x018] = 0x10;
    //5
    env->memory[0x019] = 0xF0;
    env->memory[0x01A] = 0x80;
    env->memory[0x01B] = 0xF0;
    env->memory[0x01C] = 0x10;
    env->memory[0x01D] = 0xF0;
    //6
    env->memory[0x01E] = 0xF0;
    env->memory[0x01F] = 0x80;
    env->memory[0x020] = 0xF0;
    env->memory[0x021] = 0x90;
    env->memory[0x022] = 0xF0;
    //7
    env->memory[0x023] = 0xF0;
    env->memory[0x024] = 0x10;
    env->memory[0x025] = 0x20;
    env->memory[0x026] = 0x40;
    env->memory[0x027] = 0x40;
    //8
    env->memory[0x028] = 0xF0;
    env->memory[0x029] = 0x90;
    env->memory[0x02A] = 0xF0;
    env->memory[0x02B] = 0x90;
    env->memory[0x02C] = 0xF0;
    //9
    env->memory[0x02D] = 0xF0;
    env->memory[0x02E] = 0x90;
    env->memory[0x02F] = 0xF0;
    env->memory[0x030] = 0x10;
    env->memory[0x031] = 0xF0;
    //A
    env->memory[0x032] = 0xF0;
    env->memory[0x033] = 0x90;
    env->memory[0x034] = 0xF0;
    env->memory[0x035] = 0x90;
    env->memory[0x036] = 0x90;
    //B
    env->memory[0x037] = 0xF0;
    env->memory[0x038] = 0x90;
    env->memory[0x039] = 0xE0;
    env->memory[0x03A] = 0x90;
    env->memory[0x03B] = 0xE0;
    //C
    env->memory[0x03C] = 0xF0;
    env->memory[0x03D] = 0x90;
    env->memory[0x03E] = 0xF0;
    env->memory[0x03F] = 0x10;
    env->memory[0x040] = 0xF0;
    //D
    env->memory[0x041] = 0xE0;
    env->memory[0x042] = 0x90;
    env->memory[0x043] = 0x90;
    env->memory[0x044] = 0x90;
    env->memory[0x045] = 0xE0;
    //E
    env->memory[0x046] = 0xF0;
    env->memory[0x047] = 0x80;
    env->memory[0x048] = 0xF0;
    env->memory[0x049] = 0x80;
    env->memory[0x04A] = 0xF0;
    //F
    env->memory[0x04B] = 0xF0;
    env->memory[0x04C] = 0x80;
    env->memory[0x04D] = 0xF0;
    env->memory[0x04E] = 0x80;
    env->memory[0x04F] = 0x80;

    //Init PC
    env->pc = 512;
    //Init SP
    env->sp = 0;

}

uint8_t c8Env_load_memory(c8Env *env, char *filename)
{
    if((!filename) | (!env))
        return 1;
    FILE *f = fopen(filename,"rb");
    if(!f){
        return 1;
    }

    fread(env->memory + 512, 1, 3583, f); //Read up to 3583 elements of 1 byte and store them from position 512
    fclose(f);
    return 0;
}

void c8Env_tick(c8Env *env)
{
    if(env->sound_timer > 0){
        --env->sound_timer;
        if(env->sound_timer == 0){
            //Play sound
            printf("\a");
        }
    }
    if(env->delay_timer > 0) --delay_timer;
}

uint16_t fetch(c8Env *env)
{
    return (env->memory[env->pc] << 8) | env->memory[env->pc + 1];
}
