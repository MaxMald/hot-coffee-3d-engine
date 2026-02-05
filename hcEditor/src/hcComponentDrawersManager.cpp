#include "hc/editor/hcComponentDrawersManager.h"

namespace hc::editor
{
  ComponentDrawersManager::ComponentDrawersManager()
  {
  }

  ComponentDrawersManager::~ComponentDrawersManager()
  {
    clear();
  }

  void ComponentDrawersManager::drawComponent(IComponent* component)
  {
    if (!component)
      return;

    componentType::Type type = component->getType();
    auto it = m_componentViews.find(type);
    if (it != m_componentViews.end())
    {
      it->second->drawComponent(component);
    }
    else
    {
      m_notImplementedView->drawComponent(component);
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
