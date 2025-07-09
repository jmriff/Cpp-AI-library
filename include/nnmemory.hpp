#ifndef __NNMEMORY_HPP__
#define __NNMEMORY_HPP__

#include <memory>
#include <vector>

typedef struct _FreeBlock
{
    std::size_t size;
    std::size_t offset;
    struct _FreeBlock* next;
} FreeBlock;

template <typename T>
struct NNAllocator
{
    std::size_t poolSize;
    std::size_t offset;
    char* pool;

    FreeBlock* freeList;

    NNAllocator() = default;

    template <typename U>
    NNAllocator(const MyAllocator<U>&) {} 

    T* allocate(std::size_t size);

    void deallocate(T* ptr, std::size_t size);
};

#endif // __NNMEMORY_HPP__