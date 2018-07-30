#include "c8IO.h"


uint8_t init_io(c8IO *sc)
{
    memset(sc, 0x00, sizeof(c8IO));
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    sc->window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W*ZOOM, SCREEN_H*ZOOM, 0);
    if(!sc->window)
        return 1;
    sc->ren = SDL_CreateRenderer(sc->window, -1, 0);
    if(!sc->ren)
        return 1;
    sc->text = SDL_CreateTexture(sc->ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_W*ZOOM, SCREEN_H*ZOOM);
    if(!sc->text)
        return 1;
    sc->pixels = (uint32_t*)malloc(SCREEN_W*ZOOM*SCREEN_H*ZOOM*sizeof(uint32_t));
    memset(sc->pixels, 255, SCREEN_W*ZOOM*SCREEN_H*ZOOM);
    return 0;
}

void teardown_io(c8IO *sc)
{
    if(sc->text)
        SDL_DestroyTexture(sc->text);
    if(sc->ren)
        SDL_DestroyRenderer(sc->ren);
    if(sc->window)
        SDL_DestroyWindow(sc->window);
    if(sc->pixels)
        free(sc->pixels);
    SDL_Quit();
}

void render(c8Env *env, c8IO *sc)
{

    int x, y;   //Screen positions
    int i,j;    //Pixel offsets

    //Wash screen white
    memset(sc->pixels, 255, sizeof(SCREEN_H*ZOOM*SCREEN_W*ZOOM)*sizeof(uint32_t));
    //Draw what we actually want to draw
    for(x=0;x<(SCREEN_W - 1);x++){
        for(y=0;y<(SCREEN_H - 1);y++){

            if(env->screen[y*SCREEN_W+x] != 0){
                for(j=0;j<ZOOM;j++)
                    for(i=0;i<ZOOM;i++)
                        sc->pixels[(y*ZOOM+j)*SCREEN_W + (x*ZOOM+i)] = 0;
            }
        }
    }
    //Push to screen
    SDL_UpdateTexture(sc->text,NULL, sc->pixels, SCREEN_W*ZOOM*sizeof(uint32_t));
    SDL_RenderClear(sc->ren);
    SDL_RenderCopy(sc->ren, sc->text, NULL, NULL);
    SDL_RenderPresent(sc->ren);
}

uint8_t keyboard(c8Env *env, c8IO *sc)
{
    SDL_Event event;
	int val, key;
    while( (SDL_PollEvent(&event)) ){
		if(event.type == SDL_QUIT)
				return 1;
		if((event.type != SDL_KEYDOWN) & (event.type != SDL_KEYUP))
				continue;
		val = (event.type == SDL_KEYDOWN) ? 1 : 0;
        if(event.key.keysym.sym == SDLK_0)
				key = 0x0;
		else if(event.key.keysym.sym == SDLK_1)
                key = 0x1;
		else if(event.key.keysym.sym == SDLK_2)
                key = 0x2;
		else if(event.key.keysym.sym == SDLK_3)
                key = 0x3;
		else if(event.key.keysym.sym == SDLK_4)
                key = 0xC;
		else if(event.key.keysym.sym == SDLK_q)
                key = 0x4;
		else if(event.key.keysym.sym == SDLK_w)
                key = 0x5;
		else if(event.key.keysym.sym == SDLK_e)
                key = 0x6;
		else if(event.key.keysym.sym == SDLK_r)
                key = 0xD;
		else if(event.key.keysym.sym == SDLK_a)
                key = 0x7;
		else if(event.key.keysym.sym == SDLK_s)
                 key = 0x8;
		else if(event.key.keysym.sym == SDLK_d)
                 key = 0x9;
		else if(event.key.keysym.sym == SDLK_f)
                 key = 0xE;
		else if(event.key.keysym.sym == SDLK_z)
                 key = 0xA;
		else if(event.key.keysym.sym == SDLK_x)
                 key = 0x0;
		else if(event.key.keysym.sym == SDLK_c)
                 key = 0xB;
		else if(event.key.keysym.sym == SDLK_v)
                key = 0xF;
		else if(event.key.keysym.sym == SDLK_p)
                return 1;
		env->keypad[key] = val;
    } //end while
    return 0;
}
