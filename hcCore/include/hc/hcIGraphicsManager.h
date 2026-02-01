#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcIWindow.h"
#include "hc/hcShaderStageType.h"

namespace hc
{
  class ITexture;
  class IShaderProgram;
  class Image;
  class IDrawable;
  class HotCoffeeEngine;
  class ITextureManager;
  class IMaterialManager;
  class IShaderManager;

  class HC_CORE_EXPORT IGraphicsManager : public NonCopyable
  {
  public:
    virtual ~IGraphicsManager();

    virtual void beginFrame() = 0;
    virtual void draw(IDrawable&) = 0;
    virtual void endFrame(IWindow&) = 0;
    virtual ITextureManager& getTextureManager() = 0;
    virtual IMaterialManager& getMaterialManager() = 0;
    virtual IShaderManager& getShaderManager() = 0;
    virtual SharedPtr<IShaderProgram> createUnlitShaderProgram() const = 0;

  protected:
    IGraphicsManager();

    virtual void init() = 0;
    virtual void destroy() = 0;

    friend HotCoffeeEngine;
  };
}
