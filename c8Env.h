#ifndef _C8ENV_H_
#define _C8ENV_H_
#include <stdint.h>

#define SCREEN_W    64
#define SCREEN_H    32

typedef struct{
    uint8_t debug;
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint8_t screen[SCREEN_W*SCREEN_H];
    uint8_t keypad[16];
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
} c8Env;

void c8Env_init(c8Env *env);
uint8_t c8Env_load_memory(c8Env *env, char *filename);
uint16_t fetch(c8Env *env);
void c8Env_tick(c8Env *env);

#endif
