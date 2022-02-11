#include <inttypes.h>
#include <stdio.h>
#include "wcounter.h"


void fatal(const char* text) {
    perror(text);
    exit(EXIT_FAILURE);
}


HashMap get_word_stat(const char* file_path) {
    HashMap stats = hm_init();

    FILE* fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        fatal("open input file failed");
    }

    char word[100];
    uint64_t word_actual_count = 0;
    while (fscanf(fp, "%99s", word) != EOF) {
        word_actual_count = hm_get(&stats, word);
        word_actual_count += 1;

        hm_set(&stats, word, word_actual_count);
    }

    fclose(fp);

    return stats;
}
