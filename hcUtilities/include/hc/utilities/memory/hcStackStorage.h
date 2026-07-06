#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  namespace memory
  {
    class HC_UTILITY_EXPORT StackStorage
    {
    public:
      StackStorage();
      ~StackStorage();

      void initialize(SizeT capacity);
      bool isInitialized() const noexcept;
      SizeT getCapacity() const noexcept;
      void* allocate(SizeT size);
      void free(void* ptr) noexcept;
      void reset();
      void destroy() noexcept;

    private:
      SizeT m_capacity;
      SizeT m_current;
      void* m_memory;
      bool m_initialized;
    };
  }
}
