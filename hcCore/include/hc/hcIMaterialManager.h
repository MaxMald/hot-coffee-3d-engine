#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIMaterial.h"

namespace hc
{
  class MaterialDescriptor;
  class IMaterial;

  class IMaterialManager : public NonCopyable
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
     * @brief Clears all cached materials.
     */
    virtual void clear() = 0;
  };
}
