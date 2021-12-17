#ifndef _C8IO_H_
#define _C8IO_H_

#include <stdint.h>
#include "SDL.h"
#include "c8Env.h"

#define SDL_WHITE  0xFFFFFFFF
#define SDL_BLACK  0x00000000
#define ZOOM    4

typedef struct{
    SDL_Window *window;
    SDL_Renderer *ren;
    SDL_Texture *text;
    uint32_t *pixels;
} c8IO;

uint8_t init_io(c8IO *sc);
void teardown_io(c8IO *sc);
void render(c8Env *env, c8IO *sc);
uint8_t keyboard(c8Env *env, c8IO *sc);

#endif
