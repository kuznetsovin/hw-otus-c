#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

// Init empty hashmap
HashMap hm_init() {
    HashMap map = { 0 };

    // Library used jenkins hash function which returns uint32 value, therefore
    // for hash table size may be got UINT32_MAX number of records
    map.data = calloc(UINT32_MAX, sizeof(Item));

    return map;
}

// Destroy hashmap
void hm_clear(HashMap* m) {
    free(m->using_cells);
    for(uint32_t i=0; i < m->num_of_recs; i++) {
        free(m->data[i].key);
    };
    free(m->data);
}

// Gets number of records in hashmap
size_t hm_len(HashMap* m) {
    return m->num_of_recs;
}

// Using [Jenkins hash function](https://en.wikipedia.org/wiki/Jenkins_hash_function)
uint32_t hash(char* val) {
    size_t i = 0;
    uint32_t hash = 0;

    while (i != strlen(val)) {
        hash += val[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

// Set key value in hashmap
void hm_set(HashMap* m, char* key, int64_t val) {
    Item item = { .key = NULL, .value = val };

    item.key = strdup(key);

    size_t hash_val = hash(key);

    // resets values don't increase record counter
    if (m->data[hash_val].key == NULL) {
        // increase record counter
        m->num_of_recs += 1;
        m->using_cells = realloc(m->using_cells, m->num_of_recs * sizeof(uint32_t));

        // mark cell as using
        m->using_cells[m->num_of_recs - 1] = hash_val;
    }

    m->data[hash_val] = item;
}

// Get value by key form hash map
int64_t hm_get(HashMap* m, char* key) {
    size_t hash_val = hash(key);

    // cheks empty pointer
    if (m->data+hash_val != NULL) {
        return m->data[hash_val].value;
    }

    return 0;
}

// Delete key and value from hashmap
void hm_del(HashMap* m, char* key) {
    size_t hash_val = hash(key);

    Item empty_item = { .key = NULL, .value = 0};

    m->data[hash_val] = empty_item;
    m->num_of_recs -= 1;
}


Item* hm_iter(HashMap* m) {
    Item* result = calloc(0, sizeof(Item));

    uint32_t offset = 0;
    size_t i = 0;

    for (i = 0; i < hm_len(m); i++) {
        offset = m->using_cells[i];
        result = realloc(result, (i + 1) * sizeof(uint32_t));

        result[i] =  m->data[offset];
    }

    return result;
}
