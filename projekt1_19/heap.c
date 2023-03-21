#include "heap.h"

void* heap_start = NULL;
mem_header* first_block = NULL;
uint8_t heap_is_empty = 1;
unsigned long pages_allocated = 0;
unsigned long header_size = sizeof(mem_header);
pthread_mutex_t mutex;


void draw_fences(mem_header* address)
{
    for(int i = 0; i < FENCE_SIZE; ++i)
        *(char*)((uint8_t*)address + header_size + i) = 'f';
    for(int i = 0; i < FENCE_SIZE; ++i)
        *(char*)((uint8_t*)address + header_size + FENCE_SIZE + address->size + i) = 'F';
}

size_t calculate_control_size(uint8_t* ptr)
{
    size_t control_sum = 0;
    for(size_t i = 0; i < CONTROL_SIZE; ++i)
        control_sum += *(ptr + i);
    return control_sum;
}

void header_setup(mem_header* header, unsigned long size, mem_header* prev, mem_header* next)
{
    header->size = size;
    header->free = 0;
    header->next = next;
    header->prev = prev;
    header->control_sum = calculate_control_size((uint8_t*)header);

    draw_fences(header);

    if(prev)
    {
        prev->next = header;
        prev->control_sum = calculate_control_size((uint8_t*)prev);
    }
    if(next)
    {
        next->prev = header;
        next->control_sum = calculate_control_size((uint8_t*)next);
    }
}

void header_setup_debug(mem_header* header, unsigned long size, mem_header* prev, mem_header* next, int fileline, const char* filename)
{
    header->size = size;
    header->free = 0;
    header->next = next;
    header->prev = prev;
    header->fileline = fileline;
    header->filename = filename;
    header->control_sum = calculate_control_size((uint8_t*)header);

    draw_fences(header);

    if(prev)
    {
        prev->next = header;
        prev->control_sum = calculate_control_size((uint8_t*)prev);
    }
    if(next)
    {
        next->prev = header;
        next->control_sum = calculate_control_size((uint8_t*)next);
    }
}

int heap_setup(void)
{
    heap_start = custom_sbrk(PAGE_SIZE);
    if(heap_start == (void*)-1)
        return -1;
    pages_allocated = 1;
    pthread_mutex_init(&mutex, NULL);

    return 0;
}

void heap_clean(void)
{
    if(heap_start == NULL)
        return;
    unsigned long memory_used = pages_allocated * PAGE_SIZE;
    custom_sbrk((-1) * memory_used);
    heap_start = NULL;
    first_block = NULL;
    heap_is_empty = 1;
    pthread_mutex_destroy(&mutex);
}

void* heap_malloc(size_t size)
{
    if(!size || heap_validate() || !heap_start)
    {
        return NULL;
    }
    pthread_mutex_lock(&mutex);

    if(heap_start && heap_is_empty == 1)  //empty heap
    {
        size_t offset = ALIGN((size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE);
        heap_is_empty = 0;
        while(pages_allocated * PAGE_SIZE < size + offset)
        {
            void* temp = custom_sbrk(PAGE_SIZE);
            if(temp == (void*)-1)
            {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            else
                ++pages_allocated;
        }

        first_block = (mem_header*)((uint8_t*)heap_start + offset);
        mem_header* first_block_allocated = first_block;
        header_setup(first_block_allocated, size, NULL, NULL);
        pthread_mutex_unlock(&mutex);
        return (void*)((uint8_t*)first_block_allocated + FENCE_SIZE + header_size);
    }

    mem_header* temp = first_block;
    while(temp)     // look for free blocks with enough size and right address
    {
        size_t offset = ALIGN((size_t)((uint8_t*)temp + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + header_size + FENCE_SIZE);
        if(temp->free && temp->size >= size + offset)
        {
            size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + header_size + FENCE_SIZE);
            if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + offset_new_block + HEADER_FENCE_SIZE(1)) < (uintptr_t)temp->next)
            {
                mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + offset_new_block);
                header_setup(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + offset_new_block) - header_size - 2 * FENCE_SIZE, temp, temp->next);
                new_block->free = 1;
                new_block->control_sum = calculate_control_size((uint8_t*)new_block);
                header_setup(temp, size, temp->prev, new_block);
            }
            else
                header_setup(temp, size, temp->prev, temp->next);
            pthread_mutex_unlock(&mutex);
            return (void*)((uint8_t*)temp + header_size + FENCE_SIZE);
        }
        if(temp->next)
            temp = temp->next;
        else        //if not present, see how much memory is free, and if not sufficient, request OS for more. check the result and then create new header
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            if(IS_POINTER_DIVISIBLE_BY_WORD((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE))
            {
                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size));
                header_setup(temp->next, size, temp, NULL);

                if(temp->free)
                {
                    header_setup(temp, temp->size, temp->prev, temp->next);
                    temp->free = 1;
                    temp->control_sum = calculate_control_size((uint8_t*)temp);
                }
                else
                    header_setup(temp, temp->size, temp->prev, temp->next);

                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + header_size + FENCE_SIZE);
            }
            else
            {
                offset = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE);

                while(free_memory_on_heap < HEADER_FENCE_SIZE(size) + offset)
                {
                    void* res = custom_sbrk(PAGE_SIZE);
                    if(res == (void*)-1)
                    {
                        pthread_mutex_unlock(&mutex);
                        return NULL;
                    }
                    else
                    {
                        free_memory_on_heap += PAGE_SIZE;
                        ++pages_allocated;
                    }
                }
                if(temp->free)
                {
                    header_setup(temp, temp->size, temp->prev, temp->next);
                    temp->free = 1;
                    temp->control_sum = calculate_control_size((uint8_t*)temp);
                }
                else
                    header_setup(temp, temp->size, temp->prev, temp->next);

                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset);
                header_setup(temp->next, size, temp, NULL);

                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + FENCE_SIZE + header_size);
            }
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* heap_calloc(size_t number, size_t size)
{
    if(!number || !size || heap_validate())
    {
        return NULL;
    }

    void* ptr = heap_malloc(number * size);
    pthread_mutex_lock(&mutex);
    if(ptr)
    {
        memset(ptr, 0, number * size);
        pthread_mutex_unlock(&mutex);
        return ptr;
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* heap_realloc(void* memblock, size_t size)
{
    if((!memblock && !size) || heap_validate())
    {
        return NULL;
    }
    if(size == 0)
    {
        heap_free(memblock);
        return NULL;
    }
    if(!memblock)
    {
        return heap_malloc(size);
    }

    if(get_pointer_type(memblock) != pointer_valid)
    {
        return NULL;
    }

    pthread_mutex_lock(&mutex);

    mem_header* temp = (mem_header*)((uint8_t*)memblock - FENCE_SIZE - header_size);
    if(temp->size == size)    // new size == old size, nothing changes
    {
        pthread_mutex_unlock(&mutex);
        return memblock;
    }

    if(temp->size > size)     // new size < old size, shrink the block and update control sum
    {
        size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE);
        if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + HEADER_FENCE_SIZE(1) + offset_new_block) < (uintptr_t)temp->next)
        {
            mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block);
            header_setup(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block) - header_size - 2 * FENCE_SIZE, temp, temp->next);
            new_block->free = 1;
            new_block->control_sum = calculate_control_size((uint8_t*)new_block);
            header_setup(temp, size, temp->prev, new_block);
        }
        else
            header_setup(temp, size, temp->prev, temp->next);

        pthread_mutex_unlock(&mutex);
        return memblock;
    }
    else
    {
        if(temp->next)         //check whether next block exists
        {
            // if so, check if its' size + size of curr block is enough to fit the new block
            if(temp->next->free && (unsigned long)(((uint8_t*)temp->next + HEADER_FENCE_SIZE(temp->next->size) - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup(temp, size, temp->prev, temp->next->next);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else if((unsigned long)(((uint8_t*)temp->next - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup(temp, size, temp->prev, temp->next);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else                   // if not, there is a need to change the block's location
            {
                pthread_mutex_unlock(&mutex);
                void *new_block_location = heap_malloc(size);
                pthread_mutex_lock(&mutex);
                if(!new_block_location)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }

                memcpy(new_block_location, memblock, temp->size);
                pthread_mutex_unlock(&mutex);
                heap_free((uint8_t*)temp + header_size + FENCE_SIZE);
                pthread_mutex_lock(&mutex);
                ((mem_header*)((uint8_t*)new_block_location - header_size - FENCE_SIZE))->control_sum = calculate_control_size((uint8_t*)new_block_location - header_size - FENCE_SIZE);
                pthread_mutex_unlock(&mutex);
                return new_block_location;
            }
        }
        else
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            header_setup(temp, size, temp->prev, NULL);
            pthread_mutex_unlock(&mutex);
            return memblock;
        }
    }
}

mem_header* concat_memory_blocks(mem_header* p1, mem_header* p2)
{
    p1->next = p2->next;
    if(p2->next)
        p2->next->prev = p1;
    p1->size = p1->size + p2->size + header_size;
    return p1;
}


void heap_free(void* memblock)
{
    if(!memblock || get_pointer_type(memblock) != pointer_valid)
    {
        return;
    }
    pthread_mutex_lock(&mutex);

    mem_header* header = (mem_header*)((uint8_t*)memblock - FENCE_SIZE - header_size);
    header->free = 1;

    if(header->prev && header->prev->free)
        header = concat_memory_blocks(header->prev, header);
    if(header->next && header->next->free)
        header = concat_memory_blocks(header, header->next);
    if(header->next)
        header->size = (uint8_t*)header->next - (uint8_t*)header - HEADER_FENCE_SIZE(0);

    draw_fences((void*)header);
    if(header->prev)
        header->prev->control_sum = calculate_control_size((uint8_t*)header->prev);
    header->control_sum = calculate_control_size((uint8_t*)header);
    if(header->next)
        header->next->control_sum = calculate_control_size((uint8_t*)header->next);

    pthread_mutex_unlock(&mutex);
}

size_t heap_get_largest_used_block_size(void)
{
    if(!heap_start || heap_is_empty || heap_validate())
        return 0;

    size_t max_size = 0;
    mem_header* temp = first_block;

    while(temp)
    {
        if(temp->free == 0)
        {
            if(max_size < temp->size)
                max_size = temp->size;
        }
        temp = temp->next;
    }

    return max_size;
}

enum pointer_type_t get_pointer_type(const void* const pointer)
{
    if(!pointer)
        return pointer_null;
    if(heap_validate())
        return pointer_heap_corrupted;

    intptr_t ptr_handle = (intptr_t)pointer;
    if(ptr_handle < (intptr_t)heap_start)
        return pointer_unallocated;
    else if(ptr_handle < (intptr_t)((uint8_t*)first_block + header_size))
        return pointer_control_block;

    mem_header* temp = first_block;

    while(temp->next && (intptr_t)temp->next <= ptr_handle)
        temp = temp->next;
    if(ptr_handle < (intptr_t)((uint8_t*)temp + header_size))
        return pointer_control_block;
    else if(ptr_handle < (intptr_t)((uint8_t*)temp + header_size + FENCE_SIZE) && temp->free == 0)
        return pointer_inside_fences;
    else if(ptr_handle < (intptr_t)((uint8_t*)temp + header_size + FENCE_SIZE) && temp->free == 1)
        return pointer_unallocated;
    else if(ptr_handle == (intptr_t)((uint8_t*)temp + header_size + FENCE_SIZE) && temp->free == 0)
        return pointer_valid;
    else if(ptr_handle == (intptr_t)((uint8_t*)temp + header_size + FENCE_SIZE))
        return pointer_unallocated;
    else if(ptr_handle < (intptr_t)((uint8_t*)temp + header_size + FENCE_SIZE + temp->size) && temp->free == 0)
        return pointer_inside_data_block;
    else if(ptr_handle < (intptr_t)((uint8_t*)temp + header_size + FENCE_SIZE + temp->size))
        return pointer_unallocated;
    else if(ptr_handle < (intptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) && temp->free == 0)
        return pointer_inside_fences;
    else if(ptr_handle >= (intptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) - FENCE_SIZE) && temp->free == 0 && ptr_handle < (intptr_t)temp->next)
        return pointer_inside_fences;

    return pointer_unallocated;
}

int heap_validate(void)
{
    // check if heap initialized
    if(heap_start == NULL)
    {
        return 2;              // return value 2 == HEAP_UNINITIALIZED
    }
    ///////////////////////////
    if(heap_is_empty)
    {
        return 0;              // return value 0 == HEAP_OK
    }
    pthread_mutex_lock(&mutex);
    mem_header* i = first_block;
    while(i)
    {
        // check control sum //
        size_t temp_ctr_sum = calculate_control_size((uint8_t*)i);

        if(temp_ctr_sum != i->control_sum)
        {
            pthread_mutex_unlock(&mutex);
            return 3;          // return value 3 == HEAP_CONTROL_STRUCTURES_CORRUPTED
        }

        //////////////////////
        // check fences integrity //
        for(int j = 0; j < FENCE_SIZE; ++j)
        {
            if(*(char*)((uint8_t*)i + header_size + j) != 'f')
            {
                pthread_mutex_unlock(&mutex);
                return 1;      // return value 1 == FENCES_CORRUPTED
            }
        }
        for(int j = 0; j < FENCE_SIZE; ++j)
        {
            if(*(char*)((uint8_t*)i + header_size + FENCE_SIZE + i->size + j) != 'F')
            {
                pthread_mutex_unlock(&mutex);
                return 1;      // return value 1 == FENCES_CORRUPTED
            }
        }
        ////////////////////////////
        i = i->next;
    }

    pthread_mutex_unlock(&mutex);
    return 0;       // return value 0 == HEAP_OK
}

void* heap_malloc_aligned(size_t size)
{
    if(!size || heap_validate() || !heap_start)
    {
        return NULL;
    }
    pthread_mutex_lock(&mutex);

    if(heap_start && heap_is_empty == 1)  //empty heap
    {
        size_t offset_page = ALIGN((size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE), PAGE_SIZE) - (size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE);
        size_t offset_word = ALIGN((size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE);
        heap_is_empty = 0;
        while(pages_allocated * PAGE_SIZE < HEADER_FENCE_SIZE(size) + offset_page)
        {
            void* temp = custom_sbrk(PAGE_SIZE);
            if(temp == (void*)-1)
            {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            else
                ++pages_allocated;
        }

        first_block = (mem_header*)((uint8_t*)heap_start + offset_word);
        mem_header* first_block_allocated = first_block;     //first block will be free, it fits in the offset area of the block aligned for page size
        mem_header* second_block = (mem_header*)((uint8_t*)heap_start + offset_page);  //the second is aligned to the page and with size given by user

        header_setup(first_block_allocated, offset_page - offset_word - header_size - 2 * FENCE_SIZE, NULL, second_block);
        first_block_allocated->free = 1;
        first_block_allocated->control_sum = calculate_control_size((uint8_t*)first_block_allocated);
        header_setup(second_block, size, first_block, NULL);

        pthread_mutex_unlock(&mutex);
        return (void*)((uint8_t*)second_block + FENCE_SIZE + header_size);
    }

    mem_header* temp = first_block;
    while(temp)     // look for free blocks with enough size and right address
    {
        size_t offset = ALIGN((size_t)((uint8_t*)temp + header_size + FENCE_SIZE), PAGE_SIZE) - (size_t)((uint8_t*)temp + header_size + FENCE_SIZE);
        if(temp->free && temp->size >= HEADER_FENCE_SIZE(size) + offset)
        {
            if(offset == 0)
            {
                size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE);
                if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block + HEADER_FENCE_SIZE(1)) < (uintptr_t)temp->next)
                {
                    mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block);
                    header_setup(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block) - header_size - 2 * FENCE_SIZE, temp, temp->next);
                    new_block->free = 1;
                    new_block->control_sum = calculate_control_size((uint8_t*)new_block);
                    temp->next->prev = new_block;
                    temp->next->control_sum = calculate_control_size((uint8_t*)temp->next);
                    header_setup(temp, size, temp->prev, new_block);
                }
                else
                    header_setup(temp, size, temp->prev, temp->next);
                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp + header_size + FENCE_SIZE);
            }
            else
            {
                if(offset > HEADER_FENCE_SIZE(1))
                {
                    mem_header* new_block = (mem_header*)((uint8_t*)temp + offset);
                    header_setup(new_block, temp->size - offset - HEADER_FENCE_SIZE(0), temp, temp->next);
                    new_block->free = 1;
                    new_block->control_sum = calculate_control_size((uint8_t*)new_block);
                    temp->next->prev = new_block;
                    temp->next->control_sum = calculate_control_size((uint8_t*)temp->next);
                    header_setup(temp, offset - header_size - 2 * FENCE_SIZE, temp->prev, new_block);
                    temp->free = 1;
                    temp->control_sum = calculate_control_size((uint8_t*)temp);

                    temp = temp->next;
                    continue;
                }
                else
                {
                    temp = temp->next;
                    continue;
                }
            }
        }
        if(temp->next)
            temp = temp->next;
        else        //if not present, see how much memory is free, and if not sufficient, request OS for more. check the result and then create new header
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            if(IS_POINTER_DIVISIBLE_BY_4096((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE))
            {
                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size));
                header_setup(temp->next, size, temp, NULL);
                header_setup(temp, temp->size, temp->prev, temp->next);
                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + header_size + FENCE_SIZE);
            }
            else
            {
                offset = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE), PAGE_SIZE) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE);
                while(free_memory_on_heap < HEADER_FENCE_SIZE(size) + offset)
                {
                    void* res = custom_sbrk(PAGE_SIZE);
                    if(res == (void*)-1)
                    {
                        pthread_mutex_unlock(&mutex);
                        return NULL;
                    }
                    else
                    {
                        free_memory_on_heap += PAGE_SIZE;
                        ++pages_allocated;
                    }
                }
                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset);

                size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE);
                if(offset > HEADER_FENCE_SIZE(1) + offset_new_block)
                {
                    mem_header* new_block_empty = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset_new_block);
                    mem_header* new_block_allocated = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset);
                    header_setup(new_block_empty, offset - offset_new_block - HEADER_FENCE_SIZE(0), temp, new_block_allocated);
                    new_block_empty->free = 1;
                    new_block_empty->control_sum = calculate_control_size((uint8_t*)new_block_empty);
                    header_setup(new_block_allocated, size, new_block_empty, NULL);
                    if(temp->free)
                    {
                        header_setup(temp, temp->size, temp->prev, new_block_empty);
                        temp->free = 1;
                        temp->control_sum = calculate_control_size((uint8_t*)temp);
                    }
                    else
                        header_setup(temp, temp->size, temp->prev, new_block_empty);
                    pthread_mutex_unlock(&mutex);
                    return (void*)((uint8_t*)new_block_allocated + FENCE_SIZE + header_size);
                }
                header_setup(temp->next, size, temp, NULL);
                if(temp->free)
                {
                    header_setup(temp, temp->size, temp->prev, temp->next);
                    temp->free = 1;
                    temp->control_sum = calculate_control_size((uint8_t*)temp);
                }
                else
                    header_setup(temp, temp->size, temp->prev, temp->next);

                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + FENCE_SIZE + header_size);
            }
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* heap_calloc_aligned(size_t number, size_t size)
{
    if(!number || !size || heap_validate())
    {
        return NULL;
    }

    void* ptr = heap_malloc_aligned(number * size);
    pthread_mutex_lock(&mutex);
    if(ptr)
    {
        memset(ptr, 0, number * size);
        pthread_mutex_unlock(&mutex);
        return ptr;
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* heap_realloc_aligned(void* memblock, size_t size)
{
    if((!memblock && !size) || heap_validate())
    {
        return NULL;
    }

    if(size == 0)
    {
        heap_free(memblock);
        return NULL;
    }
    if(!memblock)
    {
        return heap_malloc_aligned(size);
    }
    if(get_pointer_type(memblock) != pointer_valid)
    {
        return NULL;
    }
    pthread_mutex_lock(&mutex);

    mem_header* temp = (mem_header*)((uint8_t*)memblock - FENCE_SIZE - header_size);
    if(temp->size == size)    // new size == old size, nothing changes
    {
        pthread_mutex_unlock(&mutex);
        return memblock;
    }
    if(temp->size > size)     // new size < old size, shrink the block and update control sum
    {
        if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + HEADER_FENCE_SIZE(1)) < (uintptr_t)temp->next)
        {
            mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size));
            header_setup(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size)) - header_size - 2 * FENCE_SIZE, temp, temp->next);
            new_block->free = 1;
            new_block->control_sum = calculate_control_size((uint8_t*)new_block);
            header_setup(temp, size, temp->prev, new_block);
        }
        else
            header_setup(temp, size, temp->prev, temp->next);

        pthread_mutex_unlock(&mutex);
        return memblock;
    }
    else
    {
        if(temp->next)         //check whether next block exists
        {
            // if so, check if its' size + size of curr block is enough to fit the new block
            if(temp->next->free && (unsigned long)(((uint8_t*)temp->next + HEADER_FENCE_SIZE(temp->next->size) - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup(temp, size, temp->prev, temp->next->next);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else if((unsigned long)(((uint8_t*)temp->next - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup(temp, size, temp->prev, temp->next);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else                   // if not, there is a need to change the block's location
            {
                pthread_mutex_unlock(&mutex);
                void *new_block_location = heap_malloc_aligned(size);
                pthread_mutex_lock(&mutex);
                if(!new_block_location)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }

                memcpy(new_block_location, memblock, temp->size);
                pthread_mutex_unlock(&mutex);
                heap_free((uint8_t*)temp + header_size + FENCE_SIZE);
                pthread_mutex_lock(&mutex);
                ((mem_header*)((uint8_t*)new_block_location - header_size - FENCE_SIZE))->control_sum = calculate_control_size((uint8_t*)new_block_location - header_size - FENCE_SIZE);
                pthread_mutex_unlock(&mutex);
                return new_block_location;
            }
        }
        else
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            header_setup(temp, size, temp->prev, NULL);
            pthread_mutex_unlock(&mutex);
            return memblock;
        }
    }
}


void* heap_malloc_debug(size_t size, int fileline, const char* filename)
{
    if(!size || heap_validate() || !heap_start)
    {
        return NULL;
    }

    pthread_mutex_lock(&mutex);
    if(heap_start && heap_is_empty == 1)  //empty heap
    {
        size_t offset = ALIGN((size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE);
        heap_is_empty = 0;
        while(pages_allocated * PAGE_SIZE < size + offset)
        {
            void* temp = custom_sbrk(PAGE_SIZE);
            if(temp == (void*)-1)
            {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            else
                ++pages_allocated;
        }

        first_block = (mem_header*)((uint8_t*)heap_start + offset);
        mem_header* first_block_allocated = first_block;
        header_setup_debug(first_block_allocated, size, NULL, NULL, fileline, filename);
        pthread_mutex_unlock(&mutex);
        return (void*)((uint8_t*)first_block_allocated + FENCE_SIZE + header_size);
    }

    mem_header* temp = first_block;
    while(temp)     // look for free blocks with enough size and right address
    {
        size_t offset = ALIGN((size_t)((uint8_t*)temp + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + header_size + FENCE_SIZE);
        if(temp->free && temp->size >= size + offset)
        {
            size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + header_size + FENCE_SIZE);
            if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + offset_new_block + HEADER_FENCE_SIZE(1)) < (uintptr_t)temp->next)
            {
                mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + offset_new_block);
                header_setup_debug(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset + offset_new_block) - header_size - 2 * FENCE_SIZE, temp, temp->next, fileline, filename);
                new_block->free = 1;
                new_block->control_sum = calculate_control_size((uint8_t*)new_block);
                header_setup_debug(temp, size, temp->prev, new_block, fileline, filename);
            }
            else
                header_setup_debug(temp, size, temp->prev, temp->next, fileline, filename);
            pthread_mutex_unlock(&mutex);
            return (void*)((uint8_t*)temp + header_size + FENCE_SIZE);
        }
        if(temp->next)
            temp = temp->next;
        else        //if not present, see how much memory is free, and if not sufficient, request OS for more. check the result and then create new header
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            if(IS_POINTER_DIVISIBLE_BY_WORD((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE))
            {
                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size));
                header_setup_debug(temp->next, size, temp, NULL, fileline, filename);
                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + header_size + FENCE_SIZE);
            }
            else
            {
                offset = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE);

                while(free_memory_on_heap < HEADER_FENCE_SIZE(size) + offset)
                {
                    void* res = custom_sbrk(PAGE_SIZE);
                    if(res == (void*)-1)
                    {
                        pthread_mutex_unlock(&mutex);
                        return NULL;
                    }
                    else
                    {
                        free_memory_on_heap += PAGE_SIZE;
                        ++pages_allocated;
                    }
                }
                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset);
                header_setup_debug(temp->next, size, temp, NULL, fileline, filename);

                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + FENCE_SIZE + header_size);
            }
        }
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void* heap_calloc_debug(size_t number, size_t size, int fileline, const char* filename)
{
    if(!number || !size || heap_validate())
    {
        return NULL;
    }

    void* ptr = heap_malloc_debug(number * size, fileline, filename);
    pthread_mutex_lock(&mutex);
    if(ptr)
    {
        memset(ptr, 0, number * size);
        pthread_mutex_unlock(&mutex);
        return ptr;
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}
void* heap_realloc_debug(void* memblock, size_t size, int fileline, const char* filename)
{
    if((!memblock && !size) || heap_validate())
    {
        return NULL;
    }

    if(size == 0)
    {
        heap_free(memblock);
        return NULL;
    }
    if(!memblock)
    {
        return heap_malloc_debug(size, fileline, filename);
    }
    if(get_pointer_type(memblock) != pointer_valid)
    {
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    mem_header* temp = (mem_header*)((uint8_t*)memblock - FENCE_SIZE - header_size);
    if(temp->size == size)    // new size == old size, nothing changes
    {
        pthread_mutex_unlock(&mutex);
        return memblock;
    }
    if(temp->size > size)     // new size < old size, shrink the block and update control sum
    {
        size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE);
        if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + HEADER_FENCE_SIZE(1) + offset_new_block) < (uintptr_t)temp->next)
        {
            mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block);
            header_setup_debug(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block) - header_size - 2 * FENCE_SIZE, temp, temp->next, fileline, filename);
            new_block->free = 1;
            new_block->control_sum = calculate_control_size((uint8_t*)new_block);
            header_setup_debug(temp, size, temp->prev, new_block, fileline, filename);
        }
        else
            header_setup_debug(temp, size, temp->prev, temp->next, fileline, filename);

        pthread_mutex_unlock(&mutex);
        return memblock;
    }
    else
    {
        if(temp->next)         //check whether next block exists
        {
            // if so, check if its' size + size of curr block is enough to fit the new block
            if(temp->next->free && (unsigned long)(((uint8_t*)temp->next + HEADER_FENCE_SIZE(temp->next->size) - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup_debug(temp, size, temp->prev, temp->next->next, fileline, filename);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else if((unsigned long)(((uint8_t*)temp->next - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup_debug(temp, size, temp->prev, temp->next, fileline, filename);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else                   // if not, there is a need to change the block's location
            {
                pthread_mutex_unlock(&mutex);
                void *new_block_location = heap_malloc_debug(size, fileline, filename);
                pthread_mutex_lock(&mutex);
                if(!new_block_location)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }

                memcpy(new_block_location, memblock, temp->size);
                pthread_mutex_unlock(&mutex);
                heap_free((uint8_t*)temp + header_size + FENCE_SIZE);
                pthread_mutex_lock(&mutex);
                ((mem_header*)((uint8_t*)new_block_location - header_size - FENCE_SIZE))->control_sum = calculate_control_size((uint8_t*)new_block_location - header_size - FENCE_SIZE);
                pthread_mutex_unlock(&mutex);
                return new_block_location;
            }
        }
        else
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            header_setup_debug(temp, size, temp->prev, NULL, fileline, filename);
            pthread_mutex_unlock(&mutex);
            return memblock;
        }
    }
}

void* heap_malloc_aligned_debug(size_t size, int fileline, const char* filename)
{
    if(!size || heap_validate() || !heap_start)
    {
        return NULL;
    }
    pthread_mutex_lock(&mutex);

    if(heap_start && heap_is_empty == 1)  //empty heap
    {
        size_t offset_page = ALIGN((size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE), PAGE_SIZE) - (size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE);
        size_t offset_word = ALIGN((size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)heap_start + header_size + FENCE_SIZE);
        heap_is_empty = 0;
        while(pages_allocated * PAGE_SIZE < HEADER_FENCE_SIZE(size) + offset_page)
        {
            void* temp = custom_sbrk(PAGE_SIZE);
            if(temp == (void*)-1)
            {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            else
                ++pages_allocated;
        }

        first_block = (mem_header*)((uint8_t*)heap_start + offset_word);
        mem_header* first_block_allocated = first_block;     //first block will be free, it fits in the offset area of the block aligned for page size
        mem_header* second_block = (mem_header*)((uint8_t*)heap_start + offset_page);  //the second is aligned to the page and with size given by user

        header_setup_debug(first_block_allocated, offset_page - offset_word - header_size - 2 * FENCE_SIZE, NULL, second_block, fileline, filename);
        first_block_allocated->free = 1;
        first_block_allocated->control_sum = calculate_control_size((uint8_t*)first_block_allocated);
        header_setup_debug(second_block, size, first_block, NULL, fileline, filename);

        pthread_mutex_unlock(&mutex);
        return (void*)((uint8_t*)second_block + FENCE_SIZE + header_size);
    }

    mem_header* temp = first_block;
    while(temp)     // look for free blocks with enough size and right address
    {
        size_t offset = ALIGN((size_t)((uint8_t*)temp + header_size + FENCE_SIZE), PAGE_SIZE) - (size_t)((uint8_t*)temp + header_size + FENCE_SIZE);
        if(temp->free && temp->size >= HEADER_FENCE_SIZE(size) + offset)
        {
            if(offset == 0)
            {
                size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + header_size + FENCE_SIZE);
                if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block + HEADER_FENCE_SIZE(1)) < (uintptr_t)temp->next)
                {
                    mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block);
                    header_setup_debug(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size) + offset_new_block) - header_size - 2 * FENCE_SIZE, temp, temp->next, fileline, filename);
                    new_block->free = 1;
                    new_block->control_sum = calculate_control_size((uint8_t*)new_block);
                    temp->next->prev = new_block;
                    temp->next->control_sum = calculate_control_size((uint8_t*)temp->next);
                    header_setup_debug(temp, size, temp->prev, new_block, fileline, filename);
                }
                else
                    header_setup_debug(temp, size, temp->prev, temp->next, fileline, filename);

                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp + header_size + FENCE_SIZE);
            }
            else
            {
                if(offset > HEADER_FENCE_SIZE(1))
                {
                    mem_header* new_block = (mem_header*)((uint8_t*)temp + offset);
                    header_setup_debug(new_block, temp->size - offset - HEADER_FENCE_SIZE(0), temp, temp->next, fileline, filename);
                    new_block->free = 1;
                    new_block->control_sum = calculate_control_size((uint8_t*)new_block);
                    temp->next->prev = new_block;
                    temp->next->control_sum = calculate_control_size((uint8_t*)temp->next);
                    header_setup_debug(temp, offset - header_size - 2 * FENCE_SIZE, temp->prev, new_block, fileline, filename);
                    temp->free = 1;
                    temp->control_sum = calculate_control_size((uint8_t*)temp);

                    temp = temp->next;
                    continue;
                }
                else
                {
                    temp = temp->next;
                    continue;
                }
            }
        }
        if(temp->next)
            temp = temp->next;
        else        //if not present, see how much memory is free, and if not sufficient, request OS for more. check the result and then create new header
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            if(IS_POINTER_DIVISIBLE_BY_4096((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE))
            {
                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size));
                header_setup_debug(temp->next, size, temp, NULL, fileline, filename);
                header_setup_debug(temp, temp->size, temp->prev, temp->next, fileline, filename);

                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + header_size + FENCE_SIZE);
            }
            else
            {
                offset = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE), PAGE_SIZE) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE);
                while(free_memory_on_heap < HEADER_FENCE_SIZE(size) + offset)
                {
                    void* res = custom_sbrk(PAGE_SIZE);
                    if(res == (void*)-1)
                    {
                        pthread_mutex_unlock(&mutex);
                        return NULL;
                    }
                    else
                    {
                        free_memory_on_heap += PAGE_SIZE;
                        ++pages_allocated;
                    }
                }
                temp->next = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset);

                size_t offset_new_block = ALIGN((size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE), WORD_LEN) - (size_t)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + header_size + FENCE_SIZE);
                if(offset > HEADER_FENCE_SIZE(1) + offset_new_block)
                {
                    mem_header* new_block_empty = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset_new_block);
                    mem_header* new_block_allocated = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size) + offset);
                    header_setup_debug(new_block_empty, offset - offset_new_block - HEADER_FENCE_SIZE(0), temp, new_block_allocated, fileline, filename);
                    new_block_empty->free = 1;
                    new_block_empty->control_sum = calculate_control_size((uint8_t*)new_block_empty);
                    header_setup_debug(new_block_allocated, size, new_block_empty, NULL, fileline, filename);
                    if(temp->free)
                    {
                        header_setup_debug(temp, temp->size, temp->prev, new_block_empty, fileline, filename);
                        temp->free = 1;
                        temp->control_sum = calculate_control_size((uint8_t*)temp);
                    }
                    else
                        header_setup_debug(temp, temp->size, temp->prev, new_block_empty, fileline, filename);
                    pthread_mutex_unlock(&mutex);
                    return (void*)((uint8_t*)new_block_allocated + FENCE_SIZE + header_size);
                }
                header_setup_debug(temp->next, size, temp, NULL, fileline, filename);
                if(temp->free)
                {
                    header_setup_debug(temp, temp->size, temp->prev, temp->next, fileline, filename);
                    temp->free = 1;
                    temp->control_sum = calculate_control_size((uint8_t*)temp);
                }
                else
                    header_setup_debug(temp, temp->size, temp->prev, temp->next, fileline, filename);

                pthread_mutex_unlock(&mutex);
                return (void*)((uint8_t*)temp->next + FENCE_SIZE + header_size);
            }
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}
void* heap_calloc_aligned_debug(size_t number, size_t size, int fileline, const char* filename)
{
    if(!number || !size || heap_validate())
    {
        return NULL;
    }

    void* ptr = heap_malloc_aligned_debug(number * size, fileline, filename);
    pthread_mutex_lock(&mutex);
    if(ptr)
    {
        memset(ptr, 0, number * size);
        pthread_mutex_unlock(&mutex);
        return ptr;
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}
void* heap_realloc_aligned_debug(void* memblock, size_t size, int fileline, const char* filename)
{
    if((!memblock && !size) || heap_validate())
    {
        return NULL;
    }
    if(size == 0)
    {
        heap_free(memblock);
        return NULL;
    }
    if(!memblock)
    {
        return heap_malloc_aligned_debug(size, fileline, filename);
    }
    if(get_pointer_type(memblock) != pointer_valid)
    {
        return NULL;
    }
    pthread_mutex_lock(&mutex);

    mem_header* temp = (mem_header*)((uint8_t*)memblock - FENCE_SIZE - header_size);
    if(temp->size == size)    // new size == old size, nothing changes
    {
        pthread_mutex_unlock(&mutex);
        return memblock;
    }
    if(temp->size > size)     // new size < old size, shrink the block and update control sum
    {
        if(temp->next && (uintptr_t)((uint8_t*)temp + HEADER_FENCE_SIZE(size) + HEADER_FENCE_SIZE(1)) < (uintptr_t)temp->next)
        {
            mem_header* new_block = (mem_header*)((uint8_t*)temp + HEADER_FENCE_SIZE(size));
            header_setup_debug(new_block, (uint8_t*)temp->next - ((uint8_t*)temp + HEADER_FENCE_SIZE(size)) - header_size - 2 * FENCE_SIZE, temp, temp->next, fileline, filename);
            new_block->free = 1;
            new_block->control_sum = calculate_control_size((uint8_t*)new_block);
            header_setup_debug(temp, size, temp->prev, new_block, fileline, filename);
        }
        else
            header_setup_debug(temp, size, temp->prev, temp->next, fileline, filename);

        pthread_mutex_unlock(&mutex);
        return memblock;
    }
    else
    {
        if(temp->next)         //check whether next block exists
        {
            // if so, check if its' size + size of curr block is enough to fit the new block
            if(temp->next->free && (unsigned long)(((uint8_t*)temp->next + HEADER_FENCE_SIZE(temp->next->size) - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup_debug(temp, size, temp->prev, temp->next->next, fileline, filename);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else if((unsigned long)(((uint8_t*)temp->next - (uint8_t*)temp)) >= HEADER_FENCE_SIZE(size))
            {
                header_setup_debug(temp, size, temp->prev, temp->next, fileline, filename);
                pthread_mutex_unlock(&mutex);
                return memblock;
            }
            else                   // if not, there is a need to change the block's location
            {
                pthread_mutex_unlock(&mutex);
                void *new_block_location = heap_malloc_aligned_debug(size, fileline, filename);
                pthread_mutex_lock(&mutex);
                if(!new_block_location)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                memcpy(new_block_location, memblock, temp->size);
                pthread_mutex_unlock(&mutex);
                heap_free((uint8_t*)temp + header_size + FENCE_SIZE);
                pthread_mutex_lock(&mutex);
                ((mem_header*)((uint8_t*)new_block_location - header_size - FENCE_SIZE))->control_sum = calculate_control_size((uint8_t*)new_block_location - header_size - FENCE_SIZE);
                pthread_mutex_unlock(&mutex);
                return new_block_location;
            }
        }
        else
        {
            unsigned long free_memory_on_heap = pages_allocated * PAGE_SIZE - (((uint8_t*)temp + HEADER_FENCE_SIZE(temp->size)) - (uint8_t*)heap_start);

            while(free_memory_on_heap < HEADER_FENCE_SIZE(size))
            {
                void* res = custom_sbrk(PAGE_SIZE);
                if(res == (void*)-1)
                {
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                else
                {
                    free_memory_on_heap += PAGE_SIZE;
                    ++pages_allocated;
                }
            }
            header_setup_debug(temp, size, temp->prev, NULL, fileline, filename);
            pthread_mutex_unlock(&mutex);
            return memblock;
        }
    }
}

