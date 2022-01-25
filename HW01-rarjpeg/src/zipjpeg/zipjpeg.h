#include <stdlib.h>

typedef struct {
    unsigned char*  data;
    uint32_t size;
} FileInfo;

typedef struct {
    char* filename;
    size_t record_len;
} ZipRecord;

FileInfo load_file(const char* file_path);
size_t has_file_zip_part(FileInfo*  f);
size_t get_zip_offest(FileInfo* f);
ZipRecord get_zip_recod(FileInfo* f, size_t record_offset);
