#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoding/encoder.h"

int main(int argc, char *argv[]) {

    switch(argc) {
        case 1:
            printf("Usage: ./utf-encoder <encoding> <input_file> <output_file>");
            return 0;
        case 2:
            printf("Input file is required");
            return 0;
        case 3:
            printf("Output file is required");
            return 0;
    }

    UtfFile f = encoding_to_utf8(argv[1], argv[2]);

    FILE* fp=fopen(argv[3], "wb");
    if (fp == NULL) {
        perror("open output file error: ");
        exit(EXIT_FAILURE);
    }

    // size - 1 because last char in content is \0
    if (fwrite(f.content, f.size - 1, 1, fp) != 1) {
        perror("write output file error: ");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    free(f.content);
    return 0;
}
