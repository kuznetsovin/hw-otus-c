#include "../../../assets/Unity/src/unity.h"
#include "wcounter.h"

void setUp(void) {}

void tearDown(void) {}

void test_word_stat(void)
{
    HashMap hm = get_word_stat("test/text.txt");

    TEST_ASSERT_EQUAL(3, hm_len(&hm));
    TEST_ASSERT_EQUAL(4, hm_get(&hm, "foo"));
    TEST_ASSERT_EQUAL(5, hm_get(&hm, "bar"));
    TEST_ASSERT_EQUAL(1, hm_get(&hm, "co"));

    hm_clear(&hm);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_word_stat);
    return UNITY_END();
}
