#include <inttypes.h>
#include <stdio.h>
#include "wcounter/wcounter.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Output file is required");
            return 0;
    }

    HashMap hm = get_word_stat(argv[1]);

    Item* records = hm_iter(&hm);
    printf("Words stats:\n");
    for(size_t i =0; i < hm_len(&hm); i++) {
        printf("\t%s: %" PRId64 "\n", records[i].key, records[i].value);
    }

    hm_clear(&hm);
    return 0;
}
