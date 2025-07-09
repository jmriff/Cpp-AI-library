#include "nnmemory.hpp"

void* NNAllocator.allocate(std::size_t size)
{
    for (auto& freeBlock = freeList; freeBlock != nullptr; freeBlock = freeBlock->next)
    {
        if (freeBlock->size >= size)
        {
            void* ptr = pool + freeBlock->offset;
            freeBlock->offset += size;
            freeBlock->size -= size;

            if (freeBlock->size == 0)
            {
                // Remove the block from the free list
                if (freeList == freeBlock)
                {
                    freeList = freeBlock->next;
                }
                else
                {
                    FreeBlock* prev = freeList;
                    while (prev->next != freeBlock)
                    {
                        prev = prev->next;
                    }
                    prev->next = freeBlock->next;
                }
                delete freeBlock;
            }
            return ptr;
        }
    }
    
    if (offset + size > poolSize)
    {
        throw std::bad_alloc();
    }
    void* ptr = pool + offset;
    offset += size;
    return ptr;
}

void MemoryPool::deallocate(void* ptr, std::size_t size)
{
    FreeBlock* block = new FreeBlock();

    block->size = size;
    block->offset = static_cast<char*>(ptr) - pool;
    block->next = freeList;
    freeList = block;
}