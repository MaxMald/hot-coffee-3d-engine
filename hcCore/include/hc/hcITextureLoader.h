#pragma once

#include "hc/hcTexture.h"

namespace hc
{
  /**
   * @brief Interface for texture loaders.
   */
  class HC_CORE_EXPORT ITextureLoader
  {
  public:
    virtual ~ITextureLoader() = default;

    /**
     * @brief Loads a texture from the specified file path.
     * 
     * @param filePath Path to the texture file.
     * 
     * @return Shared pointer to the loaded Texture object.
     */
    virtual SharedPtr<Texture> loadTexture(const Path& filePath) = 0;
  };
}
