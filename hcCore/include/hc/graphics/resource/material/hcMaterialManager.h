#pragma once

#include "hc/graphics/resource/material/hcIMaterialManager.h"

namespace hc
{
  class ITexture;
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
    * @copydoc IMaterialManager::initialize
    */
    void initialize() override;

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
     * @copydoc IMaterialManager::createBlinnPhongMaterial
     */
    SharedPtr<BlinnPhongMaterial> createBlinnPhongMaterial(
      const BlinnPhongMaterialDescriptor& descriptor
    ) override;

    /**
     * @copydoc IMaterialManager::getMaterials
     */
    const Vector<SharedPtr<IMaterial>>& getMaterials() const override;

    /**
     * @copydoc IMaterialManager::getDefaultAlbedoTexture
     */
    const SharedPtr<ITexture>& getDefaultAlbedoTexture() const override;

    /** 
     * @copydoc IMaterialManager::getDefaultNormalTexture
     */
    const SharedPtr<ITexture>& getDefaultNormalTexture() const override;

    /**
     * @copydoc IMaterialManager::getDefaultSpecularTexture
     */
    const SharedPtr<ITexture>& getDefaultSpecularTexture() const override;

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
    SharedPtr<ITexture> m_whiteTexture;
    SharedPtr<ITexture> m_defaultNormalTexture;

    UInt16 generateMaterialId();
    void createDefaultTextures();
    SharedPtr<ITexture> getTextureFromPath(const Path& texturePath);
  };
}
