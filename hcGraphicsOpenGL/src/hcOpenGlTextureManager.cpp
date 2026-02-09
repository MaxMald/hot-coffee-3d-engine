#include "hc/hcOpenGlTextureManager.h"
#include "hc/hcOpenGlTexture.h"

namespace hc
{
  OpenGlTextureManager::OpenGlTextureManager()
  {
  }

  OpenGlTextureManager::~OpenGlTextureManager()
  {
  }

  SharedPtr<ITexture> OpenGlTextureManager::createTextureFromImage(SharedPtr<Image> image)
  {
    if (!image)
    {
      LogService::Error("Failed to create texture. Image is null.");
      return nullptr;
    }

    if (hasCachedTextureForImageId(image->getId()))
      return m_cachedTexturesByImageId[image->getId()];

    SharedPtr<OpenGlTexture> texture = MakeShared<OpenGlTexture>(image);
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

    addTexture(texture);
    return texture;
  }

  SharedPtr<ITexture> OpenGlTextureManager::createTextureFromImage(const String& imageKey)
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

  SharedPtr<ITexture> OpenGlTextureManager::createTextureFromFile(const Path& filePath)
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

  const Vector<SharedPtr<ITexture>>& OpenGlTextureManager::getTextures()
  {
    return m_textures;
  }

  void OpenGlTextureManager::clear()
  {
    for (const auto& texture : m_textures)
    {
      if (texture)
        texture->destroy();
    }
    m_textures.clear();
    m_cachedTexturesByImageId.clear();
  }

  void OpenGlTextureManager::initialize(SharedPtr<AssetManager> assetManager)
  {
    m_assetManager = assetManager;
  }

  void OpenGlTextureManager::addTexture(SharedPtr<OpenGlTexture> texture)
  {
    m_textures.push_back(texture);
    m_cachedTexturesByImageId[texture->getImage()->getId()] = texture;
  }

  bool OpenGlTextureManager::hasCachedTextureForImageId(const Id& imageId) const
  {
    return m_cachedTexturesByImageId.find(imageId) != 
      m_cachedTexturesByImageId.end();
  }
}
