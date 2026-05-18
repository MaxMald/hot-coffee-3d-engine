#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcALightComponentDrawer.h"

namespace hc::editor
{
  class SpotLightComponentDrawer : public ALightComponentDrawer
  {
  public:
    SpotLightComponentDrawer();
    virtual ~SpotLightComponentDrawer() override = default;

  protected:
    virtual void onDrawComponent(ALightComponent* component) override;
  };
}
