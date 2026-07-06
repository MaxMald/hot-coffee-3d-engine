#include "hc/utilities/memory/hcPoolStorage.h"

#include <cstdlib>

namespace hc
{
  namespace memory
  {
    PoolStorage::PoolStorage() :
      m_initialized(false),
      m_blockSize(0),
      m_blockCount(0),
      m_memory(nullptr),
      m_freeList(nullptr)
    {}

    PoolStorage::~PoolStorage()
    {
      destroy();
    }

    void PoolStorage::initialize(SizeT blockSize, SizeT blockCount)
    {
      if (m_initialized)
        throw RuntimeErrorException("PoolStorage is already initialized.");

      if (blockSize == 0 || blockCount == 0)
        throw InvalidArgumentException("Block size and block count must be greater than zero.");

      if (blockSize < sizeof(void*))
        throw InvalidArgumentException("Block size must be at least the size of a pointer.");

      if (blockSize % alignof(void*) != 0)
        throw InvalidArgumentException("Block size must be a multiple of the pointer size.");

      if (blockCount > SIZE_MAX / blockSize)
        throw InvalidArgumentException("Block size and block count are too large, causing overflow.");

      m_memory = std::malloc(blockSize * blockCount);
      if (!m_memory)
        throw std::bad_alloc();

      m_freeList = m_memory;

      Byte* currentBlock = static_cast<Byte*>(m_memory);
      for (SizeT i = 0; i < blockCount - 1; ++i)
      {
        *reinterpret_cast<void**>(currentBlock) = currentBlock + blockSize;
        currentBlock += blockSize;
      }
      *reinterpret_cast<void**>(currentBlock) = nullptr;

      m_initialized = true;
      m_blockSize = blockSize;
      m_blockCount = blockCount;
    }

    void PoolStorage::destroy()
    {
      if (m_memory != nullptr)
      {
        std::free(m_memory);
        m_memory = nullptr;
      }

      m_freeList = nullptr;
      m_initialized = false;
      m_blockSize = 0;
      m_blockCount = 0;
    }

    void* PoolStorage::allocate()
    {
      if (!m_initialized)
        throw RuntimeErrorException("PoolStorage is not initialized.");

      if (m_freeList == nullptr)
        throw RuntimeErrorException("PoolStorage is exhausted. No free blocks available.");

      void* block = m_freeList;
      m_freeList = *reinterpret_cast<void**>(m_freeList);
      return block;
    }

    void PoolStorage::free(void* ptr)
    {
      if (!m_initialized)
        throw RuntimeErrorException("PoolStorage is not initialized.");

      if (ptr == nullptr)
        return;

      if (!owns(ptr))
        throw InvalidArgumentException("Pointer does not belong to this PoolStorage.");

      *reinterpret_cast<void**>(ptr) = m_freeList;
      m_freeList = ptr;
    }

    bool PoolStorage::owns(const void* ptr) const
    {
      if (!m_initialized)
        throw RuntimeErrorException("PoolStorage is not initialized.");

      if (ptr == nullptr)
        return false;

      const UIntPtr start = reinterpret_cast<UIntPtr>(m_memory);
      const UIntPtr end = start + m_blockSize * m_blockCount;
      const UIntPtr ptrAddress = reinterpret_cast<UIntPtr>(ptr);

      return ptrAddress >= start && ptrAddress < end && (ptrAddress - start) % m_blockSize == 0;
    }

    void PoolStorage::reset()
    {
      if (!m_initialized)
        throw RuntimeErrorException("PoolStorage is not initialized.");

      m_freeList = m_memory;
      Byte* currentBlock = static_cast<Byte*>(m_memory);
      for (SizeT i = 0; i < m_blockCount - 1; ++i)
      {
        *reinterpret_cast<void**>(currentBlock) = currentBlock + m_blockSize;
        currentBlock += m_blockSize;
      }
      *reinterpret_cast<void**>(currentBlock) = nullptr;
    }

    SizeT PoolStorage::getBlockSize() const
    {
      return m_blockSize;
    }

    SizeT PoolStorage::getBlockCount() const
    {
      return m_blockCount;
    }

    SizeT PoolStorage::getFreeBlockCount() const
    {
      if (!m_initialized)
        throw RuntimeErrorException("PoolStorage is not initialized.");

      SizeT freeCount = 0;
      void* current = m_freeList;
      while (current != nullptr)
      {
        ++freeCount;
        current = *reinterpret_cast<void**>(current);
      }
      return freeCount;
    }

    SizeT PoolStorage::getUsedBlockCount() const
    {
      if (!m_initialized)
        throw RuntimeErrorException("PoolStorage is not initialized.");

      return m_blockCount - getFreeBlockCount();
    }

    bool PoolStorage::isInitialized() const
    {
      return m_initialized;
    }
  }
}
