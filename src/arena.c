#include <stdint.h>
#include <stdlib.h>

#include "arena.h"

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#define is_power_two(x) ((x != 0) && ((x & (x - 1)) == 0))

static uintptr_t align_forward(uintptr_t ptr, size_t alignment) {
    if (!is_power_two(alignment)) return 0;

    uintptr_t p = ptr;
    uintptr_t a = (uintptr_t)alignment;
    uintptr_t modulo = p & (a - 1);

    if (modulo) p += a - modulo;

    return p;
}

Arena *arena_init(size_t capacity) {
    Arena *arena = malloc(sizeof(Arena));
    if (arena == NULL) return NULL;

    arena->data = malloc(capacity);
    if (arena->data == NULL) {
        free(arena);
        return NULL;
    }

    arena->capacity = capacity;
    arena->size = 0;
    return arena;
}

void arena_free(Arena *arena) {
    free(arena->data);
    free(arena);
}

void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment) {
    uintptr_t current = (uintptr_t)arena->data + (uintptr_t)arena->size;
    uintptr_t offset = align_forward(current, alignment);

    offset -= (uintptr_t)arena->data;
    if (offset + size > arena->capacity) return NULL;

    void *ptr = arena->data + offset;
    arena->size = offset + size;

    return ptr;
}

void *arena_alloc(Arena *arena, size_t size) {
    if (!size) return NULL;
    return arena_alloc_aligned(arena, size, DEFAULT_ALIGNMENT);
}

void arena_clear(Arena *arena) {
    arena->size = 0;
}