#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace graphics::generators
  {
    struct CubeMapGeneratorSettings;
  }

  class ICubeMap;
  class IGraphicsManager;
  class IAssetManager;

  /**
   * @brief Factory class for creating cube maps from descriptor files.
   */
  struct HC_CORE_EXPORT CubeMapFactory
  {
    /**
     * @brief Creates a cube map from a descriptor file.
     *
     * This function loads a cube map descriptor from the specified source path,
     * retrieves the corresponding images for each face of the cube map, and
     * initializes a new ICubeMap instance with those images.
     *
     * @param cubeMapDescriptorSourcePath The path to the cube map descriptor file.
     * @param assetManager Reference to the asset manager for loading assets.
     * @param graphicsManager Reference to the graphics manager for creating the cube map.
     * @return A shared pointer to the created ICubeMap instance.
     *
     * @throw RuntimeErrorException if any of the images fail to load or if the
     * cube map fails to initialize.
     */
    static SharedPtr<ICubeMap> CreateFromDescriptor(
      const Path& cubeMapDescriptorSourcePath,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    );

    /**
     * @brief Creates a cube map from an equirectangular image.
     *
     * This function loads an equirectangular image from the specified source path,
     * converts it into a cube map, and initializes a new ICubeMap instance with the
     * generated cube map faces.
     *
     * @param equirectangularImageSourcePath The path to the equirectangular image file.
     * @param settings The settings for generating the cube map from the equirectangular image.
     * @param assetManager Reference to the asset manager for loading assets.
     * @param graphicsManager Reference to the graphics manager for creating the cube map.
     * @return A shared pointer to the created ICubeMap instance.
     *
     * @throw RuntimeErrorException if the image fails to load or if the cube map
     * fails to initialize.
     */
    static SharedPtr<ICubeMap> CreateFromEquirectangularImage(
      const Path& equirectangularImageSourcePath,
      const graphics::generators::CubeMapGeneratorSettings& settings,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    );
  };
}
