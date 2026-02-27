#pragma once

#include "hc/assets/hcATypedAssetManager.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

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
    public ATypedAssetManager<AMaterialDescriptor>
  {
  public:
    virtual ~IMaterialDescriptorAssetManager() = default;

    /**
     * Retrieves the default material descriptor.
     *
     * @return A shared pointer to the default material descriptor asset
     */
    virtual SharedPtr<AMaterialDescriptor> getDefault() const = 0;

  protected:
    IMaterialDescriptorAssetManager() = default;
  };
}
