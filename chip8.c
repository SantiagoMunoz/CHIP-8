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

void print_help(const char *prog_name)
{
    printf("CHIP-8 Interpreter\n");
    printf("Usage: %s <rom_file> [options]\n\n", prog_name);
    printf("Arguments:\n");
    printf("  <rom_file>    Path to CHIP-8 ROM file to execute\n\n");
    printf("Options:\n");
    printf("  -d            Enable debug mode\n");
    printf("  -h, --help    Display this help message\n");
}

int main(int argc, char **argv)
{
    c8Env *m_env = NULL;
    c8IO *m_io = NULL;
    uint16_t op;
    c8Func f;
    char *rom_file = NULL;
    int debug_mode = 0;

    // Check if no arguments or help flag
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-d") == 0) {
            debug_mode = 1;
        } else if (rom_file == NULL) {
            rom_file = argv[i];
        } else {
            printf("Error: Unknown argument '%s'\n\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    // Verify ROM file was specified
    if (rom_file == NULL) {
        printf("Error: No ROM file specified\n\n");
        print_help(argv[0]);
        return 1;
    }

    m_env = (c8Env*)malloc(sizeof(c8Env));
    c8Env_init(m_env);
    if(c8Env_load_memory(m_env, rom_file))
        goto cleanup;

    if (debug_mode) {
        m_env->debug = 1;
        printf("Running on DEBUG mode\n");
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
		usleep(100);
        render(m_env,m_io);
    }while(!keyboard(m_env, m_io));

cleanup:
    free(m_env);
    teardown_io(m_io);
    free(m_io);
    printf("Bye bye\n");
    return 0;
}

