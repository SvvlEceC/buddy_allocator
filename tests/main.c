#include "../include/buddy.h"

int main(int argc, char const *argv[])
{
    void* pool = buddy_init(1024);

    int* list2 = (int*)buddy_alloc(pool, sizeof(int) * 2);
    buddy_dump(pool);
    char* list1 = (char*)buddy_alloc(pool, sizeof(char) * 64);
    buddy_dump(pool);

    buddy_free(pool, list2);
    buddy_dump(pool);

    buddy_free(pool, list1);
    buddy_dump(pool);

    return 0;
}
