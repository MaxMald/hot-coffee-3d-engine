#pragma once

#include "hc/editor/hcABaseComponentView.h"

namespace hc::editor
{
  class NotImplementedComponentView : public IComponentView
  {
  public:
    NotImplementedComponentView() = default;
    virtual ~NotImplementedComponentView() = default;

    virtual componentType::Type getComponentType() const override;
    virtual void drawComponent(IComponent* component) override;
  };
}
