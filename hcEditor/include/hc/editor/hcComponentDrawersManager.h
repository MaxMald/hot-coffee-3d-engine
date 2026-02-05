#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcIComponentDrawer.h"
#include "hc/editor/hcNotImplementedComponentDrawer.h"

namespace hc::editor
{
  /**
   * @brief Manages registration and retrieval of component drawer in the editor.
   */
  class ComponentDrawersManager : public NonCopyable
  {
  public:
    ComponentDrawersManager();
    virtual ~ComponentDrawersManager();

    /**
     * @brief Draws the specified component using its registered drawer. If no
     * drawer is registered for the component type, uses the fallback drawer.
     * 
     * @param component Pointer to the component to be drawn.
     */
    void drawComponent(IComponent* component);

    /**
     * @brief Registers a new component drawer for a specific component type.
     * 
     * @param componentView Unique pointer to the component drawer to register.
     */
    void registerComponentView(UniquePtr<IComponentDrawer> componentView);

    /**
     * @brief Clears all registered component drawers.
     */
    void clear();

  private:
    UniquePtr<NotImplementedComponentDrawer> m_notImplementedView;
    UnorderedMap<componentType::Type, UniquePtr<IComponentDrawer>> m_componentViews;
  };
}
