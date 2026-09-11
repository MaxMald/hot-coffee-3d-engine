#pragma once

#include "hc/assets/hcATypedAssetManager.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptor.h"

namespace hc
{
  /**
   * Interface for managing material descriptor assets.
   *
   * Extends the typed asset manager to provide specialized functionality
   * for loading and managing material descriptors, which define the properties
   * and resources used by materials in the graphics engine.
   */
  class HC_CORE_EXPORT IMaterialDescriptorAssetManager :
    public ATypedAssetManager<MaterialDescriptor>
  {
  public:
    virtual ~IMaterialDescriptorAssetManager() = default;

    /**
     * Retrieves the default material descriptor.
     *
     * @return A shared pointer to the default material descriptor asset
     */
    virtual SharedPtr<MaterialDescriptor> getDefault() const = 0;

    /**
     * Saves a material descriptor to the specified path.
     *
     * @param path The file path where the descriptor should be saved
     * @param descriptor The material descriptor to save
     */
    virtual void save(const Path& path, const MaterialDescriptor& descriptor) = 0;

  protected:
    IMaterialDescriptorAssetManager() = default;
  };
}
