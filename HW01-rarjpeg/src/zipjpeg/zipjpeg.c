#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "zipjpeg.h"

const unsigned char START_JPEG_SIG[] = { 0xff, 0xd8 };
const unsigned char END_JPEG_SIG[] = { 0xff, 0xd9 };
const unsigned char START_ZIP_SIG[] = { 0x50, 0x4b, 0x03, 0x04 };

void exception(const char* text) {
    perror(text);
    exit(EXIT_FAILURE);
}

// Function is read data from file and load it to temporary struct
FileInfo load_file(const char* file_path)
{
    struct stat file_stat_buf;
    FileInfo result = { .data = NULL, .size = 0};

    FILE* fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        exception("open file failed");
    }

    // getting file len
    fstat (fileno (fp), &file_stat_buf);
    if ((fstat(fileno(fp), &file_stat_buf) != 0) || (!S_ISREG(file_stat_buf.st_mode))) {
        exception("failed get file size");
    }
    result.size = (uint32_t)file_stat_buf.st_size;

    // read file binary data to buffer
    if (fseek(fp, 0L, SEEK_SET) != 0)
    {
        exception("failed seek file to start");
    };

    result.data =  malloc(result.size + 1);
    int count = fread(result.data, sizeof(unsigned char), result.size, fp);

    // array must end 0-terminate
    result.data[count] = '\0';

    fclose(fp);
    return result;
}


/*
 * Function check that file has zip path.
 *
 * If file contains zip, then returns 1 (true) else returns 0 (false).

 * If file is jpen only it starts with 0xFFD8 and ends with 0xFFD9 bytes.
 * More information: https://habr.com/ru/company/infowatch/blog/337084/
*/
size_t has_file_zip_part(FileInfo*  f)
{
    if (f == NULL)
    {
        exception("invalid file descriptor");
    }

    size_t start_as_jpeg = memcmp(f->data, START_JPEG_SIG, sizeof(START_JPEG_SIG));

    // getting offset to last bytes in data
    unsigned char* last_data_bytes = f->data + (f->size - 2);
    size_t end_as_jpeg = memcmp(last_data_bytes, END_JPEG_SIG, sizeof(END_JPEG_SIG));

    // check that file is jpeg
    if (start_as_jpeg == 0 && end_as_jpeg == 0) {
        // it meens that file doesn't have zip part, because returns false
        return 0;
    }

    // file has zip part
    return 1;
}

/*
 * Function gets offest zip part
 *
 * Implementation finds only simple zip archive (that start with 0x50 0x4B 0x03 0x04 bytes)
 */
size_t get_zip_offest(FileInfo* f)
{
    if (f == NULL)
    {
        exception("invalid file descriptor");
    }

    size_t zip_offset = 0;
    for (size_t i = 0; i < f->size; i++)
    {
        size_t is_jpeg_end = memcmp(f->data + i, END_JPEG_SIG, sizeof(END_JPEG_SIG));
        if (is_jpeg_end == 0) {
            zip_offset = i + 2;
            size_t is_zip_start = memcmp(f->data + zip_offset, START_ZIP_SIG, sizeof(START_ZIP_SIG));

            if (is_zip_start == 0) {
                return zip_offset;
            }
        }
    }

    return 0;
}

/*
** Function read zip record info from input file by offset
 */
ZipRecord get_zip_recod(FileInfo* f, size_t record_offset)
{
    if (f == NULL)
    {
        exception("invalid file descriptor");
    }

    ZipRecord result = { .filename = NULL, .record_len = 0};

    // simple chechecking that there aren't files in archive or it were ended
    if (memcmp(f->data + record_offset, START_ZIP_SIG, sizeof(START_ZIP_SIG)) != 0)
    {
        return result;
    }

    result.record_len += 18;
    size_t current_offset = record_offset + result.record_len;

    // get compress size file from bytes to uint32
    uint32_t compress_size = f->data[current_offset]
        | (f->data[current_offset + 1] << 8)
        | (f->data[current_offset + 2] << 16)
        | (f->data[current_offset + 3] << 24);

    // add to offset for compressed size and uncompressed size in bytes
    result.record_len += 8;
    current_offset += 8;

    // transform filename lentgh (2 bytes) from bytes to uint
    uint16_t name_len = f->data[current_offset] | (f->data[current_offset + 1] << 8);
    result.record_len += 2;
    current_offset += 2;

    // transform extra data lentgh (2 bytes) from bytes to uint
    uint16_t exta_data_len = f->data[current_offset] | (f->data[current_offset + 1] << 8);
    result.record_len += 2;
    current_offset += 2;

    result.filename = calloc(name_len+1, sizeof(char));
    memcpy(result.filename, &f->data[current_offset], name_len);
    result.filename[name_len] = '\0';

    // calc full record len with data section
    result.record_len = result.record_len + name_len + exta_data_len + compress_size;

    return result;
}
