#include "hc/utilities/memory/hcStackStorage.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include "hc/utilities/memory/hcMemory.h"

namespace
{
  struct AllocationHeader
  {
    hc::SizeT previousCurrent;
    hc::UIntPtr previousHeaderAddress;
    hc::SizeT userAlignment;
    hc::SizeT size;
  };
}

namespace hc::memory
{
  StackStorage::StackStorage() :
    m_capacity(0),
    m_current(0),
    m_lastHeaderAddress(0),
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

    if (capacity > SIZE_MAX - alignof(AllocationHeader))
      throw InvalidArgumentException("Capacity is too large.");

    try
    {
      m_memory = AlignedAlloc(capacity, alignof(AllocationHeader));
      m_capacity = capacity;
      m_current = 0;
      m_lastHeaderAddress = 0;
      m_initialized = true;
    }
    catch (...)
    {
      destroy();
      throw;
    }
  }

  bool StackStorage::isInitialized() const noexcept
  {
    return m_initialized;
  }

  SizeT StackStorage::getCapacity() const noexcept
  {
    return m_capacity;
  }

  void* StackStorage::allocate(SizeT size, SizeT alignment)
  {
    if (!m_initialized)
      throw RuntimeErrorException("StackStorage is not initialized.");

    if (size == 0)
      throw InvalidArgumentException("Size must be greater than zero.");

    if (!IsAlignmentPowerOfTwo(alignment))
      throw InvalidArgumentException("Alignment must be a power of two.");

    UIntPtr const baseAddress = reinterpret_cast<UIntPtr>(m_memory);
    UIntPtr const currentAddress = baseAddress + m_current;

    // Block Structure:
    // [PreHeaderPadding][AllocationHeader][PreUserPadding][UserData]

    const SizeT headerAlignment = alignof(AllocationHeader);
    const SizeT preHeaderPadding = AlignAddressAdjustment(currentAddress, headerAlignment);
    const UIntPtr headerAddress = currentAddress + preHeaderPadding;
    const SizeT preUserPadding = AlignAddressAdjustment(headerAddress + sizeof(AllocationHeader), alignment);
    const SizeT requiredSize = preHeaderPadding + sizeof(AllocationHeader) + preUserPadding + size;

    if (m_current > m_capacity || requiredSize > m_capacity - m_current)
      throw std::bad_alloc();

    auto* const header = reinterpret_cast<AllocationHeader*>(currentAddress + preHeaderPadding);
    header->previousCurrent = m_current;
    header->previousHeaderAddress = m_lastHeaderAddress;
    header->userAlignment = alignment;
    header->size = size;

    m_lastHeaderAddress = currentAddress + preHeaderPadding;
    m_current += requiredSize;

    return static_cast<void*>(reinterpret_cast<Byte*>(header) + sizeof(AllocationHeader) + preUserPadding);
  }

  void StackStorage::free(void* ptr) noexcept
  {
    if (!m_initialized)
    {
      HC_ASSERT(false && "StackStorage is not initialized.");
      return;
    }

    if (ptr == nullptr)
      return;

    UIntPtr const base = reinterpret_cast<UIntPtr>(m_memory);
    UIntPtr const userAddress = reinterpret_cast<UIntPtr>(ptr);

    bool isPointerInRange = (userAddress >= base + sizeof(AllocationHeader))
      && (userAddress < base + m_current);

    if (!isPointerInRange)
    {
      HC_ASSERT(false && "Pointer does not belong to StackStorage.");
      return;
    }

    // Check if the pointer is the most recent allocation

    auto* lastRegisteredHeader = reinterpret_cast<AllocationHeader*>(m_lastHeaderAddress);
    const SizeT registeredUserAlignment = lastRegisteredHeader->userAlignment;
    const SizeT registeredPreUserPadding = AlignAddressAdjustment(m_lastHeaderAddress + sizeof(AllocationHeader), registeredUserAlignment);
    UIntPtr const expectedUserAddress = m_lastHeaderAddress + sizeof(AllocationHeader) + registeredPreUserPadding;

    if (userAddress != expectedUserAddress)
    {
      HC_ASSERT(false && "StackStorage can only free the most recent allocation.");
      return;
    }

    m_current = lastRegisteredHeader->previousCurrent;
    m_lastHeaderAddress = lastRegisteredHeader->previousHeaderAddress;
  }

  void StackStorage::reset()
  {
    if (!m_initialized)
      throw RuntimeErrorException("StackStorage is not initialized.");

    m_current = 0;
    m_lastHeaderAddress = 0;
  }

  void StackStorage::destroy() noexcept
  {
    if (m_memory != nullptr)
    {
      AlignedFree(m_memory, alignof(AllocationHeader));
      m_memory = nullptr;
    }

    m_current = 0;
    m_capacity = 0;
    m_lastHeaderAddress = 0;
    m_initialized = false;
  }
}
