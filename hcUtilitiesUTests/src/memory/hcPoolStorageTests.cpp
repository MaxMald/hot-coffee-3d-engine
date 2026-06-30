#include <gtest/gtest.h>
#include "hc/utilities/memory/hcPoolStorage.h"

namespace hc::memory
{
  TEST(PoolStorageTests, UninitializationTests)
  {
    PoolStorage poolStorage;

    EXPECT_THROW(poolStorage.allocate(), RuntimeErrorException);
    EXPECT_THROW(poolStorage.free(nullptr), RuntimeErrorException);
    EXPECT_THROW(poolStorage.owns(nullptr), RuntimeErrorException);
  }

  TEST(PoolStorageTests, InvalidInitializationTests)
  {
    PoolStorage poolStorage;

    EXPECT_THROW(poolStorage.initialize(0, 10), InvalidArgumentException);
    EXPECT_THROW(poolStorage.initialize(10, 0), InvalidArgumentException);
    // Block size must be at least the size of a pointer
    EXPECT_THROW(poolStorage.initialize(sizeof(void*) - 1, 10), InvalidArgumentException);
  }

  TEST(PoolStorageTests, DoubleInitializationTest)
  {
    PoolStorage poolStorage;
    poolStorage.initialize(sizeof(Int64), 10);

    EXPECT_THROW(poolStorage.initialize(sizeof(Int64), 10), RuntimeErrorException);
  }

  TEST(PoolStorageTests, NotOwnedPointerTest)
  {
    PoolStorage poolStorage;
    poolStorage.initialize(sizeof(Int64), 10);
    std::unique_ptr<Int64> externalPtr = std::make_unique<Int64>(42);

    EXPECT_FALSE(poolStorage.owns(externalPtr.get()));
  }

  class PoolStorageWithMultipleBlockSizesTests
    : public ::testing::TestWithParam<SizeT> {};

  TEST_P(PoolStorageWithMultipleBlockSizesTests, InitalizationTest)
  {
    const SizeT blockSize = GetParam();

    PoolStorage poolStorage;
    poolStorage.initialize(blockSize, 10);

    EXPECT_EQ(poolStorage.getBlockSize(), blockSize);
    EXPECT_EQ(poolStorage.getUsedBlockCount(), 0);
    EXPECT_EQ(poolStorage.getFreeBlockCount(), 10);
  }

  TEST_P(PoolStorageWithMultipleBlockSizesTests, AllocationAndDeallocationTest)
  {
    const SizeT blockSize = GetParam();

    PoolStorage poolStorage;
    poolStorage.initialize(blockSize, 10);

    void* block1 = poolStorage.allocate();
    void* block2 = poolStorage.allocate();

    EXPECT_TRUE(poolStorage.owns(block1));
    EXPECT_TRUE(poolStorage.owns(block2));
    EXPECT_EQ(poolStorage.getUsedBlockCount(), 2);
    EXPECT_EQ(poolStorage.getFreeBlockCount(), 8);

    poolStorage.free(block1);

    EXPECT_EQ(poolStorage.getUsedBlockCount(), 1);
    EXPECT_EQ(poolStorage.getFreeBlockCount(), 9);

    poolStorage.free(block2);

    EXPECT_EQ(poolStorage.getUsedBlockCount(), 0);
    EXPECT_EQ(poolStorage.getFreeBlockCount(), 10);
  }

  TEST_P(PoolStorageWithMultipleBlockSizesTests, ExhaustedBlocksTest)
  {
    const SizeT blockSize = GetParam();

    PoolStorage poolStorage;
    poolStorage.initialize(blockSize, 2);

    void* block1 = poolStorage.allocate();
    void* block2 = poolStorage.allocate();

    EXPECT_THROW(poolStorage.allocate(), RuntimeErrorException);

    poolStorage.free(block1);
    poolStorage.free(block2);
  }

  INSTANTIATE_TEST_SUITE_P(
    DifferentBlockSizes,
    PoolStorageWithMultipleBlockSizesTests,
    ::testing::Values(64, 128, 256, 512, 1024)
  );
}
