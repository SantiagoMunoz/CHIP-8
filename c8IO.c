#include "c8Screen.h"


uint8_t init_io(c8IO *sc)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        return 1;
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    env->window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W*ZOOM, SCREEN_H*ZOOM, 0);
    if(!env)
        return 1;
    env->ren = SDL_CreateRenderer(env->window, -1, 0);
    if(!env->ren)
        return 1;
    env->text = SDL_CreateTexture(env->ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_W*ZOOM, SCREEN_H*ZOOM);
    if(!env->text)
        return 1;
    env->pixels = (uint8_t*)malloc(SCREEN_W*ZOOM*SCREEN_H*ZOOM);
    memset(pixels, 255, SCREEN_W*ZOOM*SCREEN_H*ZOOM);
}

void render(c8Env *env, c8IO *sc)
{

}

uint8_t keyboard(c8Env *env, c8IO *sc)
{
    
    return 0;
}
