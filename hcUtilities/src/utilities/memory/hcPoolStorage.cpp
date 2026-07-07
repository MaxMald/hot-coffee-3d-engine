#include "hc/utilities/memory/hcPoolStorage.h"

#include <cstdlib>
#include "hc/utilities/memory/hcMemory.h"

namespace hc
{
  namespace memory
  {
    PoolStorage::PoolStorage() :
      m_blockSize(0),
      m_blockCount(0),
      m_alignment(0),
      m_memory(nullptr),
      m_freeList(nullptr),
      m_initialized(false)
    {}

    PoolStorage::~PoolStorage()
    {
      destroy();
    }

    void PoolStorage::initialize(SizeT blockSize, SizeT blockCount, SizeT alignment)
    {
      if (m_initialized)
        throw RuntimeErrorException("PoolStorage is already initialized.");

      if (blockSize == 0 || blockCount == 0)
        throw InvalidArgumentException("Block size and block count must be greater than zero.");

      if (blockSize < sizeof(void*))
        throw InvalidArgumentException("Block size must be at least the size of a pointer.");

      if (!IsAlignmentPowerOfTwo(alignment))
        throw InvalidArgumentException("Alignment must be a power of two.");

      if (blockSize % alignment != 0)
        throw InvalidArgumentException("Block size must be a multiple of the alignment.");

      if (blockCount > SIZE_MAX / blockSize)
        throw InvalidArgumentException("Block size and block count are too large, causing overflow.");

      try
      {
        m_memory = AlignedAlloc(blockSize * blockCount, alignment);
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
        m_alignment = alignment;
      }
      catch (...)
      {
        destroy();
        throw;
      }
    }

    void PoolStorage::destroy()
    {
      if (m_memory != nullptr)
      {
        AlignedFree(m_memory, m_alignment);
        m_memory = nullptr;
      }

      m_freeList = nullptr;
      m_initialized = false;
      m_blockSize = 0;
      m_blockCount = 0;
      m_alignment = 0;
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

    void PoolStorage::free(void* ptr) noexcept
    {
      if (ptr == nullptr)
        return;

      if (!m_initialized)
      {
        HC_ASSERT(false && "PoolStorage is not initialized.");
        return;
      }

      if (!owns(ptr))
      {
        HC_ASSERT(false && "Pointer does not belong to this PoolStorage.");
        return;
      }

      *reinterpret_cast<void**>(ptr) = m_freeList;
      m_freeList = ptr;
    }

    bool PoolStorage::owns(const void* ptr) const noexcept
    {
      if (ptr == nullptr)
        return false;

      if (!m_initialized)
      {
        HC_ASSERT(false && "PoolStorage is not initialized.");
        return false;
      }

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
