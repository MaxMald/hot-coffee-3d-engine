#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcIComponentView.h"

namespace hc::editor
{
  class NotImplementedComponentView;

  /**
   * @brief Manages registration and retrieval of component views in the editor.
   *
   * This singleton class is responsible for managing all component view
   * instances. It allows registration of custom component views, provides a
   * default view for unimplemented components, and handles drawing of components
   * via their views.
   */
  class ComponentViewManager
  {
  public:
    static ComponentViewManager& Instance();
    static void Prepare();
    static void Shutdown();

    void drawComponent(IComponent* component);
    void registerComponentView(UniquePtr<IComponentView> componentView);

  private:
    static ComponentViewManager* s_instance;

    UniquePtr<NotImplementedComponentView> m_notImplementedView;
    UnorderedMap<componentType::Type, UniquePtr<IComponentView>> m_componentViews;

    ComponentViewManager();
    ~ComponentViewManager();

    void registerDefaultComponentViews();
  };
}
