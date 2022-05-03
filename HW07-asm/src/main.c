#include <stdio.h>
#include <stdlib.h>

static const size_t data[] = {4, 8, 15, 16, 23, 42,};
static const char empty_str[] = {0x00};
static const char int_format[] = "%ld ";

typedef struct node {
    size_t el;
    struct node *prev;
} node;

size_t p(size_t el) {
    return el % 2;
}

node *add_element(node *rec, size_t el) {
    node *cur_node = malloc(16);
    if (!cur_node) {
        abort();
    }

    cur_node->el = el;
    cur_node->prev = rec;
    return cur_node;
}

void print_int(size_t el) {
    printf(int_format, el);
    fflush(0);
}

// asm `m` function
void m(node *list, void (*print_fn)(size_t)) {
    if (!list) {
        return;
    }

    print_fn(list->el);
    m(list->prev, print_fn);
}

struct node *f(node *list, node *odd_list, size_t (*fn)(size_t)) {
    if (!list) {
        return odd_list;
    }

    size_t el = list->el;
    size_t r = fn(el);
    if (r != 0) {
        odd_list = add_element(odd_list, el);
    }

    return f(list->prev, odd_list, fn);
}

void free_stack(node *s) {
    if (!s) {
        return;
    }
    free_stack(s->prev);
    free(s);
}

int main() {
    size_t data_length = sizeof(data) / sizeof(size_t);
    size_t i = data_length;

    node *list = NULL;

    while (i > 0) {
        list = add_element(list, data[i - 1]);
        i--;
    };

    m(list, print_int);
    puts(empty_str);

    node *odd_list = NULL;
    odd_list = f(list, odd_list, p);

    m(odd_list, print_int);
    puts(empty_str);

    free_stack(list);
    free_stack(odd_list);

    return 0;
}
