#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/material/hcIMaterial.h"

namespace hc
{
  class MaterialDescriptor;
  class IMaterial;

  class HC_CORE_EXPORT IMaterialManager : public NonCopyable
  {
  public:
    virtual ~IMaterialManager() = default;

    /**
     * @brief Creates a material from a material descriptor file.
     *
     * @param materialDescriptorPath Path to the material descriptor file.
     *
     * @return Shared pointer to the created material.
     */
    virtual SharedPtr<IMaterial> createMaterialFromFile(
      const Path& materialDescriptorPath
    ) = 0;

    /**
     * @brief Creates a material from a material descriptor key.
     *
     * @param materialDescriptorKey Key identifying the material descriptor.
     *
     * @return Shared pointer to the created material.
     */
    virtual SharedPtr<IMaterial> createMaterialFromDescriptor(
      const String& materialDescriptorKey
    ) = 0;

    /**
     * @brief Creates a material from a material descriptor object.
     *
     * @param descriptor Shared pointer to the material descriptor.
     *
     * @return Shared pointer to the created material.
     */
    virtual SharedPtr<IMaterial> createMaterialFromDescriptor(
      SharedPtr<MaterialDescriptor> descriptor
    ) = 0;

    /**
     * @brief Gets all cached materials.
     *
     * @return A constant reference to the vector of cached materials.
     */
    virtual const Vector<SharedPtr<IMaterial>>& getMaterials() const = 0;

    /**
     * @brief Clears all cached materials.
     */
    virtual void clear() = 0;
  };
}
