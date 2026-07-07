#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  namespace memory
  {
    /**
     * @brief Fixed-size pool allocator interface.
     *
     * A pool allocator is best suited for many allocations of the same size.
     * Allocation and deallocation are typically O(1).
     *
     * Typical usage:
     * - call initialize(...) to set up the pool
     * - call allocate() / free() for individual objects
     * - call reset() when all objects can be discarded at once
     * - call destroy() to release all resources
     */
    class HC_UTILITY_EXPORT PoolStorage
    {
    public:
      PoolStorage();
      ~PoolStorage();

      /**
       * @brief Initializes the pool.
       *
       * @param blockSize Size of each block in bytes.
       * @param blockCount Number of blocks in the pool.
       * @param alignment Alignment of each block in bytes (default is max alignment).
       */
      void initialize(
        SizeT blockSize,
        SizeT blockCount,
        SizeT alignment = alignof(std::max_align_t)
      );

      /**
       * @brief Releases all resources owned by the pool.
       */
      void destroy();

      /**
       * @brief Allocates one block from the pool.
       *
       * @return Pointer to a block.
       *
       * @throws RuntimeError if the pool is exhausted or not initialized.
       */
      void* allocate();

      /**
       * @brief Returns a block to the pool.
       *
       * @param ptr Pointer previously returned by allocate().
       */
      void free(void* ptr) noexcept;

      /**
       * @brief Returns true if the pointer belongs to this pool.
       */
      bool owns(const void* ptr) const noexcept;

      /**
       * @brief Releases all allocated blocks back to the pool.
       */
      void reset();

      /**
       * @brief Gets the size of each block in bytes.
       */
      SizeT getBlockSize() const;

      /**
       * @brief Gets the total number of blocks in the pool.
       */
      SizeT getBlockCount() const;

      /**
       * @brief Gets the number of currently free blocks.
       */
      SizeT getFreeBlockCount() const;

      /**
       * @brief Gets the number of currently allocated blocks.
       */
      SizeT getUsedBlockCount() const;

      /**
       * @brief Returns true if the pool has been initialized.
       */
      bool isInitialized() const;

    private:
      SizeT m_blockSize = 0;
      SizeT m_blockCount = 0;
      SizeT m_alignment = 0;
      void* m_memory = nullptr;
      void* m_freeList = nullptr;
      bool m_initialized = false;
    };
  }
}
