#include <gtest/gtest.h>
#include "hc/utilities/memory/hcStackStorage.h"

namespace hc::memory
{
  TEST(StackStorageTests, AllocationTest)
  {
    StackStorage storage;
    storage.initialize(2048);
    void* ptr =storage.allocate(61);
    void* ptr2 = storage.allocate(61);
    storage.free(ptr2);
    storage.free(ptr);
  }
}
