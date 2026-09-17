#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  namespace memory
  {
    /**
     * @brief Allocates a block of memory of the specified size.
     *
     * You must free the allocated memory using Free() when it is no longer needed.
     *
     * @param size The size of the memory block to allocate in bytes.
     *
     * @return A pointer to the allocated memory block.
     */
    HC_UTILITY_EXPORT void* Alloc(SizeT size);

    /**
     * @brief Allocates a block of memory of the specified size with the specified
     * alignment.
     *
     * You must free the allocated memory using AlignedFree() when it is no longer needed.
     *
     * @param size The size of the memory block to allocate in bytes.
     * @param alignment The alignment of the memory block. Must be a power of two.
     *
     * @return A pointer to the allocated memory block.
     */
    HC_UTILITY_EXPORT void* AlignedAlloc(SizeT size, SizeT alignment);

    /**
     * @brief Frees a previously allocated block of memory.
     *
     * You must ensure that the memory was allocated using Alloc() before calling this
     * function.
     *
     * @param ptr A pointer to the memory block to free. If nullptr, no action is taken.
     */
    HC_UTILITY_EXPORT void Free(void* ptr);

    /**
     * @brief Frees a previously allocated block of memory that was allocated with
     * AlignedAlloc.
     *
     * You must ensure that the memory was allocated using AlignedAlloc() before calling
     * this function.
     *
     * @param ptr A pointer to the memory block to free. If nullptr, no action is taken.
     * @param alignment The alignment of the memory block. Must be a power of two.
     */
    HC_UTILITY_EXPORT void AlignedFree(void* ptr, SizeT alignment);

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
     * Check if the given alignment is a power of two.
     *
     * @param alignment The alignment to check.
     *
     * @return True if the alignment is a power of two, false otherwise.
     */
    inline bool IsAlignmentPowerOfTwo(SizeT alignment)
    {
      return (alignment != 0) && ((alignment & (alignment - 1)) == 0);
    }

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
     * Calculate the adjustment (in bytes) needed to align the given address to the
     * specified alignment.
     *
     * @param address The address to align.
     * @param alignment The alignment to align to. Must be a power of two.
     *
     * @return The adjustment needed to align the address in bytes.
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
