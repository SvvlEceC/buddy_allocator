#ifndef INTERNAL_H
#define INTERNAL_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#define MAX_ORDER 20
#define MIN_ORDER 4


typedef struct BuddyHeader{
    uint8_t order;
    bool is_free;
    struct BuddyHeader* prev;
    struct BuddyHeader* next;
}BuddyHeader;

typedef struct{
    uint8_t *base;
    size_t size;
    BuddyHeader *free_list[MAX_ORDER + 1];
}BuddyPool;

BuddyHeader* get_buddy(const BuddyPool* pool, const BuddyHeader* ptr);
uint8_t find_order(const size_t size);

#endif