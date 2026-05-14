#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcALightComponentDrawer.h"

namespace hc::editor
{
  class OmniLightComponentDrawer : public ALightComponentDrawer
  {
  public:
    OmniLightComponentDrawer();
    virtual ~OmniLightComponentDrawer() override = default;

  protected:

    virtual void onDrawComponent(ALightComponent* component) override;
  };
}
