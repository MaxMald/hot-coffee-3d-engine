#pragma once

#include "hc/scene/gameObject/components/hcLightComponent.h"
#include "hc/scene/gameObject/components/factories/hcATypedComponentFactory.h"

namespace hc
{
  class SceneManager;

  class LightComponentFactory : public ATypedComponentFactory<LightComponent>
  {
  public:
    LightComponentFactory(SceneManager& sceneManager);
    ~LightComponentFactory() override = default;

    UniquePtr<LightComponent> create() const override;

  private:
    SceneManager& m_sceneManager;
  };
}
