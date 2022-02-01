#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zipjpeg/zipjpeg.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("invalid path to testing file\n");
        return 0;
    }

    FileInfo f = load_file(argv[1]);

    if (has_file_zip_part(&f) == 0) {
        printf("file doesn't cointain zip\n");
        return 0;
    }

    size_t current_offset = get_zip_offest(&f);
    ZipRecord current_record;

    printf("Zip includes files: \n");
    while (current_offset < f.size) {
        current_record = get_zip_recod(&f, current_offset);

        // checking that record has filename
        if (current_record.filename == NULL) {
            break;
        }

        printf("%s\n", current_record.filename);
        current_offset += current_record.record_len;

        free(current_record.filename);
    }

    free(f.data);

    return 0;
}
