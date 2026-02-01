#include "buddy.h"
#include <stdio.h>

int main() {
    printf("--- Test 2: Power of Two Alignment ---\n");
    void* pool = buddy_init(1024);

    void* p1 = buddy_alloc(pool, 100);
    printf("Allocated 100 bytes (should take 128)\n");
    buddy_dump(pool);

    void* p2 = buddy_alloc(pool, 200);
    printf("Allocated 200 bytes (should take 256)\n");
    buddy_dump(pool);

    buddy_free(pool, p1);
    buddy_free(pool, p2);
    buddy_dump(pool);
    return 0;
}