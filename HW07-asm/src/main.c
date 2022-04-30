#include <stdio.h>
#include <stdlib.h>

/*
 * Finding structure is `stack`
 */

static const size_t data[] = {4, 8, 15, 16, 23, 42,};
static const char empty_str[] = {0x00};
static const char int_format[] = "%ld ";

size_t p(size_t el) {
    return el % 2;
}

size_t *add_element(size_t *stack, size_t el) {
    size_t *new_cell = malloc(16);
    if (!new_cell) {
        abort();
    }

    *new_cell = el;

    *((size_t **) new_cell + 1) = stack;
    return new_cell;
}

void print_int(size_t el) {
    printf(int_format, el);
    fflush(0);
}

// asm `m` function
void m(const size_t *stack, void (*print_fn)(size_t)) {
    if (!stack) {
        return;
    }

    print_fn(*stack);
    m(*((size_t **) stack + 1), print_fn);
}

size_t *f(const size_t *stack, size_t *odd_stack, size_t (*fn)(size_t)) {
    if (!stack) {
        return odd_stack;
    }

    size_t el = *stack;
    size_t r = fn(el);
    if (r != 0) {
        odd_stack = add_element(odd_stack, el);
    }

    return f(*((size_t **) stack + 1), odd_stack, fn);
}

void free_stack(size_t *s) {
    if (!s) {
        return;
    }
    free_stack(*((size_t **) s + 1));
    free(s);
}

int main() {
    size_t data_length = sizeof(data) / sizeof(size_t);
    size_t i = data_length;

    size_t *stack = NULL;

    while (i > 0) {
        stack = add_element(stack, data[i - 1]);
        i--;
    };

    m(stack, print_int);
    puts(empty_str);

    size_t *odd_stack = NULL;
    odd_stack = f(stack, odd_stack, p);

    m(odd_stack, print_int);
    puts(empty_str);

    free_stack(stack);
    free_stack(odd_stack);

    return 0;
}
