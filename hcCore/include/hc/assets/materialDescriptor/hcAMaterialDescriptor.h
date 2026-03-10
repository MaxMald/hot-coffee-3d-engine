#pragma once

#include "hc/assets/hcAsset.h"
#include "hc/graphics/resource/material/hcShadingType.h"
#include "hc/graphics/resource/material/hcMaterialRenderMode.h"

namespace hc
{
  /**
   * Abstract base class for material descriptors.
   *
   * Defines the interface for material descriptors that describe
   * material properties and associated resources. Implementations
   * provide specific material types with their shading models and
   * texture references.
   */
  class HC_CORE_EXPORT AMaterialDescriptor : public Asset
  {
  public:
    virtual ~AMaterialDescriptor() = default;

    /**
     * Gets the shading type used by this material.
     *
     * @return The shading type identifier
     */
    virtual shadingType::Type getShaderType() const = 0;

    /**
     * Gets the file paths of all images used by this material descriptor.
     *
     * This function populates the provided vector with the paths of all images
     * used by the material descriptor. This method does not cleans the vector
     * before adding paths, so the caller should ensure it is empty or handle the
     * existing contents appropriately.
     *
     * @param paths Output parameter to receive the list of image paths.
     */
    virtual void getImagesPaths(Vector<Path>& paths) const = 0;

    /**
     * Gets the render mode of the material, which determines how it should be
     * rendered (e.g., opaque, transparent).
     *
     * @return The material render mode.
     */
    materialRenderMode::Type getRenderMode() const;
    
    /**
     * Sets the render mode of the material, which determines how it should be
     * rendered (e.g., opaque, transparent).
     *
     * @param renderMode The material render mode to set.
     */
    void setRenderMode(materialRenderMode::Type renderMode);

  protected:
    materialRenderMode::Type m_renderMode;

    /**
     * Constructs a material descriptor with the given asset path.
     *
     * @param path The file path to the material descriptor asset
     * @param renderMode The material render mode (default is Opaque)
     */
    AMaterialDescriptor(
      const Path& path,
      materialRenderMode::Type renderMode = materialRenderMode::Type::Opaque
    );
  };
}
