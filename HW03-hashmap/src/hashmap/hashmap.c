#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

// Init empty hashmap
HashMap hm_init() {
    HashMap map = { 0 };


    map.data = calloc(1, sizeof(Item));
    map.size = 1;

    return map;
}

// Destroy hashmap
void hm_clear(HashMap* m) {
    for(size_t i=0; i < m->size; i++) {
        if (m->data[i].key != NULL) {
            free(m->data[i].key);
        }
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

// Rebuild hashmap after size increasing
void hm_rebuild(HashMap* old_map) {
    HashMap new_map = hm_init();

    Item* records = hm_iter(old_map);
    for(size_t i =0; i < hm_len(old_map); i++) {
        hm_set(&new_map, records[i].key, records[i].value);
    }

    // it's very bad
    old_map = &new_map;
}

// Set key value in hashmap
void hm_set(HashMap* m, char* key, int64_t val) {
    Item item = { .key = strdup(key), .value = val };

    size_t hash_val = hash(key) % m->size;

    if (m->data[hash_val].key == NULL) {
        m->num_of_recs += 1;
        m->data[hash_val] = item;
        return;
    }

    // if key hash is exist then compare old and new keys.
    // if keys is equals update key value
    if (strncmp(m->data[hash_val].key, item.key, strlen(item.key)) == 0) {
        m->data[hash_val].value = item.value;
        return;
    }

    // resize hash map if it doesn't have free cells
    if (m->num_of_recs == m->size) {
        m->size *= 2;
        m->data = realloc(m->data, m->size * sizeof(Item));

        // rebuild hash map because divide base was changed and index isn't change offset
        hm_rebuild(m);

        //recursion call for inserting current value to resize map
        hm_set(m, item.key, item.value);
        return;
    }

    // Try find key in using index and update their value.
    // if key doesn't find in using indexes then find first free cell in map,
    // that insert new item and increase record counter. If hash key does't exist while loop doesn't
    // start and item was inserted in current hash value
    size_t nearest_empty_cell = 0;
    for(size_t i = 0; i < m->size; i++) {
        if (m->data[i].key == NULL) {
            if (nearest_empty_cell == 0 && i > hash_val) {
                nearest_empty_cell = i;
            }

            continue;
        }

        if (strncmp(m->data[i].key, item.key, strlen(item.key)) == 0) {
            m->data[i].value = item.value;
            return;
        }
    }

    m->data[nearest_empty_cell] = item;
    m->num_of_recs += 1;
}

// Get value by key form hash map
int64_t hm_get(HashMap* m, char* key) {
    size_t hash_val = hash(key) % m->size;

    // cheks empty pointer
    if (m->data[hash_val].key == NULL) {
        return 0;
    }

    //if hash was found compairs keys
    if (strncmp(m->data[hash_val].key, key, strlen(key)) == 0) {
        return m->data[hash_val].value;
    }

    // was collision and nessesary find in another cells
    for(size_t i = 0; i < m->size; i++) {
        if (m->data[i].key == NULL) {
            continue;
        }

        if (strncmp(m->data[i].key, key, strlen(key)) == 0) {
            return m->data[i].value;
        }
    }

    return 0;
}

// Delete key and value from hashmap
void hm_del(HashMap* m, char* key) {
    size_t hash_val = hash(key) % m->size;

    Item empty_item = { 0 };

    if (strcmp(m->data[hash_val].key, key) != 0) {
        size_t i = hash_val;
        while (strcmp(m->data[i].key, key) != 0) {
            i++;
        }
        hash_val = i;
    }

    m->data[hash_val] = empty_item;
    m->num_of_recs -= 1;

    return;
}


Item* hm_iter(HashMap* m) {
    Item* result = calloc(0, sizeof(Item));

    uint32_t offset = 0;
    size_t i = 0;

    for (i = 0; i < m->size; i++) {
        if (m->data[i].key != NULL) {
            result = realloc(result, (offset + 1) * sizeof(Item));
            result[offset] = m->data[i];
            offset += 1;
        }
    }

    return result;
}
