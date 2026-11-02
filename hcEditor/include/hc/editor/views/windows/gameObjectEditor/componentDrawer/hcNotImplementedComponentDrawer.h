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

    /**
     * @brief Draws the specified component. Since this is the not implemented drawer,
     * it typically indicates that no specific drawer is available for the component type.
     *
     * @param component Pointer to the component to be drawn.
     * @param shouldRemove Reference to a bool indicating if the component should be removed.
     */
    virtual void drawComponent(IComponent* component, bool& shouldRemove) override;
  };
}
