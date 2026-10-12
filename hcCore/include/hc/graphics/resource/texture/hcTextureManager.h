#pragma once

#include "hc/graphics/resource/texture/hcITextureManager.h"
#include "hc/graphics/resource/hcResourcesCache.h"

namespace hc
{
  class ITextureFactory;
  class IAssetManager;

  /**
   * @brief Manages texture resources and their creation, caching, and retrieval.
   */
  class HC_CORE_EXPORT TextureManager :
    public ITextureManager,
    private ResourcesCache<UUID, ITexture>
  {
  public:
    /**
     * @brief Constructs a TextureManager with the required dependencies.
     *
     * @param textureFactory Unique pointer to the texture factory used for texture
     * creation.
     * @param assetManager Reference to the asset manager.
     */
    TextureManager(
      UniquePtr<ITextureFactory> textureFactory,
      IAssetManager& assetManager
    );
    ~TextureManager() override;

    /**
     * @copydoc ITextureManager::createTexture
     */
    SharedPtr<ITexture> createTexture() override;

    /**
     * @brief Creates a texture from the given image.
     *
     * @param image Shared pointer to the image used for texture creation.
     *
     * @return Shared pointer to the created texture instance. nullptr if
     * creation failed.
     */
    SharedPtr<ITexture> createTextureFromImage(SharedPtr<Image> image) override;

    /**
     * @brief Creates a texture from the specified file path.
     *
     * @param filePath Path to the texture file.
     *
     * @return Shared pointer to the created texture instance. nullptr if
     * creation failed.
     */
    SharedPtr<ITexture> createTextureFromFile(const Path& filePath) override;

    /**
     * @copydoc ITextureManager::createTextureFromFile(const Path&, colorSpaceType::Type)
     */
    SharedPtr<ITexture> createTextureFromFile(
      const Path& filePath,
      colorSpaceType::Type colorSpace
    ) override;

    /**
     * @brief Returns a vector of all managed texture instances.
     * 
     * @return Const reference to the vector of texture instances.
     */
    const Vector<SharedPtr<ITexture>>& getTextures() override;

    /**
     * @copydoc ITextureManager::getDefaultTexture
     */
    SharedPtr<ITexture> getDefaultTexture(defaultTextureType::Type type) override;

    /**
     * @brief Clears all managed textures and cached resources.
     */
    void clear() override;

  private:
    UniquePtr<ITextureFactory> m_textureFactory;
    IAssetManager& m_assetManager;
    Vector<SharedPtr<ITexture>> m_textures;
    UnorderedMap<defaultTextureType::Type, SharedPtr<ITexture>> m_defaultTextures;

    /**
     * @brief Creates default textures for the specified type and saves them in the
     * default textures map.
     * 
     * @param type The type of default texture to create.
     * @param image The image data to use for the default texture.
     * @return Shared pointer to the created default texture.
     */
    SharedPtr<ITexture> createAndSaveDefaultTexture(
      defaultTextureType::Type type,
      const Image& image
    );
  };
}
