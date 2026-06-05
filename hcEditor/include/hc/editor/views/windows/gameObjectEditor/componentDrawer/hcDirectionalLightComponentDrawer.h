#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcALightComponentDrawer.h"

namespace hc::editor
{
  class DirectionalLightComponentDrawer : public ALightComponentDrawer
  {
  public:
    DirectionalLightComponentDrawer();
    virtual ~DirectionalLightComponentDrawer() override = default;

  protected:

    virtual void onDrawComponent(ALightComponent* component) override;
  };
}
