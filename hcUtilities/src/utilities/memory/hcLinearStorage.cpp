#include "hc/utilities/memory/hcStackStorage.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include "hc/utilities/memory/hcMemory.h"

namespace
{
  struct alignas(std::max_align_t) AllocationHeader
  {
    hc::SizeT previousCurrent;
    hc::SizeT size;
  };

  static_assert(sizeof(AllocationHeader) % alignof(std::max_align_t) == 0);
}

namespace hc::memory
{
  StackStorage::StackStorage() :
    m_capacity(0),
    m_current(0),
    m_memory(nullptr),
    m_initialized(false)
  {}

  StackStorage::~StackStorage()
  {
    destroy();
  }

  void StackStorage::initialize(SizeT capacity)
  {
    if (m_initialized)
      throw RuntimeErrorException("StackStorage is already initialized.");

    if (capacity == 0)
      throw InvalidArgumentException("Capacity must be greater than zero.");

    m_memory = std::malloc(capacity);
    if (!m_memory)
      throw std::bad_alloc();

    m_capacity = capacity;
    m_current = 0;
    m_initialized = true;
  }

  bool StackStorage::isInitialized() const noexcept
  {
    return m_initialized;
  }

  SizeT StackStorage::getCapacity() const noexcept
  {
    return m_capacity;
  }

  void* StackStorage::allocate(SizeT size)
  {
    if (!m_initialized)
      throw RuntimeErrorException("StackStorage is not initialized.");

    if (size == 0)
      throw InvalidArgumentException("Size must be greater than zero.");

    UIntPtr const base = reinterpret_cast<UIntPtr>(m_memory);
    UIntPtr const current = base + m_current;

    const SizeT headerAlignment = alignof(AllocationHeader);
    const SizeT headerOffset = AlignAddressAdjustment(current, headerAlignment);
    const SizeT requiredSize = headerOffset + sizeof(AllocationHeader) + size;

    if (m_current > m_capacity || requiredSize > m_capacity - m_current)
      throw std::bad_alloc();

    auto* const header = reinterpret_cast<AllocationHeader*>(current + headerOffset);
    header->previousCurrent = m_current;
    header->size = size;

    m_current += requiredSize;

    return static_cast<void*>(reinterpret_cast<Byte*>(header) + sizeof(AllocationHeader));
  }

  void StackStorage::free(void* ptr) noexcept
  {
    HC_ASSERT(m_initialized && "StackStorage is not initialized.");

    if (ptr == nullptr)
      return;

    UIntPtr const base = reinterpret_cast<UIntPtr>(m_memory);
    UIntPtr const userPtr = reinterpret_cast<UIntPtr>(ptr);

    HC_ASSERT(userPtr >= base + sizeof(AllocationHeader) && userPtr < base + m_current
      && "Pointer does not belong to StackStorage.");

    auto* const header = reinterpret_cast<const AllocationHeader*>(userPtr - sizeof(AllocationHeader));
    const UIntPtr headerPtr = reinterpret_cast<UIntPtr>(header);
    const SizeT headerOffset = static_cast<SizeT>(headerPtr - base);
    const SizeT expectedHeaderOffset = header->previousCurrent
      + AlignAddressAdjustment(base + header->previousCurrent, alignof(AllocationHeader));

    HC_ASSERT(headerOffset == expectedHeaderOffset && headerOffset + sizeof(AllocationHeader) + header->size == m_current
      && "StackStorage can only free the most recent allocation.");

    m_current = header->previousCurrent;
  }

  void StackStorage::reset()
  {
    if (!m_initialized)
      throw RuntimeErrorException("StackStorage is not initialized.");

    m_current = 0;
  }

  void StackStorage::destroy() noexcept
  {
    if (m_memory != nullptr)
    {
      std::free(m_memory);
      m_memory = nullptr;
    }

    m_current = 0;
    m_capacity = 0;
    m_initialized = false;
  }
}
