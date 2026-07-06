#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/memory/hcPoolStorage.h"

namespace hc
{
  namespace memory
  {
    /**
     * @brief A memory allocator adapter suitable for use with STL containers, backed by a
     * PoolStorage.
     *
     * @tparam T The type of objects to allocate.
     */
    template<typename T>
    class PoolAllocator
    {
    public:
      using value_type = T;
      using pointer = T*;
      using const_pointer = const T*;
      using size_type = SizeT;
      using difference_type = std::ptrdiff_t;

      template<typename U>
      struct rebind
      {
        using other = PoolAllocator<U>;
      };

      PoolAllocator() noexcept = default;

      template<typename U>
      PoolAllocator(const PoolAllocator<U>& other) noexcept :
        m_poolStorage(other.getPoolStorage())
      {}

      pointer allocate(size_type count)
      {
        if (!m_poolStorage)
          throw RuntimeErrorException("PoolAllocator is not initialized with a PoolStorage.");

        if (count != 1)
          throw InvalidArgumentException("PoolAllocator can only allocate one object at a time.");

        return static_cast<pointer>(m_poolStorage->allocate());
      }

      void deallocate(pointer ptr, size_type count) noexcept
      {
        if (!ptr)
          return;

        if (count == 0)
          return;

        HC_ASSERT(m_poolStorage && "PoolAllocator is not initialized with a PoolStorage.");
        HC_ASSERT(count == 1 && "PoolAllocator can only deallocate one object at a time.");

        m_poolStorage->free(ptr);
      }

      PoolStorage* getPoolStorage() const noexcept
      {
        return m_poolStorage;
      }

      void setPoolStorage(PoolStorage* poolStorage) noexcept
      {
        m_poolStorage = poolStorage;
      }

      template<typename U>
      friend class PoolAllocator;

      friend bool operator==(const PoolAllocator& lhs, const PoolAllocator& rhs) noexcept
      {
        return lhs.m_poolStorage == rhs.m_poolStorage;
      }

      friend bool operator!=(const PoolAllocator& lhs, const PoolAllocator& rhs) noexcept
      {
        return !(lhs == rhs);
      }

    private:
      PoolStorage* m_poolStorage = nullptr;
    };
  }
}
