#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface for editor component drawers.
   *
   * Provides the contract for all component drawer classes in the editor.
   * Implementations must provide the component type and a method to draw the
   * component.
   */
  class IComponentDrawer
  {
  public:
    virtual ~IComponentDrawer() = default;

    /**
     * @brief Gets the type of the component associated with this view.
     * 
     * @return The component type.
     */
    virtual componentType::Type getComponentType() const = 0;

    /**
     * @brief Draws the component in the editor.
     *
     * @param component Pointer to the component to draw.
     * @param shouldRemove Reference to a bool indicating if the component should be removed.
     */
    virtual void drawComponent(IComponent* component, bool& shouldRemove) = 0;

  protected:
    IComponentDrawer() = default;
  };
}
