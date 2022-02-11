#include <inttypes.h>
#include <stdlib.h>

typedef struct {
    char*   key;
    int64_t value;
} Item;

typedef struct {
    Item*  data;
    size_t num_of_recs;
    size_t size;
} HashMap;

HashMap hm_init();
void    hm_set(HashMap* m, char* key, int64_t val);
int64_t hm_get(HashMap* m, char* key);
void    hm_del(HashMap* m, char* key);
size_t  hm_len(HashMap* m);
void    hm_clear(HashMap* m);
Item*   hm_iter(HashMap* m);
