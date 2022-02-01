#include "../../../assets/Unity/src/unity.h"
#include "zipjpeg.h"

void setUp(void) {}

void tearDown(void) {}

void test_non_zipjpeg_file(void)
{
    FileInfo f = load_file("test/non-zipjpeg.jpg");
    TEST_ASSERT_EQUAL(30471, f.size);

    size_t is_zipjpeg = has_file_zip_part(&f);
    TEST_ASSERT_EQUAL(0, is_zipjpeg);
}

void test_load_zipjpeg_file(void)
{
    FileInfo f = load_file("test/zipjpeg.jpg");
    TEST_ASSERT_EQUAL(1195612, f.size);

    size_t is_zipjpeg = has_file_zip_part(&f);
    TEST_ASSERT_EQUAL(1, is_zipjpeg);

    size_t zip_offest = get_zip_offest(&f);
    TEST_ASSERT_EQUAL(30471, zip_offest);

    ZipRecord zip_rec = get_zip_recod(&f, zip_offest);
    TEST_ASSERT_EQUAL(66, zip_rec.record_len);
    TEST_ASSERT_EQUAL_STRING("jpeg-9d/", zip_rec.filename);

    zip_offest += zip_rec.record_len;
    zip_rec = get_zip_recod(&f, zip_offest);
    TEST_ASSERT_EQUAL(3208, zip_rec.record_len);
    TEST_ASSERT_EQUAL_STRING("jpeg-9d/jmemname.c", zip_rec.filename);

    zip_offest += zip_rec.record_len;
    zip_rec = get_zip_recod(&f, zip_offest);
    TEST_ASSERT_EQUAL(1083, zip_rec.record_len);
    TEST_ASSERT_EQUAL_STRING("jpeg-9d/makermak.vc6", zip_rec.filename);

}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_non_zipjpeg_file);
    RUN_TEST(test_load_zipjpeg_file);
    return UNITY_END();
}
