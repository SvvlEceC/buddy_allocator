#ifndef BUDDY_H
#define BUDDY_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes a memory pool.
 * @param size Total size of the memory pool in bytes(must be a power of 2).
 * @return Returns the handle of the memory pool, or NULL on failure.
 */
void* buddy_init(const size_t size);

/**
 * @brief Allocates memory block.
 * @param pool The handle of the pool the memory block will be allocated in.
 * @param size Allocation size.
 * @return Resturns the start of the allocated memory.
 */
void* buddy_alloc(void* pool, const size_t size);

/**
 * @brief Deallocates memory block.
 * @param pool The handle of the pool the memory block will be allocated in.
 */
void buddy_free(void* pool, const void *ptr);

/**
 * @brief Destroys the memory pool.
 * @param pool The handle of the pool to be destroyed.
 */
void buddy_destroy(void* pool);

/**
 * @brief Visualizes the structure of the memory pool.
 * @param pool The handle of the pool to be visualized.
 */
void buddy_dump(const void* pool);

#endif