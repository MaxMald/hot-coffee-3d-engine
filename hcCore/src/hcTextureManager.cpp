#include "hc/hcTextureManager.h"
#include "hc/hcITextureFactory.h"
#include "hc/hcImage.h"
#include "hc/hcAssetManager.h"
#include "hc/hcITexture.h"

namespace hc
{
  TextureManager::TextureManager(
    UniquePtr<ITextureFactory> textureFactory,
    AssetManager& assetManager
  ) :
    m_textureFactory(std::move(textureFactory)),
    m_assetManager(assetManager)
  {
  }

  TextureManager::~TextureManager()
  {
  }

  SharedPtr<ITexture> TextureManager::createTextureFromImage(
    SharedPtr<Image> image
  )
  {
    if (!image)
    {
      LogService::Error("Failed to create texture. Image is null.");
      return nullptr;
    }

    if (hasCachedResource(image->getId()))
      return getCachedResource(image->getId());

    SharedPtr<ITexture> texture = m_textureFactory->createTexture(image);
    if (!texture)
    {
      LogService::Error(
        String::Format(
          "Failed to create texture from image at path: %s",
          image->getPath().c_str()
        )
      );

      return nullptr;
    }

    cacheResource(image->getId(), texture);
    m_textures.push_back(texture);

    return texture;
  }

  SharedPtr<ITexture> TextureManager::createTextureFromFile(const Path& filePath)
  {
    SharedPtr<Image> image = m_assetManager.load<Image>(filePath);
    if (!image)
    {
      LogService::Error(
        String::Format(
          "Failed to create texture. Could not load image from file: %s",
          filePath.c_str()
        )
      );

      return nullptr;
    }

    return createTextureFromImage(image);
  }

  const Vector<SharedPtr<ITexture>>& TextureManager::getTextures()
  {
    return m_textures;
  }

  void TextureManager::clear()
  {
    m_textures.clear();
    clearCache();
  }
}
