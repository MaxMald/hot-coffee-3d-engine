#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class ALightComponentDrawer : public ABaseComponentDrawer<ALightComponent>
  {
  public:
    virtual ~ALightComponentDrawer() override;

  protected:
    ALightComponentDrawer(componentType::Type type);

    virtual void onDrawComponent(ALightComponent* component) override;
  };
}
