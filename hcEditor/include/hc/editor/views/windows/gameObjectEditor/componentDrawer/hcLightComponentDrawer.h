#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class LightComponentDrawer : public ABaseComponentDrawer<LightComponent>
  {
  public:
    LightComponentDrawer();
    virtual ~LightComponentDrawer() override;

  protected:
    virtual void onDrawComponent(LightComponent* component) override;
    void drawDirectionalProperties(scene::lights::Light& light);
    void drawSpotProperties(scene::lights::Light& light);
    void drawOmniProperties(scene::lights::Light& light);
    void drawDirectionalShadowProperties(scene::lights::Light& light);
    void drawSpotShadowProperties(scene::lights::Light& light);
    void drawOmniShadowProperties(scene::lights::Light& light);
  };
}
