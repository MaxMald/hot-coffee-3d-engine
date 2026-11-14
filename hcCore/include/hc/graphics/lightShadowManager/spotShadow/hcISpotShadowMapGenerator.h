#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/lights/hcLight.h"

namespace hc
{
  class SceneGraph;
}

namespace hc::graphics::resource::textureArray
{
  class ITextureArray;
}

namespace hc::graphics::shadows::spotShadow
{
  class HC_CORE_EXPORT ISpotShadowMapGenerator
  {
  public:
    virtual ~ISpotShadowMapGenerator();

    virtual void generateSpotShadowMap(
      const scene::lights::Light& spotLight,
      const SceneGraph& scene,
      graphics::resource::textureArray::ITextureArray& textureArray,
      UInt32 layer
    ) = 0;

  protected:
    ISpotShadowMapGenerator();
  };
}
