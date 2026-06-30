#include <gtest/gtest.h>
#include "hc/utilities/memory/hcLinearStorage.h"

namespace hc::memory
{
  TEST(LinearStorageTests, AllocationTest)
  {
    LinearStorage storage;
    storage.initialize(2048);
    void* ptr =storage.allocate(61);
    void* ptr2 = storage.allocate(61);
    storage.free(ptr2);
    storage.free(ptr);
  }
}
