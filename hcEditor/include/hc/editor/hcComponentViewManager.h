#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcIComponentView.h"
#include "hc/editor/hcNotImplementedComponentView.h"

namespace hc::editor
{
  /**
   * @brief Manages registration and retrieval of component views in the editor.
   *
   * This singleton class is responsible for managing all component view
   * instances. It allows registration of custom component views, provides a
   * default view for unimplemented components, and handles drawing of components
   * via their views.
   */
  class ComponentViewManager : public AModule<ComponentViewManager>
  {
  public:
    static void DrawComponent(IComponent* component);

    ComponentViewManager() = default;
    ~ComponentViewManager() override = default;

    void drawComponent(IComponent* component);
    void registerComponentView(UniquePtr<IComponentView> componentView);

  private:
    UniquePtr<NotImplementedComponentView> m_notImplementedView;
    UnorderedMap<componentType::Type, UniquePtr<IComponentView>> m_componentViews;

    void onPrepare() override;
    void onShutdown() override;

    void registerDefaultComponentViews();
  };
}
