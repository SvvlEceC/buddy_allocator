#include "buddy.h"
#include <stdio.h>

int main() {
    printf("--- Test 3: Coalescing (Reuniting Buddies) ---\n");
    void* pool = buddy_init(128);

    void* p1 = buddy_alloc(pool, 32);
    void* p2 = buddy_alloc(pool, 32);
    void* p3 = buddy_alloc(pool, 32);
    void* p4 = buddy_alloc(pool, 32);
    buddy_dump(pool);

    printf("Freeing p2 and p1...\n");
    buddy_free(pool, p2);
    buddy_free(pool, p1); 
    buddy_dump(pool);

    printf("Freeing p4 and p3...\n");
    buddy_free(pool, p4);
    buddy_free(pool, p3);
    
    buddy_dump(pool);
    return 0;
}