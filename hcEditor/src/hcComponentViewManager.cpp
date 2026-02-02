#include "hc/editor/hcComponentViewManager.h"
#include "hc/editor/hcNotImplementedComponentView.h"
#include "hc/editor/hcMeshComponentView.h"

namespace hc::editor
{
  ComponentViewManager* ComponentViewManager::s_instance = nullptr;

  ComponentViewManager& ComponentViewManager::Instance()
  {
    if (!s_instance)
    {
      throw RuntimeErrorException(
        "ComponentViewManager instance is not prepared. Call Prepare() before accessing the instance."
      );
    }

    return *s_instance;
  }

  void ComponentViewManager::Prepare()
  {
    if (!s_instance)
    {
      s_instance = new ComponentViewManager();
    }
  }

  void ComponentViewManager::Shutdown()
  {
    if (s_instance)
    {
      delete s_instance;
      s_instance = nullptr;
    }
  }

  ComponentViewManager::ComponentViewManager()
  {
    registerDefaultComponentViews();
  }

  ComponentViewManager::~ComponentViewManager()
  {
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

  void ComponentViewManager::registerDefaultComponentViews()
  {
    m_notImplementedView = MakeUnique<NotImplementedComponentView>();
    registerComponentView(MakeUnique<MeshComponentView>());
  }
}
