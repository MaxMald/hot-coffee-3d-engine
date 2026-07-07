#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  namespace memory
  {
    /**
     * @brief A stack-based memory allocator.
     *
     * StackStorage is a simple memory allocator that allocates memory in a stack-like
     * manner. It is best suited for scenarios where you need to allocate and free
     * memory in a last-in-first-out (LIFO) order.
     *
     * Typical usage:
     * - call initialize(...) to set up the storage
     * - call allocate() for individual allocations
     * - call free() to free the most recent allocation
     * - call reset() when all allocations can be discarded at once
     * - call destroy() to release all resources
     */
    class HC_UTILITY_EXPORT StackStorage
    {
    public:
      StackStorage();
      ~StackStorage();

      /**
       * @brief Initializes the stack storage with a given capacity.
       *
       * @param capacity The total size of the storage in bytes.
       */
      void initialize(SizeT capacity);

      /**
       * @brief Checks if the stack storage has been initialized.
       *
       * @return true if initialized, false otherwise.
       */
      bool isInitialized() const noexcept;

      /**
       * @brief Returns the total capacity of the stack storage.
       *
       * @return The total size of the storage in bytes.
       */
      SizeT getCapacity() const noexcept;

      /**
       * @brief Allocates memory from the stack storage.
       *
       * @param size The size of the memory block to allocate in bytes.
       * @param alignment The alignment requirement for the memory block (default is max
       * alignment).
       *
       * @return A pointer to the allocated memory.
       */
      void* allocate(SizeT size, SizeT alignment = alignof(std::max_align_t));

      /**
       * @brief Frees the most recent allocation from the stack storage.
       *
       * @param ptr Pointer to the memory block to free.
       */
      void free(void* ptr) noexcept;

      /**
       * @brief Resets the stack storage, freeing all allocations.
       */
      void reset();

      /**
       * @brief Destroys the stack storage, releasing all resources.
       */
      void destroy() noexcept;

    private:
      SizeT m_capacity;
      SizeT m_current;
      UIntPtr m_lastHeaderAddress;
      void* m_memory;
      bool m_initialized;
    };
  }
}
