#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
    unsigned char* content;
    size_t size;
} UtfFile;

UtfFile encoding_to_utf8(char* encoding, char* input_file);
