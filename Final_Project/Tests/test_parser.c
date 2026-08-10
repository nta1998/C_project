#include <stdio.h>
#include <string.h>
#include "../Headers/parser.h"

static int checks = 0;
static int failures = 0;

#define CHECK(expr)                                            \
    do {                                                       \
        checks++;                                              \
        if (!(expr)) {                                         \
            failures++;                                        \
            printf("FAIL  %s:%d  %s\n", __FILE__, __LINE__, #expr); \
        }                                                      \
    } while (0)

static void test_start_with_letter(void)
{
    CHECK(start_with_letter("MAIN")   != 0);
    CHECK(start_with_letter("m")      != 0);
    CHECK(start_with_letter("Loop2")  != 0);

    CHECK(start_with_letter("1abc")   == 0);
    CHECK(start_with_letter("_abc")   == 0);
    CHECK(start_with_letter(".entry") == 0);
    CHECK(start_with_letter("")       == 0);
}

static void test_all_chars_valid(void)
{
    CHECK(all_chars_valid_l("MAIN")     != 0);
    CHECK(all_chars_valid_m("Loop2")    != 0);
    CHECK(all_chars_valid_m("x1y2z3")   != 0);

    CHECK(all_chars_valid_l("bad_name") == 0);
    CHECK(all_chars_valid_m("bad_name") == 0);
    CHECK(all_chars_valid_l("bad_name") == 0);   /* ראה הערה למטה */
    CHECK(all_chars_valid_l("end:")     == 0);
    CHECK(all_chars_valid_l("")         == 0);
}

static void test_is_reserved_word(void)
{
    CHECK(it_is_reserved_word("mov")   != 0);
    CHECK(it_is_reserved_word("add")   != 0);
    CHECK(it_is_reserved_word("stop")  != 0);

    CHECK(it_is_reserved_word("MAIN")  == 0);
    CHECK(it_is_reserved_word("Loop")  == 0);
    CHECK(it_is_reserved_word("moving")== 0);   /* לא תת-מחרוזת! */
    CHECK(it_is_reserved_word("")      == 0);
}

int main(void)
{
    test_start_with_char();
    test_all_chars_valid();
    test_is_reserved_word();

    printf("\n%d checks, %d failures\n", checks, failures);
    return failures != 0;
}