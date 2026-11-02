#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersManager.h"

namespace hc::editor
{
  ComponentDrawersManager::ComponentDrawersManager()
  {
  }

  ComponentDrawersManager::~ComponentDrawersManager()
  {
    clear();
  }

  void ComponentDrawersManager::drawComponent(IComponent* component, bool& shouldRemove)
  {
    if (!component)
      return;

    componentType::Type type = component->getType();
    auto it = m_componentViews.find(type);
    if (it != m_componentViews.end())
    {
      it->second->drawComponent(component, shouldRemove);
    }
    else
    {
      m_notImplementedView.drawComponent(component, shouldRemove);
    }
  }

  void ComponentDrawersManager::registerComponentView(
    UniquePtr<IComponentDrawer> componentView
  )
  {
    if (!componentView)
      return;

    componentType::Type type = componentView->getComponentType();
    m_componentViews[type] = std::move(componentView);
  }

  void ComponentDrawersManager::clear()
  {
    m_componentViews.clear();
  }
}
