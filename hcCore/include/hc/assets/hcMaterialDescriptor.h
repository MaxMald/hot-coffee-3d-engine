#pragma once

#include "hc/assets/hcAsset.h"
#include "hc/graphics/resource/material/hcShadingType.h"

namespace hc
{
  /**
   * @brief Describes the properties and configuration for a material asset.
   */
  class HC_CORE_EXPORT MaterialDescriptor : public Asset
  {
  public:
    virtual ~MaterialDescriptor() = default;

    /**
     * @brief Gets the shader type associated with this material descriptor.
     * 
     * @return The shader type.
     */
    shadingType::Type getShaderType() const;

    /**
     * @brief Gets the paths of images used by this material descriptor.
     *
     * @return A vector of image paths.
     */
    virtual Vector<Path> getImagesPaths() const = 0;

  protected:
    shadingType::Type m_shaderType;
    
    MaterialDescriptor(shadingType::Type type);
  };
}
