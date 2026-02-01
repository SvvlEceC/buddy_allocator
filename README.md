# buddy_allocator
# Buddy Memory Allocator

A **custom buddy memory allocator** implemented in **C**, using `mmap` for
memory management.

This project was developed to understand low-level memory allocation,
fragmentation handling, and allocator design.

> Educational project – not intended to replace `malloc`.

---
## Running test files
    make test%
---

## Overview
The allocator manages a fixed-size memory pool whose size is a power of two.
Memory is split and merged using the **buddy allocation algorithm**, allowing
fast allocation and deallocation with limited fragmentation.

---

## Features
- Buddy allocation algorithm
- Power-of-two memory pool
- Free lists indexed by block order
- Block splitting on allocation
- Buddy merging on deallocation
- Constant-time buddy address calculation using XOR
- Memory visualization via `buddy_dump`

---

## Architecture
- **Memory Source**: `mmap`-allocated contiguous memory region
- **Metadata**:
  - Per-block headers storing order and allocation state
  - Pool-level structure holding free lists
- **Allocation**:
  - Requested size rounded up to nearest power of two
  - Larger blocks recursively split if needed
- **Deallocation**:
  - Buddy blocks merged recursively when both are free

---

## API
```c
void* buddy_init(size_t size);
void* buddy_alloc(void* pool, size_t size);
void  buddy_free(void* pool, const void* ptr);
void  buddy_destroy(void* pool);
void  buddy_dump(const void* pool);
