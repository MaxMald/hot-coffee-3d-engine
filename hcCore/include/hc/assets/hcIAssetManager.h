#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/assets/image/hcIImageAssetManager.h"
#include "hc/assets/model/hcIModelAssetManager.h"
#include "hc/assets/materialDescriptor/hcIMaterialDescriptorAssetManager.h"

namespace hc
{
  /**
   * Central interface for managing all asset types in the engine.
   *
   * Provides access to specialized asset managers. Handles the lifecycle of
   * all assets and ensures proper cleanup on destruction.
   */
  class IAssetManager : public NonCopyable
  {
  public:
    virtual ~IAssetManager() = default;

    /**
     * Retrieves the model asset manager.
     *
     * @return Reference to the model asset manager instance
     */
    virtual IModelAssetManager& getModelAssetManager() = 0;

    /**
     * Retrieves the image asset manager.
     *
     * @return Reference to the image asset manager instance
     */
    virtual IImageAssetManager& getImageAssetManager() = 0;

    /**
     * Retrieves the material descriptor asset manager.
     *
     * @return Reference to the material descriptor asset manager
     */
    virtual IMaterialDescriptorAssetManager& getMaterialDescriptorAssetManager() = 0;

    /**
     * Clears all asset groups from the manager.
     */
    virtual void clear() = 0;

    /**
     * Destroys the asset manager, clearing all assets and loaders.
     */
    virtual void destroy() = 0;

  protected:
    IAssetManager() = default;
  };
}
