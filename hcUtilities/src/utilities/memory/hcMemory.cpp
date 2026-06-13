#include "hc/utilities/memory/hcMemory.h"

#include <memory>

namespace hc
{
  namespace memory
  {
    void* Alloc(SizeT size)
    {
      return std::malloc(size);
    }

    void Free(void* ptr)
    {
      std::free(ptr);
    }
  }
}
