#include "hc/editor/hcComponentViewManager.h"
#include "hc/editor/hcMeshComponentView.h"

namespace hc::editor
{
  void ComponentViewManager::DrawComponent(IComponent* component)
  {
    ComponentViewManager::Instance().drawComponent(component);
  }

  void ComponentViewManager::drawComponent(IComponent* component)
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

  void ComponentViewManager::registerComponentView(
    UniquePtr<IComponentView> componentView
  )
  {
    if (!componentView)
      return;

    componentType::Type type = componentView->getComponentType();
    m_componentViews[type] = std::move(componentView);
  }

  void ComponentViewManager::onPrepare()
  {
    registerDefaultComponentViews();
  }

  void ComponentViewManager::onShutdown()
  {
  }

  void ComponentViewManager::registerDefaultComponentViews()
  {
    m_notImplementedView = MakeUnique<NotImplementedComponentView>();
    registerComponentView(MakeUnique<MeshComponentView>());
  }
}
