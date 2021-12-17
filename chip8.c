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
#include <unistd.h>

#include "c8Env.h"
#include "c8IO.h"
#include "c8Instructions.h"
#include "c8Decode.h"

#define FRAME_MS 200

int main(int argc, char **argv)
{
    c8Env *m_env = NULL;
    c8IO *m_io = NULL;
    uint16_t op;
    c8Func f;
    if(argc < 2){
        printf("Usage: chip8 <filename> [-d]\n");
        return 1;
    }
    m_env = (c8Env*)malloc(sizeof(c8Env));
    c8Env_init(m_env);
    if(c8Env_load_memory(m_env,argv[1]))
        goto cleanup;
    if(argc == 3){
        if(strcmp(argv[2],"-d") == 0){
            m_env->debug = 1;
            printf("Running on DEBUG mode\n");
        }
    }
    m_io = (c8IO*)malloc(sizeof(c8IO));
    if(init_io(m_io))
        goto cleanup;
    do{
        op = fetch(m_env);
        f = decode(op);
        if(f)
            f(m_env, op);   //Exec
        else
            break;
        if(m_env->debug)
            printf("\n");
        c8Env_tick(m_env); 
		usleep(20000);
        render(m_env,m_io);
    }while(!keyboard(m_env, m_io));

cleanup:
    free(m_env);
    teardown_io(m_io);
    free(m_io);
    printf("Bye bye\n");
    return 0;
}

