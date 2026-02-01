#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlGraphicsManager;
  class OpenGlTexture;

  class OpenGlTextureManager : public ITextureManager
  {
  public:
    OpenGlTextureManager();
    virtual ~OpenGlTextureManager() override;

    virtual SharedPtr<ITexture> createTextureFromImage(SharedPtr<Image> image) override;
    virtual SharedPtr<ITexture> createTextureFromImage(const String& imageKey) override;
    virtual SharedPtr<ITexture> createTextureFromFile(const Path& filePath) override;
    virtual void clear() override;

  private:
    SharedPtr<AssetManager> m_assetManager;
    UnorderedMap<Id, SharedPtr<OpenGlTexture>> m_cachedTextures;

    void initialize(SharedPtr<AssetManager> assetManager);

    friend class OpenGlGraphicsManager;
  };
}
