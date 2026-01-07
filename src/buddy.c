#include <sys/mman.h>
#include <stdio.h>
#include "buddy.h"
#include "internal.h"

BuddyHeader* get_buddy(const BuddyPool* pool, const BuddyHeader* ptr){
    return pool->base + ((ptr - pool) ^ (uint8_t)pow(2, ptr->order));
}

uint8_t find_order(const size_t size){
    size_t n = size;
    uint8_t order = 0;

    while(n > 1){
        n /= 2;
        order++;
    }

    return ++order;
}

void* buddy_init(const size_t size){
    uint8_t order = find_order(size);

    if(size != pow(2, order)){
        printf("size is not a power of two\n");
        return NULL;
    } 

    size_t meta_data_size = (sizeof(BuddyPool) + 63) & ~63;

    void* addr = mmap(NULL, size + meta_data_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(addr == MAP_FAILED){
        printf("mmap failed\n");
        return NULL;
    }

    BuddyPool* pool = (BuddyPool*) addr;
    pool->base = (uint8_t*) addr + meta_data_size;

    for(int i = 0; i < MAX_ORDER + 1; i++){
        pool->free_list[i] = NULL;
    }

    BuddyHeader* header = (BuddyHeader*)pool->base;
    header->is_free = true;
    header->order = order;
    header->next = NULL;
    header->prev = NULL;

    pool->free_list[order] = header;
    return pool;
}

void* buddy_alloc(void* pool, const size_t size){
    size_t meta_data_size = (sizeof(BuddyHeader) + 31) & ~31;
    uint8_t order = find_order(size + meta_data_size);
    BuddyPool* pool_ptr = pool;
    int split = -1;
    void* ret = NULL;

    if(pool_ptr->free_list[order]){
        ret = (uint8_t*)(pool_ptr->free_list[order]) + meta_data_size;
        pool_ptr->free_list[order] = pool_ptr->free_list[order]->next;
        return ret;
    }
    
    for(int i = order; i < MAX_ORDER + 1; i++){
        if(pool_ptr->free_list[i])
            split = i;
    }

    if(split == -1){
        printf("No avaliable memory");
        return ret;
    }

    for(int i = split; i > order; i--)
}

void buddy_free(void* pool, const void *ptr){

}
void buddy_cleanup(void* pool){

}
void buddy_dump(const void* pool){

}
