#pragma once

#include "hc/hcAssetManagerPrerequisites.h"
#include "hc/assets/model/hcModelAssetManager.h"
#include "hc/assets/image/hcImageAssetManager.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptorAssetManager.h"

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
     * Retrieves the model asset manager.
     *
     * @return Reference to the model asset manager instance
     */
    IModelAssetManager& getModelAssetManager() override;

    /**
     * Retrieves the image asset manager.
     *
     * @return Reference to the image asset manager instance
     */
    IImageAssetManager& getImageAssetManager() override;

    /**
     * Retrieves the material descriptor asset manager.
     *
     * @return Reference to the material descriptor asset manager
     */
    IMaterialDescriptorAssetManager& getMaterialDescriptorAssetManager() override;

    /**
     * Clears all asset groups from all sub-managers.
     */
    void clear() override;

    /**
     * Destroys the asset manager, clearing all assets and resources.
     */
    void destroy() override;

    /**
     * Sets the root path for asset loading.
     *
     * @param rootPath The root directory path where assets are located
     */
    void setRootPath(const Path& rootPath);

    /**
     * Gets the root path for asset loading.
     *
     * @returns The root directory path where assets are located
     */
    const Path& getRootPath() const;

    /**
     * Checks if a root path has been set for asset loading.
     *
     * @return true if a root path is set, false otherwise
     */
    bool hasRootPath() const;

  private:
    MaterialDescriptorAssetManager m_materialDescriptorAssetManager;
    ModelAssetManager m_modelAssetManager;
    ImageAssetManager m_imageAssetManager;
    Path m_rootPath;
  };
}
