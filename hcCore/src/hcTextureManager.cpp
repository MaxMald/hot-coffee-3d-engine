#include "hc/hcTextureManager.h"
#include "hc/hcAssetManager.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcImage.h"

namespace hc
{
  void TextureManager::resolveDependencies(DependencyContainer& container)
  {
    m_assetManager = container.resolve<AssetManager>();
    m_graphicsManager = container.resolve<IGraphicsManager>();
  }

  SharedPtr<ITexture> TextureManager::createTextureFromImage(SharedPtr<Image> image)
  {
    if (!image)
    {
      LogService::Error("Failed to create texture. Image is null.");
      return nullptr;
    }

    String cacheKey = image->getPath().string();

    auto it = m_textureCache.find(cacheKey);
    if (it != m_textureCache.end())
    {
      return it->second;
    }

    SharedPtr<ITexture> texture = m_graphicsManager->createTexture(image);
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

    m_textureCache[cacheKey] = texture;
    return texture;
  }

  SharedPtr<ITexture> TextureManager::createTextureFromImage(const String& imageKey)
  {
    SharedPtr<Image> image = m_assetManager->get<Image>(imageKey);
    if (!image)
    {
      LogService::Error(
        String::Format(
          "Failed to create texture. Image with key '%s' not found.",
          imageKey.c_str()
        )
      );

      return nullptr;
    }

    return createTextureFromImage(image);
  }

  SharedPtr<ITexture> TextureManager::createTextureFromFile(
    const Path& filePath
  )
  {
    SharedPtr<Image> image = m_assetManager->load<Image>(filePath);
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

  void TextureManager::clear()
  {
    m_textureCache.clear();
  }
}
