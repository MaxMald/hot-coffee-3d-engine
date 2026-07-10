#include <gtest/gtest.h>
#include <list>
#include "hc/utilities/memory/hcPoolStorage.h"
#include "hc/utilities/memory/hcPoolAllocator.h"

namespace hc
{
  namespace memory
  {
    TEST(PoolAllocatorTests, SetAndGetPoolStorageTest)
    {
      PoolStorage poolStorage;
      poolStorage.initialize(sizeof(Int64), 10);
      PoolAllocator<Int64> allocator;

      allocator.setPoolStorage(&poolStorage);

      // Assert that the allocator's pool storage is set correctly
      ASSERT_EQ(allocator.getPoolStorage(), &poolStorage);
    }

    TEST(PoolAllocatorTests, ListAllocationTest)
    {
      // Calculate the block size needed for a list node containing an Int64 value
      hc::SizeT pointerSize = sizeof(void*);
      hc::SizeT blockSize = (2 * pointerSize + sizeof(Int64) + alignof(Int64) - 1) & ~(alignof(Int64) - 1);

      PoolStorage poolStorage;
      poolStorage.initialize(blockSize, 14);

      PoolAllocator<Int64> allocator;
      allocator.setPoolStorage(&poolStorage);

      std::list<Int64, PoolAllocator<Int64>> numbers(allocator);

      SizeT usedBlocksByList = poolStorage.getUsedBlockCount();

      numbers.push_back(static_cast<Int64>(42));
      numbers.push_back(static_cast<Int64>(100));
      numbers.push_back(static_cast<Int64>(200));

      // Assert that the list has the correct number of elements
      ASSERT_EQ(numbers.size(), 3);

      // Assert that the values are correctly stored in the list
      auto iterator = numbers.begin();
      ASSERT_EQ(*iterator, 42);
      iterator++;
      ASSERT_EQ(*iterator, 100);
      iterator++;
      ASSERT_EQ(*iterator, 200);

      // Assert that the used block count has increased by 3 (one for each element added)
      ASSERT_EQ(poolStorage.getUsedBlockCount(), usedBlocksByList + 3);
    }

    TEST(PoolAllocatorTests, ListDeallocationTest)
    {
      // Calculate the block size needed for a list node containing an Int64 value
      hc::SizeT pointerSize = sizeof(void*);
      hc::SizeT blockSize = (2 * pointerSize + sizeof(Int64) + alignof(Int64) - 1) & ~(alignof(Int64) - 1);

      PoolStorage poolStorage;
      poolStorage.initialize(blockSize, 14);
      PoolAllocator<Int64> allocator;
      allocator.setPoolStorage(&poolStorage);

      std::list<Int64, PoolAllocator<Int64>> numbers(allocator);
      SizeT usedBlocksByList = poolStorage.getUsedBlockCount();

      numbers.push_back(static_cast<Int64>(42));
      numbers.push_back(static_cast<Int64>(100));
      numbers.push_back(static_cast<Int64>(200));

      // Remove one element from the list and check if the used block count decreases
      numbers.pop_back();
      ASSERT_EQ(poolStorage.getUsedBlockCount(), usedBlocksByList + 2);

      // Remove the rest of the elements and check if the used block count decreases accordingly
      numbers.clear();
      ASSERT_EQ(poolStorage.getUsedBlockCount(), usedBlocksByList);
    }

    TEST(PoolAllocatorTests, InvalidAllocationCount)
    {
      PoolStorage poolStorage;
      poolStorage.initialize(sizeof(Int64), 10);
      PoolAllocator<Int64> allocator;
      allocator.setPoolStorage(&poolStorage);

      // Attempt to allocate more than one object at a time, which should throw an exception
      EXPECT_THROW(allocator.allocate(2), InvalidArgumentException);
    }
  }
}
