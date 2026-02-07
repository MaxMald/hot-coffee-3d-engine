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
    virtual const Vector<SharedPtr<ITexture>>& getTextures() override;
    virtual void clear() override;

  private:
    SharedPtr<AssetManager> m_assetManager;
    Vector<SharedPtr<ITexture>> m_textures;
    UnorderedMap<Id, SharedPtr<OpenGlTexture>> m_cachedTexturesByImageId;

    void initialize(SharedPtr<AssetManager> assetManager);
    void addTexture(SharedPtr<OpenGlTexture> texture);
    bool hasCachedTextureForImageId(const Id& imageId) const;

    friend class OpenGlGraphicsManager;
  };
}
