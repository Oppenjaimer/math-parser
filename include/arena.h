#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t size;
} Arena;

Arena *arena_init(size_t capacity);
void arena_free(Arena *arena);
void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment);
void *arena_alloc(Arena *arena, size_t size);
void arena_clear(Arena *arena);

#endif