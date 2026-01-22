#pragma once

#include "hc/hcAsset.h"
#include "hc/hcShaderType.h"

namespace hc
{
  /**
   * @brief Describes the properties and configuration for a material asset.
   */
  class HC_CORE_EXPORT MaterialDescriptor :
    public Asset
  {
  public:
    /**
     * @brief Constructs a MaterialDescriptor with the specified shader type.
     * 
     * @param type The shader type associated with this material descriptor.
     */
    MaterialDescriptor(shaderType::Type type);
    virtual ~MaterialDescriptor() = default;

    /**
     * @brief Gets the shader type associated with this material descriptor.
     * 
     * @return The shader type.
     */
    shaderType::Type getShaderType() const;

    /**
     * @brief Gets the paths of images used by this material descriptor.
     *
     * @return A vector of image paths.
     */
    virtual Vector<Path> getImagesPaths() const = 0;

  protected:
    shaderType::Type m_shaderType;
  };
}
