#pragma once

#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIGameObjectSelectionServiceListener.h"
#include "hc/editor/hcCreateComponentSection.h"
#include "hc/editor/hcComponentDrawersManager.h"

namespace hc::editor
{
  class ProjectFileSelector;
  class GameObjectSelectionService;

  class GameObjectEditorWindow : public AWindowView
  {
  public:
    GameObjectEditorWindow(
      HotCoffeeEngine& hotCoffeeEngine,
      ProjectFileSelector& projectFileSelector,
      GameObjectSelectionService& gameObjectSelectionService
    );
    virtual ~GameObjectEditorWindow();

  protected:
    GameObjectSelectionService& m_gameObjectSelectionService;
    CreateComponentSection m_createComponentSection;
    ComponentDrawersManager m_componentDrawersManager;
    Vector<IComponent*> m_gameObjectComponents;

    void onDraw() override;
    void onDestroy() override;
    void drawTransform(GameObject* gameObject);
    void drawCreateComponentSection(GameObject* gameObject);
    void drawComponents(GameObject* gameObject);
  };
}
