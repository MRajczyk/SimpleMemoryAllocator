#ifndef HEAP_H
#define HEAP_H

#include "custom_unistd.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

struct my_header{
    struct my_header* next;
    struct my_header* prev;
    unsigned long size;
    uint8_t free;
    int fileline;
    const char* filename;
    size_t control_sum;
}__attribute__((__packed__));
typedef struct my_header mem_header;

#define PAGE_SIZE 4096
#define FENCE_SIZE 4
#define WORD_LEN sizeof(void*)
#define HEADER_FENCE_SIZE(size) (sizeof(mem_header) + 2 * FENCE_SIZE + (size))
#define CONTROL_SIZE sizeof(mem_header) - sizeof(size_t)
#define IS_POINTER_DIVISIBLE_BY_WORD(ptr) ((intptr_t)(ptr) & (intptr_t)(WORD_LEN - 1)) == 0
#define IS_POINTER_DIVISIBLE_BY_4096(ptr) ((intptr_t)(ptr) & (intptr_t)(PAGE_SIZE - 1)) == 0
#define ALIGN(x,a) (((x)/(a)+((x)%(a) != 0))*(a))

enum pointer_type_t
{
    pointer_null,
    pointer_heap_corrupted,
    pointer_control_block,
    pointer_inside_fences,
    pointer_inside_data_block,
    pointer_unallocated,
    pointer_valid
};

void draw_fences(mem_header* address);
size_t calculate_control_size(uint8_t* ptr);
void header_setup(mem_header* header, unsigned long size, mem_header* prev, mem_header* next);
void header_setup_debug(mem_header* header, unsigned long size, mem_header* prev, mem_header* next, int fileline, const char* filename);
int heap_setup(void);
void heap_clean(void);
void* heap_malloc(size_t size);
void* heap_calloc(size_t number, size_t size);
void* heap_realloc(void* memblock, size_t size);
mem_header* concat_memory_blocks(mem_header* p1, mem_header* p2);
void  heap_free(void* memblock);
size_t heap_get_largest_used_block_size(void);
enum pointer_type_t get_pointer_type(const void* const pointer);
int heap_validate(void);
void* heap_malloc_aligned(size_t size);
void* heap_calloc_aligned(size_t number, size_t size);
void* heap_realloc_aligned(void* memblock, size_t size);
void* heap_malloc_debug(size_t count, int fileline, const char* filename);
void* heap_calloc_debug(size_t number, size_t size, int fileline, const char* filename);
void* heap_realloc_debug(void* memblock, size_t size, int fileline, const char* filename);
void* heap_malloc_aligned_debug(size_t count, int fileline, const char* filename);
void* heap_calloc_aligned_debug(size_t number, size_t size, int fileline, const char* filename);
void* heap_realloc_aligned_debug(void* memblock, size_t size, int fileline, const char* filename);

#endif //HEAP_H
