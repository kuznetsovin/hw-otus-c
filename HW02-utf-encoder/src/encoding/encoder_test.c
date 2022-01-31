#include "../../../assets/Unity/src/unity.h"
#include "encoder.h"

unsigned char* expected_utf8;

void setUp(void) {
    FILE* fp = fopen("test/utf-8.txt", "rb");
    if (fp == NULL)
    {
        perror("open expected file failed");
        exit(EXIT_FAILURE);
    }

    struct stat file_stat_buf;
    fstat (fileno (fp), &file_stat_buf);
    if ((fstat(fileno(fp), &file_stat_buf) != 0) || (!S_ISREG(file_stat_buf.st_mode))) {
        perror("getting size failed");
        exit(EXIT_FAILURE);
    }
    size_t size = (uint32_t)file_stat_buf.st_size;

    expected_utf8 =  malloc(size + 1);
    int count = fread(expected_utf8, sizeof(unsigned char), size, fp);
    expected_utf8[count] = '\0';

    /* printf("%d", count); */
}

void tearDown(void) {
    free(expected_utf8);
}

void test_encoding_from_cp1251(void)
{
    UtfFile actual_utf8 = encoding_to_utf8("cp1251", "test/cp1251.txt");

    TEST_ASSERT_EQUAL_STRING(expected_utf8, actual_utf8.content);

    free(actual_utf8.content);
}

void test_encoding_from_koi8(void)
{
    UtfFile actual_utf8 = encoding_to_utf8("koi8-r", "test/koi8.txt");

    TEST_ASSERT_EQUAL_STRING(expected_utf8, actual_utf8.content);

    free(actual_utf8.content);
}

void test_encoding_from_iso(void)
{
    UtfFile actual_utf8 = encoding_to_utf8("iso-8859-5", "test/iso-8859-5.txt");

    TEST_ASSERT_EQUAL_STRING(expected_utf8, actual_utf8.content);

    free(actual_utf8.content);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_encoding_from_cp1251);
    RUN_TEST(test_encoding_from_iso);
    RUN_TEST(test_encoding_from_koi8);
    return UNITY_END();
}
