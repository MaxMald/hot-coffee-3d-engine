#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  /**
   * @brief Drawer used when no specific drawer is implemented for a component
   * type.
   */
  class NotImplementedComponentDrawer : public IComponentDrawer
  {
  public:
    NotImplementedComponentDrawer() = default;
    virtual ~NotImplementedComponentDrawer() = default;

    virtual componentType::Type getComponentType() const override;
    virtual void drawComponent(IComponent* component) override;
  };
}
