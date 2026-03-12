#pragma once

#include "hc/graphics/resource/material/hcIMaterialManager.h"
#include "hc/graphics/resource/hcResourcesCache.h"

namespace hc
{
  class ITextureManager;
  class IShaderProgramManager;
  class IAssetManager;
  class MaterialFactoriesManager;

  /**
   * @brief Concrete implementation of the IMaterialManager interface that
   * manages materials using a cache and a set of material factories.
   */
  class HC_CORE_EXPORT MaterialManager :
    public IMaterialManager,
    private ResourcesCache<Id, IMaterial>
  {
  public:
    MaterialManager(
      IAssetManager& assetManager,
      ITextureManager& textureManager,
      IShaderProgramManager& shaderProgramManager,
      UniquePtr<MaterialFactoriesManager> materialFactoriesManager
    );
    virtual ~MaterialManager() = default;

    /**
     * @brief Creates a material from a material descriptor file.
     *
     * @param materialDescriptorPath Path to the material descriptor file.
     *
     * @return Shared pointer to the created material.
     */
    SharedPtr<IMaterial> createMaterialFromFile(
      const Path& materialDescriptorPath
    ) override;

    /**
     * @brief Creates a material from a material descriptor object.
     *
     * @param descriptor Shared pointer to the material descriptor.
     *
     * @return Shared pointer to the created material.
     */
    SharedPtr<IMaterial> createMaterialFromDescriptor(
      SharedPtr<AMaterialDescriptor> descriptor
    ) override;

    /**
     * @brief Gets all cached materials.
     *
     * @return A constant reference to the vector of cached materials.
     */
    const Vector<SharedPtr<IMaterial>>& getMaterials() const override;

    /**
     * @brief Clears all cached materials.
     */
    void clear();

  private:
    /**
     * @brief Static counter for generating unique material IDs.
     */
    static UInt16 s_nextMaterialId;

    UniquePtr<MaterialFactoriesManager> m_materialFactoriesManager;
    IAssetManager& m_assetManager;
    IShaderProgramManager& m_shaderProgramManager;
    ITextureManager& m_textureManager;
    Vector<SharedPtr<IMaterial>> m_materials;
  };
}
