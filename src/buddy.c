#include <sys/mman.h>
#include <stdio.h>
#include "../include/buddy.h"
#include "internal.h"

static const size_t BUDDYHEADER_META_DATA_SIZE = (sizeof(BuddyHeader) + 31) & ~31;
static const size_t BUDDYPOOL_META_DATA_SIZE = (sizeof(BuddyPool) + 63) & ~63;

BuddyHeader* get_buddy(const BuddyPool* pool, const BuddyHeader* ptr){
    return (BuddyHeader*)((uint8_t*)pool->base + (((uint8_t*)ptr - (uint8_t*)pool->base) ^ (1ULL << ptr->order)));
}

uint8_t find_order(const size_t size){
    size_t n = 1;
    uint8_t order = 0;

    while(n < size){
        n *= 2;
        order++;
    }

    return (order < MIN_ORDER) ? MIN_ORDER : order;
}

void* buddy_init(const size_t size){
    uint8_t order = find_order(size);

    if(size != (1ULL << order)){
        printf("size is not a power of two\n");
        return NULL;
    } 

    void* addr = mmap(NULL, size + BUDDYPOOL_META_DATA_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(addr == MAP_FAILED){
        printf("mmap failed\n");
        return NULL;
    }

    BuddyPool* pool = (BuddyPool*) addr;
    pool->base = (uint8_t*) addr + BUDDYPOOL_META_DATA_SIZE;
    pool->size = size;

    for(int i = 0; i < MAX_ORDER + 1; i++){
        pool->free_list[i] = NULL;
    }

    BuddyHeader* header = (BuddyHeader*)pool->base;
    header->is_free = true;
    header->order = order;
    header->next = NULL;
    header->prev = NULL;

    pool->free_list[order] = header;
    return (void*)pool;
}

void* buddy_alloc(void* pool, const size_t size){
    uint8_t order = find_order(size + BUDDYHEADER_META_DATA_SIZE);
    BuddyPool* pool_ptr = (BuddyPool*)pool;
    void* ret = NULL;

    if(pool_ptr->free_list[order]){
        ret = (uint8_t*)(pool_ptr->free_list[order]) + BUDDYHEADER_META_DATA_SIZE;
        pool_ptr->free_list[order]->is_free = false;

        pool_ptr->free_list[order] = pool_ptr->free_list[order]->next;

        if(pool_ptr->free_list[order])
            pool_ptr->free_list[order]->prev = NULL;
        
        return ret;
    }
    
    for(int i = order + 1; i < MAX_ORDER + 1; i++){
        if(pool_ptr->free_list[i]){
            BuddyHeader* split = pool_ptr->free_list[i];

            split->is_free = false;
            split->next = NULL;
            split->prev = NULL;

            pool_ptr->free_list[i] = pool_ptr->free_list[i]->next;

            if(pool_ptr->free_list[i])
                pool_ptr->free_list[i]->prev = NULL;

            while(split->order != order){
                BuddyHeader* header1 = (BuddyHeader*)((uint8_t*)split + (1ULL << (split->order - 1)));
                header1->is_free = true;
                header1->order = split->order - 1;
                header1->next = pool_ptr->free_list[header1->order];
                header1->prev = NULL;
                
                if(pool_ptr->free_list[header1->order])
                    pool_ptr->free_list[header1->order]->prev = header1;
                pool_ptr->free_list[header1->order] = header1;

                split->order--;
            }

            ret = (uint8_t*)split + BUDDYHEADER_META_DATA_SIZE;
            return ret;
        }
    }

    printf("No avaliable memory\n");
    return ret;
}

void buddy_free(void* pool, const void *ptr){
    BuddyPool* pool_ptr = (BuddyPool*)pool;
    BuddyHeader* header = (BuddyHeader*)((uint8_t*)ptr - BUDDYHEADER_META_DATA_SIZE);
    uint8_t order = header->order;

    while(order < MAX_ORDER){
        header->is_free = true;
        BuddyHeader* buddy1_header = get_buddy(pool_ptr, header);
    
        if(buddy1_header->order == order && buddy1_header->is_free){
            if(pool_ptr->free_list[order] == buddy1_header)
                pool_ptr->free_list[order] = buddy1_header->next;
            if(buddy1_header->next)
                buddy1_header->next->prev = buddy1_header->prev;
            if(buddy1_header->prev)
                buddy1_header->prev->next = buddy1_header->next;
    
            BuddyHeader* merged = (header < buddy1_header)? header : buddy1_header;
    
            merged->order = order + 1;
            merged->is_free = true;
            merged->prev = NULL;

            header = merged;
            order = header->order;
        }
        else break;
    }

    header->next = pool_ptr->free_list[order];
    header->prev = NULL;

    if(pool_ptr->free_list[order]) 
        pool_ptr->free_list[order]->prev = header;

    pool_ptr->free_list[order] = header;
}
void buddy_destroy(void* pool){
    if(pool){
        munmap(pool, ((BuddyPool*)pool)->size + BUDDYPOOL_META_DATA_SIZE);
    }
}
void buddy_dump(const void* pool){
    BuddyPool* pool_ptr = (BuddyPool*)pool;
    BuddyHeader* header = (BuddyHeader*)pool_ptr->base;

    size_t size = (1ULL << header->order);
    char f = (header->is_free) ? 'F': 'A';
    printf("%p [%c:%ldB] ", header, f, size);
    header = (BuddyHeader*)((uint8_t*)header + size);

    while(header < (BuddyHeader*)((uint8_t*)(pool_ptr->base) + pool_ptr->size) && header->order <= MAX_ORDER){
        size = (1ULL << header->order);
        f = (header->is_free) ? 'F': 'A';
        printf("| %p [%c:%ldB]", header, f, size);
        header = (BuddyHeader*)((uint8_t*)header + size);
    }

    printf("\n");
}
