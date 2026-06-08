#pragma once

#include "hc/hcAssetManagerPrerequisites.h"
#include "hc/assets/model/hcModelAssetManager.h"
#include "hc/assets/image/hcImageAssetManager.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptorAssetManager.h"
#include "hc/assets/cubeMapDescriptor/hcCubeMapDescriptorAssetManager.h"

namespace hc
{
  /**
   * Concrete implementation of the central asset manager.
   *
   * Owns and coordinates all specialized asset managers.
   */
  class HC_ASSET_MANAGER_EXPORT AssetManager : public IAssetManager
  {
  public:
    /** Constructs the asset manager and initializes all sub-managers */
    AssetManager();

    /** Virtual destructor */
    virtual ~AssetManager() = default;

    /**
     * @copydoc IAssetManager::getModelAssetManager
     */
    IModelAssetManager& getModelAssetManager() override;

    /**
     * @copydoc IAssetManager::getImageAssetManager
     */
    IImageAssetManager& getImageAssetManager() override;

    /**
     * @copydoc IAssetManager::getMaterialDescriptorAssetManager
     */
    IMaterialDescriptorAssetManager& getMaterialDescriptorAssetManager() override;

    /**
     * @copydoc IAssetManager::getCubeMapDescriptorAssetManager
     */
    ICubeMapDescriptorAssetManager& getCubeMapDescriptorAssetManager() override;

    /**
     * @copydoc IAssetManager::clear
     */
    void clear() override;

    /**
     * @copydoc IAssetManager::destroy
     */
    void destroy() override;

    /**
     * @copydoc IAssetManager::setRootPath
     */
    void setRootPath(const Path& rootPath) override;

    /**
     * @copydoc IAssetManager::getRootPath
     */
    const Path& getRootPath() const override;

    /**
     * @copydoc IAssetManager::hasRootPath
     */
    bool hasRootPath() const override;

  private:
    MaterialDescriptorAssetManager m_materialDescriptorAssetManager;
    ModelAssetManager m_modelAssetManager;
    ImageAssetManager m_imageAssetManager;
    CubeMapDescriptorAssetManager m_cubeMapDescriptorAssetManager;
    Path m_rootPath;
  };
}
