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
     * @copydoc IMaterialManager::createMaterialFromFile
     */
    SharedPtr<IMaterial> createMaterialFromFile(
      const Path& materialDescriptorPath
    ) override;

    /**
     * @copydoc IMaterialManager::createMaterialFromDescriptor
     */
    SharedPtr<IMaterial> createMaterialFromDescriptor(
      SharedPtr<AMaterialDescriptor> descriptor
    ) override;

    /**
     * @copydoc IMaterialManager::createUnlitMaterial
     */
    SharedPtr<UnlitMaterial> createUnlitMaterial(
      const UnlitMaterialDescriptor& descriptor
    ) override;

    /**
     * @copydoc IMaterialManager::getMaterials
     */
    const Vector<SharedPtr<IMaterial>>& getMaterials() const override;

    /**
     * @copydoc IMaterialManager::clear
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
