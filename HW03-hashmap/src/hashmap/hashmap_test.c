#include "../../../assets/Unity/src/unity.h"
#include "hashmap.h"

void setUp(void) {}

void tearDown(void) {}

void test_hash_map(void)
{

    HashMap map = hm_init();

    hm_set(&map, "a", 100);
    TEST_ASSERT_EQUAL(1, hm_len(&map));
    TEST_ASSERT_EQUAL(100, hm_get(&map, "a"));

    hm_set(&map, "a", 200);
    TEST_ASSERT_EQUAL(1, hm_len(&map));
    TEST_ASSERT_EQUAL(200, hm_get(&map, "a"));

    hm_del(&map, "a");
    TEST_ASSERT_EQUAL(0, hm_len(&map));
    TEST_ASSERT_EQUAL(0, hm_get(&map, "a"));

    hm_clear(&map);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_hash_map);
    return UNITY_END();
}
