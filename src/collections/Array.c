#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/Array.h"
#include "../general_collection_helper.h"
#include "../../include/helper.h"
#include "../list_helper.h"
#include "../../lib/obsidian.h"

/*
    valid values for WIDTH
    - (0) no spaces i.e. `|a||b||c||d|`
    - (x > 0) spaces i.e. for x == 1 `|a| |b| |c| |d|`
    - (x == -1) no spaces and no double bars i.e. `|a|b|c|d|`
*/
#define WIDTH (-1)
#define ELLIPSES (" [ ... ] ")
#define ELLIPSES_LEN (strlen(ELLIPSES))

void array_print(Collection c);

Array array_new(char *name, size_t size) {
    Array array = malloc_with_oom(sizeof(struct _array_t), "Array");
    array->data = malloc_with_oom(sizeof(struct _array_data_t) * size, "Array Nodes");
    array->len = size;
    array->get_sizeof = list_sizeof;
    array->node_printer = list_print_node;
    array->list_printer = array_print;
    array->vert_len = DEFAULT_PRINT_HEIGHT;
    array->name = name;
    return array;
}

void array_free(Array array) {
    free(array->data);
    free(array);
}

ArrayNode array_at(Array array, size_t index) {
    if (array->len <= index) return NULL;
    return &array->data[index];
}

struct _array_data_t array_new_node(Data data, TypeTag type) {
    return (struct _array_data_t) {
        .data = data,
        .ptr = NULL,
        .data_tag = type,
    };
}

void array_resize(Array array, size_t new_size) {
    array->data = realloc(array->data, new_size);
    array->len = new_size;
}

void array_set(Array array, size_t index, struct _array_data_t node) {
    obs_assert(array->len, >, index);
    array->data[index] = node;
}

size_t array_length(Array array) {
    return array->len;
}

size_t get_sizes(Array array, size_t max, size_t **node_sizes, size_t *out_actual_len) {
    size_t count = 0;
    *out_actual_len = 0;
    *node_sizes = array->len == 0 ? NULL : malloc_with_oom(sizeof(size_t) * array->len, "Node Sizes");

    // same idea as DLL, go both ways
    size_t i;
    for (i = 0; i < (array->len + 1) / 2; i++) {
        size_t forward_size = array->get_sizeof(array_at(array, i));
        if (forward_size + count + WIDTH > max) break;
        (*node_sizes)[i] = forward_size;
        count += forward_size + WIDTH;
        (*out_actual_len)++;

        if (i == array->len / 2) break;

        size_t backward_size = array->get_sizeof(array_at(array, array->len - 1 - i));
        if (backward_size + count + WIDTH > max) break;
        (*node_sizes)[array->len - 1 - i] = backward_size;
        count += backward_size + WIDTH;
        (*out_actual_len)++;
    }

    // to discount the last one
    if (i > 0) count -= WIDTH;

    if (i != array->len / 2) count += ELLIPSES_LEN;

    return count;
}

void array_print(Collection c) {
    Array array = (Array)c;
    terminalSize size = get_terminal_size();
    size_t *node_sizes;
    size_t actual_len;
    size_t count = get_sizes(array, size.width, &node_sizes, &actual_len);

    char **buf = malloc_with_oom(sizeof(char*) * (c->vert_len + DEFAULT_PTR_HEIGHT), "Buffer");
    for (int i = 0; i < c->vert_len + DEFAULT_PTR_HEIGHT; i++) {
        buf[i] = malloc_with_oom((count + 1) * sizeof(char), "Buffer");
        for (int j = 0; j < count; j++) buf[i][j] = ' ';
        buf[i][count] = '\0';
    }

    size_t front_len = actual_len == array->len ? actual_len : (actual_len + 1) / 2;
    size_t offset = 0;
    for (size_t i = 0; i < front_len; i++) {
        if (i != 0) {
            write_str_repeat_char_grid(buf, offset, ' ', c->vert_len, WIDTH, 0);
            offset += WIDTH;
        }
        list_print_node(array_at(array, i), buf, node_sizes[i], c->vert_len, offset);
        offset += node_sizes[i];
    }

    if (front_len != actual_len) {
        // do middle/ellipses
        write_str_center_incr(buf, &offset, c->vert_len, ELLIPSES, ELLIPSES_LEN);

        // do back side
        for (size_t i = array->len - 1; i >= array->len - actual_len; i--) {
            write_str_repeat_char_grid(buf, offset, ' ', c->vert_len, WIDTH, 0);
            offset += WIDTH;
            list_print_node(array_at(array, i), buf, node_sizes[i], c->vert_len, offset);
            offset += node_sizes[i];
        }
    }

    printf("Array: %s\n", c->name);
    for (int i = 0; i < c->vert_len; i++) {
        printf("%s\n", buf[i]);
        free(buf[i]);
    }
    for (int i = c->vert_len; i < DEFAULT_PTR_HEIGHT + c->vert_len; i++) {
        bool found_non_space = false;
        for (int j = 0; j < count; j++) {
            if (buf[i][j] != ' ') {
                found_non_space = true;
                break;
            }
        }
        if (found_non_space) printf("%s\n", buf[i]);
        free(buf[i]);
    }

    obs_assert(offset, ==, count);
    printf("\n");

    free(buf);
    free(node_sizes);
}