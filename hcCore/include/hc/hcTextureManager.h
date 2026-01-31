#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Image;
  class ITexture;
  class IGraphicsManager;
  class AssetManager;

  /**
   * @brief Manages the creation and caching of texture resources.
   *
   * The TextureManager is responsible for creating textures from images or file
   * paths and caching textures to avoid duplication.
   */
  class TextureManager :
    public NonCopyable,
    public IDependencyResolvable
  {
  public:
    TextureManager() = default;
    ~TextureManager() = default;

    /**
     * @copydoc IDependencyResolvable::resolveDependencies
     */
    void resolveDependencies(DependencyContainer& container) override;

    /**
     * @brief Creates a texture from a given image. If a texture for the image
     * already exists, returns the cached instance.
     * 
     * @param image Shared pointer to the image.
     * 
     * @return Shared pointer to the created or cached texture, or nullptr on
     * failure.
     */
    SharedPtr<ITexture> createTextureFromImage(SharedPtr<Image> image);

    /**
     * @brief Creates a texture from an image asset key. Loads the image using
     * the AssetManager and creates or retrieves the corresponding texture.
     * 
     * @param imageKey Asset key identifying the image.
     * 
     * @return Shared pointer to the created or cached texture, or nullptr on
     * failure.
     */
    SharedPtr<ITexture> createTextureFromImage(const String& imageKey);

    /**
     * @brief Creates a texture from an image file path. Loads the image directly
     * from the file and creates or retrieves the corresponding texture.
     * 
     * @param filePath Path to the image file.
     * 
     * @return Shared pointer to the created or cached texture, or nullptr on
     * failure.
     */
    SharedPtr<ITexture> createTextureFromFile(const Path& filePath);

    /**
     * @brief Clears the texture cache, releasing all managed textures.
     */
    void clear();

  private:
    SharedPtr<IGraphicsManager> m_graphicsManager;
    SharedPtr<AssetManager> m_assetManager;
    UnorderedMap<String, SharedPtr<ITexture>> m_textureCache;
  };
}
