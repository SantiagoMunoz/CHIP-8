#ifndef _C8INSTRUCTIONS_H_
#define _C8INSTRUCTIONS_H_
#include "c8Env.h"
#include <stdint.h>

typedef void(*c8Func)(c8Env*,uint16_t);

void c8_CLS(c8Env *env, uint16_t opcode);
void c8_RET(c8Env *env, uint16_t opcode); 
void c8_JP(c8Env *env, uint16_t opcode);
void c8_CALL(c8Env *env, uint16_t opcode);
void c8_SE(c8Env *env, uint16_t opcode);
void c8_SNE(c8Env *env, uint16_t opcode);
void c8_LD(c8Env *env, uint16_t opcode);
void c8_ADD(c8Env *env, uint16_t opcode);
void c8_OR(c8Env *env, uint16_t opcode);
void c8_AND(c8Env *env, uint16_t opcode);
void c8_XOR(c8Env *env, uint16_t opcode);
void c8_ADD(c8Env *env, uint16_t opcode);
void c8_SUB(c8Env *env, uint16_t opcode);
void c8_SHR(c8Env *env, uint16_t opcode);
void c8_SUBN(c8Env *env, uint16_t opcode);
void c8_SHL(c8Env *env, uint16_t opcode);
void c8_RND(c8Env *env, uint16_t opcode);
void c8_DRAW(c8Env *env, uint16_t opcode);
void c8_SKP(c8Env *env, uint16_t opcode);
void c8_SKPN(c8Env *env, uint16_t opcode);

#endif

