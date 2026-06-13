#include "hc/graphics/resource/texture/hcTextureManager.h"
#include "hc/graphics/resource/texture/hcITextureFactory.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/hcIAssetManager.h"

namespace hc
{
  TextureManager::TextureManager(
    UniquePtr<ITextureFactory> textureFactory,
    IAssetManager& assetManager
  ) :
    m_textureFactory(std::move(textureFactory)),
    m_assetManager(assetManager)
  {
  }

  TextureManager::~TextureManager()
  {
  }

  SharedPtr<ITexture> TextureManager::createTexture()
  {
    SharedPtr<ITexture> texture = m_textureFactory->createTexture();
    if (!texture)
    {
      LogService::Error("Failed to create empty texture.");
      return nullptr;
    }

    m_textures.push_back(texture);
    return texture;
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
    SharedPtr<Image> image = m_assetManager.getImageAssetManager().load(filePath);
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

  SharedPtr<ITexture> TextureManager::createTextureFromFile(
    const Path& filePath,
    colorSpaceType::Type colorSpace
  )
  {
    SharedPtr<Image> image = m_assetManager.getImageAssetManager().load(filePath);
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

    image->setColorSpace(colorSpace);
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
