#ifndef MEM_HPP
#define MEM_HPP

#include "types.hpp"
#include "vga.hpp"

/// A node representing a chunk of memory.
struct MemNode {
    MemNode* prev;
    MemNode* next;
    u32 size;
    bool used;
};

/// The ammount of memory available to the allocator, in bytes.
constexpr u32 DYN_MEM_SIZE = 1048576;

/// Dynamic memory area.
/// This memory shouldn't be accessed directly,
/// but instead should be allocated and deallocated with
/// `mem_alloc` and `mem_free`.
u8 dyn_mem[DYN_MEM_SIZE];

#define dyn_mem_start reinterpret_cast<MemNode*>(dyn_mem)

/// Sets up the dynamic memory allocator.
/// This function must be called before calling
/// `mem_alloc` or `mem_free`.
/// This function should only be called once.
/// Calling it again will deallocate all dynamic memory.
extern "C" void init_alloc() {
    dyn_mem_start->prev = nullptr;
    dyn_mem_start->next = nullptr;
    dyn_mem_start->size = DYN_MEM_SIZE - sizeof(MemNode);
    dyn_mem_start->used = false;
};

inline u32 align4(u32 n) { return (n + 3) & ~0x03; }

/// Allocates a chunk of memory.
/// Returns a non-null pointer with at least the desired size,
/// or a null pointer if there isn't enough memory.
void* mem_alloc(u32 n) {
    MemNode* node;
    n = align4(n);
    
    for (
        node = dyn_mem_start;
        node != nullptr;
        node = node->next
    ) {
        if (node->used) continue;
        
        if (node->size == n) {
            node->used = true;
            return node + 1;
        }
        else if (node->size > n) {
            if (node->size - sizeof(MemNode) - n < 4) {
                node->used = true;
                return node + 1;
            }
            else {
                auto next = node->next;
                auto size = node->size;
                
                node->next = (MemNode*)(
                    (u8*)node + sizeof(MemNode) + n
                );
                node->size = n;
                node->next->size = size - sizeof(MemNode) - n;
                node->next->prev = node;
                node->next->next = next;
                next->prev = node->next;
                next->used = false;

                node->used = true;
                return node + 1;
            }
        }
    }

    return nullptr;
};

/// Merges a node with unused nodes before it.
MemNode* try_merge_prev(MemNode* node) {
    auto prev = node->prev;

    while (prev != nullptr && prev->used == false) {
        prev->next = node->next;
        node->next->prev = prev;
        prev->size += sizeof(MemNode) + node->size;

        node = prev;
        prev = prev->prev;
    }

    return node;
}

/// Merges a node with unused nodes after it.
MemNode* try_merge_next(MemNode* node) {
    auto next = node->next;

    while (next != nullptr && next->used == false) {
        node->size += sizeof(MemNode) + next->size;
        node->next = next->next;
        next->next->prev = node;

        next = node->next;
    }

    return node;
}

/// Deallocates memory allocated by `mem_alloc`.
/// Accessing memory that was deallocated by this function
/// is unsafe.
void mem_free(void* ptr) {
    auto node = (MemNode*)((u8*)ptr - sizeof(MemNode));

    node->used = false;
    try_merge_next(node);
    try_merge_prev(node);
}

/// Copies `n` bytes from `src` to `dst`.
extern "C" void memcpy(void* dst, const void* src, u32 n);
/// Sets `n` bytes starting at `buf` to `c`.
extern "C" void memset(void* buf, u8 c, u32 n);

void* operator new(size_t n) { return mem_alloc(n); }
void* operator new[](size_t n) { return mem_alloc(n); }
void operator delete(void* p) noexcept { mem_free(p); }
void operator delete[](void* p) noexcept { mem_free(p); }

#endif
