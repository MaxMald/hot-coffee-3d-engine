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

    /**
     * Shift the given address upwards if/as necessary to make it a multiple of the
     * specified alignment.
     *
     * @param address The address to align.
     * @param alignment The alignment to align to. Must be a power of two.
     *
     * @return The aligned address.
     */
    inline UIntPtr AlignAddress(UIntPtr address, SizeT alignment)
    {
      const UIntPtr mask = alignment - 1;
      assert((alignment & mask) == 0 && "Alignment must be a power of two.");
      return (address + mask) & ~mask;
    }

    /**
     * Calculate the adjustment needed to align the given address to the specified
     * alignment.
     *
     * @param address The address to align.
     * @param alignment The alignment to align to. Must be a power of two.
     *
     * @return The adjustment needed to align the address.
     */
    inline SizeT AlignAddressAdjustment(UIntPtr address, SizeT alignment)
    {
      const UIntPtr mask = alignment - 1;
      assert((alignment & mask) == 0 && "Alignment must be a power of two.");
      const UIntPtr misalignment = address & mask;
      return misalignment == 0 ? 0 : alignment - misalignment;
    }

    /**
     * @brief Aligns a pointer to the specified alignment.
     *
     * @param pointer The pointer to align.
     * @param alignment The alignment to align to. Must be a power of two.
     *
     * @return The aligned pointer.
     */
    template<typename T>
    inline T* AlignPointer(T* pointer, SizeT alignment)
    {
      const UIntPtr address = reinterpret_cast<UIntPtr>(pointer);
      const UIntPtr addressAligned = AlignAddress(address, alignment);
      return reinterpret_cast<T*>(addressAligned);
    }
  }
}
