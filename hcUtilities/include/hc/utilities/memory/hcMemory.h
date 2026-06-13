#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  namespace memory
  {
    HC_UTILITY_EXPORT void* Alloc(SizeT size);
    HC_UTILITY_EXPORT void Free(void* ptr);

    template<typename T>
    T* AllocArray(SizeT count)
    {
      return static_cast<T*>(Alloc(sizeof(T) * count));
    }

    template<typename T>
    struct ArrayDeleter
    {
      void operator()(T* ptr) const
      {
        Free(ptr);
      }
    };

    template<typename T>
    using UniqueArrayPtr = UniquePtrWithDeleter<T[], ArrayDeleter<T>>;
  }
}
