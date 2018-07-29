#include <stdlib.h>
#include <check.h>
#include "../c8Instructions.h"
#include "../c8Env.h"

c8Env tEnv;

START_TEST(c8_CLS)
{
    c8Env_init(&tEnv);
    //Set some pixels on
    tEnv.screen[32*6] = 1;
    tEnv.screen[22*15] = 1;
    tEnv.pc = 4;
    //Clear screen
    c8_CLS(&tEnv, 0x00E0);
    //Check it has made its effect
    ck_assert_msg( (tEnv.screen[32*6]== 0) & (tEnv.screen[22*15] == 0),
            "CLS does not clear the screen");
    ck_assert_msg(tEnv.pc == 6,
            "CLS does not increase the PC");
}
END_TEST

START_TEST(c8_RET)
{
    c8Env_init(&tEnv);
    //Setup tEnv as if we were inside a subroutine
    tEnv.sp = 1;
    tEnv.stack[0] = 0xCAFE;
    //Call RET
    c8_RET(&tEnv, 0x00EE);
    //Check it has made effect
    ck_assert_msg(tEnv.sp == 0,
           "RET does not decrease the SP");
    ck_assert_msg(tEnv.pc == 0xCAFE,
            "RET does not set the PC to the value at the top of the stack");
}
END_TEST

Suite *instruction_suite(void)
{
    Suite *s = suite_create("Instructions");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, c8_CLS);
    tcase_add_test(tc_core, c8_RET);

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
