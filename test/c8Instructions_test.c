#include <stdlib.h>
#include <check.h>
#include "../c8Instructions.h"
#include "../c8Env.h"

c8Env tEnv;

START_TEST(CLS)
{
    c8Env_init(&tEnv);
    tEnv.screen[32*6] = 1;
    tEnv.screen[22*15] = 1;
    tEnv.pc = 4;
    c8_CLS(&tEnv, 0x00E0);
    ck_assert_msg( (tEnv.screen[32*6]== 0) & (tEnv.screen[22*15] == 0),
            "CLS does not clear the screen");
    ck_assert_msg(tEnv.pc == 6,
            "CLS does not increase the program counter");
}
END_TEST

START_TEST(RET)
{
    c8Env_init(&tEnv);
    tEnv.sp = 1;
    tEnv.stack[0] = 0xCAFE;
    c8_RET(&tEnv, 0x00EE);
    ck_assert_msg(tEnv.sp == 0,
           "RET does not decrease the stack pointer");
    ck_assert_msg(tEnv.pc == 0xCAFE,
            "RET does not set the program counter to the value at the top of the stack");
}
END_TEST

START_TEST(JP)
{
    c8Env_init(&tEnv);
    tEnv.pc = 0x500;
    c8_JP(&tEnv, 0x1BEA);
    ck_assert_msg(tEnv.pc == 0xBEA,
            "JP did not jump to the correct memaddress");
    tEnv.V[0] = 33;
    c8_JP(&tEnv, 0xBCEA);
    ck_assert_msg(tEnv.pc == (0xCEA + 33),
            "JP did not jumpt to memaddress + V0");

}
END_TEST

START_TEST(CALL)
{
    c8Env_init(&tEnv);
    tEnv.pc = 0x123;
    tEnv.sp = 3;
    c8_CALL(&tEnv, 0x2876);
    ck_assert_msg(tEnv.pc == 0x876,
            "CALL did not jump to the correct memadress");
    ck_assert_msg(tEnv.sp == 4,
            "Stack pointer did not increase after CALL");
    ck_assert_msg(tEnv.stack[tEnv.sp-1] == 0x123,
            "Top of the stack does not contain the address before CALL");

}
END_TEST

START_TEST(SE)
{
    c8Env_init(&tEnv);
    tEnv.pc = 0;
    tEnv.V[4] = 0x11;
    c8_SE(&tEnv, 0x3411);
    ck_assert_msg(tEnv.pc == 4,
            "SE does not skip when Vx == kk");
    tEnv.pc = 0;
    c8_SE(&tEnv, 0x3412);
    ck_assert_msg(tEnv.pc == 2,
            "SE skips when Vx != kk");
    tEnv.V[5] = 0x11;
    tEnv.V[6] = 0x12;
    tEnv.pc = 0;
    c8_SE(&tEnv, 0x5450);
    ck_assert_msg(tEnv.pc == 4,
            "SE does not skip when Vx == Vy");
    tEnv.pc = 0;
    c8_SE(&tEnv, 0x5460);
    ck_assert_msg(tEnv.pc == 2,
            "SE skips when Vx != Vy");
}
END_TEST

START_TEST(SNE)
{
	c8Env_init(&tEnv);
	tEnv.pc = 0;
	tEnv.V[3] = 0x0A;
	c8_SNE(&tEnv, 0x430A);
	ck_assert_msg(tEnv.pc == 2,
					"SNE skips when Vx == kk");
	tEnv.pc = 0;
	c8_SNE(&tEnv, 0x430B);
	ck_assert_msg(tEnv.pc == 4,
					"SNE does not skip when Vx != kk");
	tEnv.V[4] = 0x0A;
	tEnv.V[5] = 0xB0;
	tEnv.pc = 0;
	c8_SNE(&tEnv, 0x9340);
	ck_assert_msg(tEnv.pc == 2,
					"SNE skips when Vx == Vy");
	tEnv.pc = 0;
	c8_SNE(&tEnv, 0x9350);
	ck_assert_msg(tEnv.pc == 4,
					"SNE does not skip when Vx != Vy");
}
END_TEST

START_TEST(LD)
{
	c8Env_init(&tEnv);
	tEnv.V[5] = 0;
	tEnv.pc = 0;
	c8_LD(&tEnv, 0x6577);
	ck_assert_msg(tEnv.V[5] == 0x77,
					"LD does not set Vx = kk");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.V[2] = 0x22;
	tEnv.pc = 0;
	c8_LD(&tEnv, 0x8520);
	ck_assert_msg(tEnv.V[5] == tEnv.V[2],
					"LD does not set Vx = Vy");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.I = 0;
	tEnv.pc = 0;
	c8_LD(&tEnv, 0xABCD);
	ck_assert_msg(tEnv.I == 0xBCD,
					"LD does not set I = kkk");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.V[7] = 0;
	tEnv.pc = 0;
	tEnv.delay_timer = 0xAA;
	c8_LD(&tEnv, 0xF707);
	ck_assert_msg(tEnv.delay_timer == tEnv.V[7],
					"LD does not set delay Vk to delay timer");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.pc = 0;
	tEnv.V[2] = 0;
	tEnv.keypad[2] = 1;
	c8_LD(&tEnv, 0xF20A);
	ck_assert_msg(tEnv.V[2] == 2 ,
					"LD does not set Vx to the correct key");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.delay_timer = 0;
	tEnv.V[5] = 0xAA;
	tEnv.pc = 0;
	c8_LD(&tEnv, 0xF515);
	ck_assert_msg(tEnv.delay_timer == tEnv.V[5],
					"LD does not set the delay timer to Vk");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.sound_timer = 0;
	tEnv.V[6] = 0xBB;
	tEnv.pc = 0;
	c8_LD(&tEnv, 0xF618);
	ck_assert_msg(tEnv.sound_timer == tEnv.V[6],
					"LD does not set the sound timer to Vk");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.I = 0;
	tEnv.pc = 0;
	int i;
	for(i=0;i<0xF;i++){
		tEnv.V[8] = i;
		c8_LD(&tEnv, 0xF829);
		ck_assert_msg(tEnv.I == (i)*0x5,
						"LD does not set I to digit sprites");
		ck_assert_msg(tEnv.pc == 2*(i+1),
						"LD does not increase the program counter");
	}
	//
	tEnv.pc = 0;
	tEnv.V[3] = 123;
	tEnv.I = 200;
	c8_LD(&tEnv,0xF333);
	ck_assert_msg( (tEnv.memory[tEnv.I] == 1) & (tEnv.memory[tEnv.I+1] == 2) & (tEnv.memory[tEnv.I+2] == 3),
					"LD does not store BCD of Vk to memory correctly");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.pc = 0;
	tEnv.V[0] = 4;
	tEnv.V[1] = 5;
	tEnv.V[2] = 6;
	tEnv.V[3] = 7;
	tEnv.I = 37;
	c8_LD(&tEnv, 0xF355);
	ck_assert_msg( (tEnv.memory[tEnv.I] == tEnv.V[0]) & (tEnv.memory[tEnv.I+1] == tEnv.V[1]) & (tEnv.memory[tEnv.I+2] == tEnv.V[2]) & (tEnv.memory[tEnv.I+3] == tEnv.V[3]),
					"LD does not store registers into memory");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");
	//
	tEnv.pc = 0;
	tEnv.I = 84;
	tEnv.memory[tEnv.I] = 2;
	tEnv.memory[tEnv.I+1] = 3;
	tEnv.memory[tEnv.I+2] = 7;
	tEnv.memory[tEnv.I+3] = 5;
	c8_LD(&tEnv, 0xF365);
	ck_assert_msg( (tEnv.memory[tEnv.I] == tEnv.V[0]) & (tEnv.memory[tEnv.I+1] == tEnv.V[1]) & (tEnv.memory[tEnv.I+2] == tEnv.V[2]) & (tEnv.memory[tEnv.I+3] == tEnv.V[3]),
					"LD does load registers from memory");
	ck_assert_msg(tEnv.pc == 2,
					"LD does not increase the program counter");


	
}
END_TEST

Suite *instruction_suite(void)
{
    Suite *s = suite_create("Instructions");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, CLS);
    tcase_add_test(tc_core, RET);
    tcase_add_test(tc_core, JP);
    tcase_add_test(tc_core, CALL);
    tcase_add_test(tc_core, SE);
	tcase_add_test(tc_core, SNE);
	tcase_add_test(tc_core, LD);

    suite_add_tcase(s, tc_core);
    return s;
}

int main()
{
    Suite *s = instruction_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int no_failed = srunner_ntests_failed(sr);

    srunner_free(sr);
    if(no_failed == 0)
        return EXIT_SUCCESS;
    return EXIT_FAILURE;

}
