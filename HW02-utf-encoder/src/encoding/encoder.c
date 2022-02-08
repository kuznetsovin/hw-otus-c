#include <inttypes.h>
#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include "encoder.h"
#include "cp1251.h"
#include "koi8-r.h"
#include "8859_5.h"


const size_t CODE_PAGE_SIZE = 256;
const size_t CODE_PAGE_MEM_SIZE = CODE_PAGE_SIZE * sizeof(uint16_t);

void fatal(const char* text) {
    perror(text);
    exit(EXIT_FAILURE);
}

UtfFile encoding_to_utf8(char* encoding, char* input_file) {
    UtfFile result = { .content = NULL, .size = 0};

    FILE* fp = fopen(input_file, "rb");
    if (fp == NULL)
    {
        fatal("open input file failed");
    }

    int encoding_char = 0;
    uint16_t unicode_char = 0;
    uint8_t utf_8_char[2] = { 0, 0 };
    result.content = calloc(0, sizeof(unicode_char));

    uint16_t* code_page = calloc(CODE_PAGE_SIZE, CODE_PAGE_MEM_SIZE);

    if (strcmp(encoding, "cp1251") == 0) {
        memcpy(code_page, cp1251_map, CODE_PAGE_MEM_SIZE);
    } else if (strcmp(encoding, "koi8-r") == 0) {
        memcpy(code_page, koi8_map, CODE_PAGE_MEM_SIZE);
    } else if (strcmp(encoding, "iso-8859-5") == 0) {
        memcpy(code_page, iso8859_5_map, CODE_PAGE_MEM_SIZE);
    } else {
        free(code_page);
        fatal("Encoding isn't support yet. Valid code page cp1251, koi8-r, iso-8859-5");
    };


    while ((encoding_char = fgetc(fp)) != EOF) {
        unicode_char = code_page[encoding_char];

        if (unicode_char <= 0x7F) {
            result.size += sizeof((unsigned char)encoding_char);
            result.content = realloc(result.content, result.size);
            memcpy(result.content + (result.size - sizeof((unsigned char)encoding_char)) , &encoding_char, sizeof((unsigned char)encoding_char));
        }

        // append code point bits
        // https://en.wikipedia.org/wiki/UTF-8
        if (unicode_char >= 0x80 && unicode_char <= 0x7FF) {
            // transform unicode to byte array for append UTF-8 code point
            utf_8_char[1] = unicode_char & 0xff;
            utf_8_char[0] = unicode_char >> 8;

            // append mask bits 110 to first byte 10 to second byte
            utf_8_char[0] = (unicode_char >> 6) | 0xC0;

            // append mask bits 10 to second byte
            utf_8_char[1] = (unicode_char & 0x3F)| 0x80;

            result.size += sizeof(utf_8_char);
            result.content = realloc(result.content, result.size);
            memcpy(result.content + (result.size - sizeof(utf_8_char)) , &utf_8_char, sizeof(utf_8_char));
        }

        if (unicode_char > 0x7FF) {
            free(code_page);
            free(result.content);
            fatal("Encoding isn't support yet.");
        }
    };

    result.size += sizeof(unsigned char);
    result.content = realloc(result.content, result.size);
    result.content[result.size] = '\0';

    free(code_page);
    fclose(fp);
    return result;
}
