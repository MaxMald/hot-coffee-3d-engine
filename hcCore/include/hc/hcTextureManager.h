#pragma once

#include "hc/hcITextureManager.h"
#include "hc/hcResourcesCache.h"

namespace hc
{
  class ITextureFactory;
  class AssetManager;

  /**
   * @brief Manages texture resources and their creation, caching, and retrieval.
   */
  class HC_CORE_EXPORT TextureManager :
    public ITextureManager,
    private ResourcesCache<Id, ITexture>
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
      AssetManager& assetManager
    );
    ~TextureManager() override;

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
     * @brief Returns a vector of all managed texture instances.
     * 
     * @return Const reference to the vector of texture instances.
     */
    const Vector<SharedPtr<ITexture>>& getTextures() override;

    /**
     * @brief Clears all managed textures and cached resources.
     */
    void clear() override;

  private:
    UniquePtr<ITextureFactory> m_textureFactory;
    AssetManager& m_assetManager;
    Vector<SharedPtr<ITexture>> m_textures;
  };
}
