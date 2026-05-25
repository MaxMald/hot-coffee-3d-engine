#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/material/hcIMaterial.h"

namespace hc
{
  class AMaterialDescriptor;
  class ITexture;
  class IMaterial;
  class UnlitMaterial;
  class UnlitMaterialDescriptor;
  class BlinnPhongMaterial;
  class BlinnPhongMaterialDescriptor;

  class HC_CORE_EXPORT IMaterialManager : public NonCopyable
  {
  public:
    virtual ~IMaterialManager() = default;

    /**
     * @brief Initializes the material manager and loads any necessary resources.
     */
    virtual void initialize() = 0;

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
     * @brief Creates a material from a material descriptor object.
     *
     * @param descriptor Shared pointer to the material descriptor.
     *
     * @return Shared pointer to the created material.
     */
    virtual SharedPtr<IMaterial> createMaterialFromDescriptor(
      SharedPtr<AMaterialDescriptor> descriptor
    ) = 0;

    /**
     * @brief Creates a default unlit material.
     *
     * @param descriptor Reference to the unlit material descriptor containing the
     * properties for the unlit material to be created.
     * 
     * @return Shared pointer to the created unlit material.
     */
    virtual SharedPtr<UnlitMaterial> createUnlitMaterial(
      const UnlitMaterialDescriptor& descriptor
    ) = 0;
    
    /**
     * @brief Creates a Blinn-Phong material.
     *
     * @param descriptor Reference to the Blinn-Phong material descriptor containing the
     * properties for the Blinn-Phong material to be created.
     * 
     * @return Shared pointer to the created Blinn-Phong material.
     */
    virtual SharedPtr<BlinnPhongMaterial> createBlinnPhongMaterial(
      const BlinnPhongMaterialDescriptor& descriptor
    ) = 0;

    /**
     * @brief Gets all materials.
     *
     * @return A constant reference to the vector of materials.
     */
    virtual const Vector<SharedPtr<IMaterial>>& getMaterials() const = 0;

    /**
     * @brief Gets the default textures used when a material does not have a specific
     * texture assigned.
     */
    virtual const SharedPtr<ITexture>& getDefaultAlbedoTexture() const = 0;

    /**
     * @brief Gets the default normal texture used when a material does not have a
     * specific normal texture assigned.
     */
    virtual const SharedPtr<ITexture>& getDefaultNormalTexture() const = 0;

    /**
     * @brief Gets the default specular texture used when a material does not have a
     * specific specular texture assigned.
     */
    virtual const SharedPtr<ITexture>& getDefaultSpecularTexture() const = 0;

    /**
     * @brief Clears all materials.
     */
    virtual void clear() = 0;
  };
}
