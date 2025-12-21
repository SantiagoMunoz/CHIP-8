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
    ck_assert_msg(tEnv.stack[tEnv.sp-1] == 0x125,
            "Top of the stack does not contain the address before CALL + 2");

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

START_TEST(ADD)
{
    c8Env_init(&tEnv);
    tEnv.V[3] = 0x5;
    tEnv.pc = 0;
    c8_ADD(&tEnv, 0x7348);
    ck_assert_msg( tEnv.V[3] == (0x5+0x48),
            "ADD Vx += kk yields an incorrect result");
    ck_assert_msg(tEnv.pc == 2,
            "ADD does  not increase the program counter");
    tEnv.pc = 0;
    tEnv.V[0] = 0xFF;
    tEnv.V[1] = 0x2;
    tEnv.V[15] = 0;
    c8_ADD(&tEnv, 0x8014);
    ck_assert_msg(tEnv.V[0] == 0x1,
            "ADD Vx += Vy yields an incorrect result");
    ck_assert_msg(tEnv.pc == 2,
            "ADD does not increase the program counter");
    ck_assert_msg(tEnv.V[15] == 1,
            "ADD does not set the carry flag when it should");
    c8_ADD(&tEnv, 0x8014);
    ck_assert_msg(tEnv.V[15] == 0,
            "ADD does not clear the carry flag when it should");
    tEnv.I = 0x33;
    tEnv.pc = 0;
    tEnv.V[6] = 0x10;
    c8_ADD(&tEnv, 0xF61E);
    ck_assert_msg(tEnv.I = 0x43,
            "ADD I += Vx yields an incorrect result");
    ck_assert_msg(tEnv.pc == 2,
            "ADD does not increase the program counter");
}
END_TEST

START_TEST(OR)
{
    c8Env_init(&tEnv);
    tEnv.pc = 0;
    tEnv.V[1] = 0xFA;
    tEnv.V[2] = 0x0B;
    c8_OR(&tEnv, 0x8121);
    ck_assert_msg(tEnv.V[1] == 0xFA | 0x0B,
            "Bitwise OR does not yield the correct result");
    ck_assert_msg(tEnv.pc == 2,
            "Bitwise OR does not increase the program counter");
}
END_TEST

START_TEST(AND)
{
    c8Env_init(&tEnv);
    tEnv.pc = 0;
    tEnv.V[1] = 0xF3;
    tEnv.V[2] = 0xF1;
    c8_OR(&tEnv, 0x8122);
    ck_assert_msg(tEnv.V[1] == 0xF3 & 0xF1,
            "Bitwise AND does not yield the correct result");
    ck_assert_msg(tEnv.pc == 2,
            "Bitwise AND does not increase the program counter");
}
END_TEST

START_TEST(XOR)
{
    c8Env_init(&tEnv);
    tEnv.pc = 0;
    tEnv.V[1] = 0xFA;
    tEnv.V[2] = 0xFB;
    c8_OR(&tEnv, 0x8123);
    ck_assert_msg(tEnv.V[1] == 0xFA ^ 0xFB,
            "Bitwise AND does not yield the correct result");
    ck_assert_msg(tEnv.pc == 2,
            "Bitwise AND does not increase the program counter");
}
END_TEST

START_TEST(SUB)
{
	c8Env_init(&tEnv);
	tEnv.pc = 0;
	tEnv.V[0] = 0x4;
	tEnv.V[1] = 0x5;
	tEnv.V[15] = 0;
	c8_SUB(&tEnv, 0x8015);
	ck_assert_msg( tEnv.V[0] == 0xFF,
					"SUB does not yield the correct result");
	ck_assert_msg( tEnv.V[15] == 0,
					"SUB does not set the carry flag when it should");
	ck_assert_msg( tEnv.pc == 2,
					"SUB does not increase the program counter");
	tEnv.V[0] = 0x5;
	tEnv.V[1] = 0x4;
	tEnv.V[15] = 0;
	c8_SUB(&tEnv, 0x8015);
	ck_assert_msg( tEnv.V[0] == 0x01,
					"SUB does not yield the correct result");
	ck_assert_msg( tEnv.V[15] == 1,
					"SUB sets the carry flag when it should not");

}
END_TEST

START_TEST(SHR)
{
	c8Env_init(&tEnv);
	tEnv.V[9] = 0x2;
	tEnv.V[15] = 0;
	tEnv.pc = 0;
	c8_SHR(&tEnv, 0x8906);
	ck_assert_msg( tEnv.V[15] == 0,
					"SHR sets VF when the LSB is 0");
	ck_assert_msg( tEnv.V[9] == 0x1,
					"SHR does not divide by two correctly");
	ck_assert_msg( tEnv.pc == 2,
					"SHR does not increase the program counter");
	tEnv.V[9] = 0x1;
	c8_SHR(&tEnv, 0x8906);
	ck_assert_msg( tEnv.V[15] == 1,
					"SHR does not set VF when the LSB is 1");
}
END_TEST

START_TEST(SUBN)
{
	tEnv.pc = 0;
	tEnv.V[0] = 0x4;
	tEnv.V[1] = 0x5;
	tEnv.V[15] = 0;
	c8_SUBN(&tEnv, 0x8017);
	ck_assert_msg( tEnv.V[0] == 0x01,
					"SUBN does not yield the correct result");
	ck_assert_msg( tEnv.V[15] == 1,
					"SUBN does not set the carry flag when it should");
	ck_assert_msg( tEnv.pc == 2,
					"SUBN does not increase the program counter");
	tEnv.V[0] = 0x5;
	tEnv.V[1] = 0x4;
	tEnv.V[15] = 0;
	c8_SUBN(&tEnv, 0x8017);
	ck_assert_msg( tEnv.V[0] == 0xFF,
					"SUBN does not yield the correct result");
	ck_assert_msg( tEnv.V[15] == 0,
					"SUBN sets the carry flag when it should not");
}
END_TEST

START_TEST(SHL)
{
	c8Env_init(&tEnv);
	tEnv.V[9] = 0x2;
	tEnv.V[15] = 0;
	tEnv.pc = 0;
	c8_SHL(&tEnv, 0x890E);
	ck_assert_msg( tEnv.V[15] == 0,
					"SHL sets VF when the LSB is 0");
	ck_assert_msg( tEnv.V[9] == 0x4,
					"SHL does not multiply by two correctly");
	ck_assert_msg( tEnv.pc == 2,
					"SHL does not increase the program counter");
	tEnv.V[9] = 0x1;
	c8_SHR(&tEnv, 0x8906);
	ck_assert_msg( tEnv.V[15] == 1,
					"SHL does not set VF when the LSB is 1");
}
END_TEST

START_TEST(DRW)
{
	c8Env_init(&tEnv);
	tEnv.V[0] = 10;
	tEnv.V[1] = 10;
	tEnv.V[15] = 0;
	tEnv.I = 0;
	tEnv.pc = 0;
	c8_DRW(&tEnv, 0xD015); // Draw digit 0 in the screen
	ck_assert_msg(tEnv.pc == 2,
					"DRW does not increase the program counter");
	ck_assert_msg(tEnv.V[15] == 0,
					"DRW set VF to 1 when there is no collision");
	//Check that pixels are set
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<8;j++){
			ck_assert_msg(tEnv.screen[(tEnv.V[1]+i)*SCREEN_W+tEnv.V[0]+j] == ((tEnv.memory[tEnv.I+i] & (0x80 >> j)) != 0),
					"DRW pixel value mismatch!");
		}
	}
	//Paint a 0 again but 1 pixel to the right
	tEnv.V[0] = 11;
	c8_DRW(&tEnv, 0xD015); // Draw digit 0 in the screen
	ck_assert_msg(tEnv.V[15] == 1,
					"DRW does not set VF to 1 when there is a collision");
	//Check collisions get XORed correctly in the screen
	for(j=0;j<2;j++){
		for(i=0;i<3;i++){
			ck_assert_msg(tEnv.screen[(tEnv.V[1]+(j*4))*SCREEN_W+tEnv.V[0]+i] == 0,
					"DRW does not XOR pixels correctly");
		}
	}
}
END_TEST

START_TEST(SKP)
{
	c8Env_init(&tEnv);
	tEnv.pc = 0;
	tEnv.V[0] = 4;
	tEnv.keypad[4] = 1;
	c8_SKP(&tEnv, 0xE09E);
	ck_assert_msg(tEnv.pc == 4,
					"SKP does not skip instruction when key Vx is pressed");
	tEnv.keypad[4] = 0;
	tEnv.pc = 0;
	c8_SKP(&tEnv, 0xE09E);
	ck_assert_msg(tEnv.pc == 2,
					"SKP skips instruction when key Vx is not pressed");
}
END_TEST

START_TEST(SKNP)
{
	c8Env_init(&tEnv);
	tEnv.pc = 0;
	tEnv.V[0] = 4;
	tEnv.keypad[4] = 1;
	c8_SKNP(&tEnv, 0xE01A);
	ck_assert_msg(tEnv.pc == 2,
					"SKNP skips instruction when key Vx is not pressed");
	tEnv.keypad[4] = 0;
	tEnv.pc = 0;
	c8_SKNP(&tEnv, 0xE01A);
	ck_assert_msg(tEnv.pc == 4,
					"SKNP does not skip instruction when key Vx is pressed");

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
    tcase_add_test(tc_core, ADD);
    tcase_add_test(tc_core, OR);
    tcase_add_test(tc_core, AND);
	tcase_add_test(tc_core, XOR);
	tcase_add_test(tc_core, SUB);
	tcase_add_test(tc_core, SHR);
	tcase_add_test(tc_core, SUBN);
	tcase_add_test(tc_core, SHL);
	tcase_add_test(tc_core, DRW);
	tcase_add_test(tc_core, SKP);
	tcase_add_test(tc_core, SKNP);
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
