#pragma once

#include "hc/graphics/resource/material/hcIMaterialManager.h"

namespace hc
{
  class ITextureManager;
  class IShaderProgramManager;
  class IAssetManager;
  class MaterialFactoriesManager;

  /**
   * @brief Concrete implementation of the IMaterialManager interface that
   * manages materials using a set of material factories.
   */
  class HC_CORE_EXPORT MaterialManager : public IMaterialManager
  {
  public:
    MaterialManager(
      IAssetManager& assetManager,
      ITextureManager& textureManager,
      IShaderProgramManager& shaderProgramManager
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
     * @brief Creates a default unlit material.
     *
     * @return Shared pointer to the created unlit material.
     */
    SharedPtr<UnlitMaterial> createUnlitMaterial(
      const UnlitMaterialDescriptor& descriptor
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

    IAssetManager& m_assetManager;
    IShaderProgramManager& m_shaderProgramManager;
    ITextureManager& m_textureManager;
    Vector<SharedPtr<IMaterial>> m_materials;

    UInt16 generateMaterialId();
  };
}
