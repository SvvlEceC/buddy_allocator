#include "buddy.h"
#include <stdio.h>

int main() {
    printf("--- Test 4: Out of Memory Stress Test ---\n");
    void* pool = buddy_init(256);

    void* p1 = buddy_alloc(pool, 128);
    void* p2 = buddy_alloc(pool, 128);
    
    printf("Full memory dump:\n");
    buddy_dump(pool);

    void* p3 = buddy_alloc(pool, 8);
    if (p3 == NULL) {
        printf("Success: p3 is NULL as expected (Out of Memory)\n");
    } else {
        printf("Fail: p3 should be NULL but got a pointer!\n");
    }

    buddy_free(pool, p1);
    p3 = buddy_alloc(pool, 8);
    if (p3 != NULL) {
        printf("Success: Allocated 8 bytes after freeing p1\n");
    }

    buddy_dump(pool);
    return 0;
}