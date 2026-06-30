#include "hc/utilities/memory/hcLinearStorage.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace
{
  struct alignas(std::max_align_t) AllocationHeader
  {
    hc::SizeT previousCurrent;
    hc::SizeT size;
  };

  static_assert(sizeof(AllocationHeader) % alignof(std::max_align_t) == 0);

  hc::SizeT AlignForwardAdjustment(const void* address, hc::SizeT alignment)
  {
    const hc::SizeT mask = alignment - 1;
    const hc::SizeT misalignment = reinterpret_cast<std::uintptr_t>(address) & mask;
    return misalignment == 0 ? 0 : alignment - misalignment;
  }
}

namespace hc::memory
{
  LinearStorage::LinearStorage() :
    m_capacity(0),
    m_current(0),
    m_memory(nullptr),
    m_initialized(false)
  {}

  LinearStorage::~LinearStorage()
  {
    destroy();
  }

  void LinearStorage::initialize(SizeT capacity)
  {
    if (m_initialized)
      throw RuntimeErrorException("LinearStorage is already initialized.");

    if (capacity == 0)
      throw InvalidArgumentException("Capacity must be greater than zero.");

    m_memory = std::malloc(capacity);
    if (!m_memory)
      throw std::bad_alloc();

    m_capacity = capacity;
    m_current = 0;
    m_initialized = true;
  }

  bool LinearStorage::isInitialized() const noexcept
  {
    return m_initialized;
  }

  SizeT LinearStorage::getCapacity() const noexcept
  {
    return m_capacity;
  }

  void* LinearStorage::allocate(SizeT size)
  {
    if (!m_initialized)
      throw RuntimeErrorException("LinearStorage is not initialized.");

    if (size == 0)
      throw InvalidArgumentException("Size must be greater than zero.");

    Byte* const base = static_cast<Byte*>(m_memory);
    Byte* const current = base + m_current;

    const SizeT headerAlignment = alignof(AllocationHeader);
    const SizeT headerOffset = AlignForwardAdjustment(current, headerAlignment);
    const SizeT requiredSize = headerOffset + sizeof(AllocationHeader) + size;

    if (m_current + requiredSize > m_capacity)
      throw std::bad_alloc();

    auto* const header = reinterpret_cast<AllocationHeader*>(current + headerOffset);
    header->previousCurrent = m_current;
    header->size = size;

    m_current += requiredSize;

    return static_cast<void*>(reinterpret_cast<Byte*>(header) + sizeof(AllocationHeader));
  }

  void LinearStorage::free(void* ptr)
  {
    if (!m_initialized)
      throw RuntimeErrorException("LinearStorage is not initialized.");

    if (ptr == nullptr)
      return;

    Byte* const base = static_cast<Byte*>(m_memory);
    Byte* const userPtr = static_cast<Byte*>(ptr);

    if (userPtr < base + sizeof(AllocationHeader) || userPtr > base + m_current)
      throw RuntimeErrorException("Pointer does not belong to LinearStorage.");

    auto* const header = reinterpret_cast<const AllocationHeader*>(userPtr - sizeof(AllocationHeader));
    const Byte* const headerPtr = reinterpret_cast<const Byte*>(header);
    const SizeT headerOffset = static_cast<SizeT>(headerPtr - base);
    const SizeT expectedHeaderOffset = header->previousCurrent + AlignForwardAdjustment(base + header->previousCurrent, alignof(AllocationHeader));

    if (headerOffset != expectedHeaderOffset || headerOffset + sizeof(AllocationHeader) + header->size != m_current)
      throw RuntimeErrorException("LinearStorage can only free the most recent allocation.");

    m_current = header->previousCurrent;
  }

  void LinearStorage::reset()
  {
    if (!m_initialized)
      throw RuntimeErrorException("LinearStorage is not initialized.");

    m_current = 0;
  }

  void LinearStorage::destroy() noexcept
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
