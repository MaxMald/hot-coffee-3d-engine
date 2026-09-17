#include "hc/utilities/memory/hcMemory.h"

#include <new>
#include <memory>

namespace hc
{
  namespace memory
  {
    void* Alloc(SizeT size)
    {
      if (size == 0)
        throw InvalidArgumentException("Size must be greater than zero.");

      if (size > SIZE_MAX)
        throw InvalidArgumentException("Size is too large, causing overflow.");

      void* ptr = std::malloc(size);
      if (!ptr)
        throw std::bad_alloc();

      return ptr;
    }

    void* AlignedAlloc(SizeT size, SizeT alignment)
    {
      if (!IsAlignmentPowerOfTwo(alignment))
        throw InvalidArgumentException("Alignment must be a power of two.");

      if (size == 0)
        throw InvalidArgumentException("Size must be greater than zero.");

      if (size > SIZE_MAX - alignment)
        throw InvalidArgumentException("Size is too large, causing overflow.");

      return ::operator new(size, std::align_val_t(alignment));
    }

    void Free(void* ptr)
    {
      if (ptr)
        std::free(ptr);
    }

    void AlignedFree(void* ptr, SizeT alignment)
    {
      if (ptr)
        ::operator delete(ptr, std::align_val_t(alignment));
    }
  }
}
