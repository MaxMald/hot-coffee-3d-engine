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

    Id imageId = image->getId();
    auto it = m_cachedTextures.find(imageId);
    if (it != m_cachedTextures.end())
    {
      return it->second;
    }

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

    m_cachedTextures[imageId] = texture;
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

  void OpenGlTextureManager::clear()
  {
    m_cachedTextures.clear();
  }

  void OpenGlTextureManager::initialize(SharedPtr<AssetManager> assetManager)
  {
    m_assetManager = assetManager;
  }
}
